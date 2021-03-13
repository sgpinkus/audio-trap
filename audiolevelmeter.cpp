#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <QDebug>
#include <QAudioDeviceInfo>
#include <qendian.h>
#include "audiolevelmeter.h"

AudioLevelMeter::AudioLevelMeter(QObject *parent) : QObject(parent) {
    setFormat(QAudioFormat());
}

AudioLevelMeter::~AudioLevelMeter() {
}

/**
 * Check the format is supported and calculate m_maxAmplitude.
 * maxAmplitude is only used for noramlizing samples.
 */
bool AudioLevelMeter::setFormat(const QAudioFormat &format) {
    m_audioFormat = format;
    m_channelCount = format.channelCount();
    m_validAudioFormat = true;
    if(m_audioFormat.channelCount() <= 0) {
        m_validAudioFormat = false;
    }
    else {
        switch (m_audioFormat.sampleSize()) {
            case 8:
                switch (m_audioFormat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    m_maxAmplitude = 255;
                    break;
                case QAudioFormat::SignedInt:
                    m_maxAmplitude = 128;
                    break;
                default:
                    break;
                }
                break;
            case 16:
                switch (m_audioFormat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    m_maxAmplitude = 65535;
                    break;
                case QAudioFormat::SignedInt:
                    m_maxAmplitude = 32768;
                    break;
                default:
                    break;
                }
                break;
            case 32:
                switch (m_audioFormat.sampleType()) {
                case QAudioFormat::UnSignedInt:
                    m_maxAmplitude = 0xffffffff;
                    break;
                case QAudioFormat::SignedInt:
                    m_maxAmplitude = 0x80000000;
                    break;
                case QAudioFormat::Float:
                    m_maxAmplitude = 0x80000000; // Kind of
                    break;
                default:
                    break;
                }
                break;
            default:
                // qWarning() << "Unsupported audio format:" << m_audioFormat;
                m_validAudioFormat = false;
        }
    }
    if(m_validAudioFormat) {
        init();
    }
    return m_validAudioFormat;
}

/**
 * Ensure lists have correct size and initial values. Bit shit.
 */
void AudioLevelMeter::init() {
    QList<qreal> l;
    for(qint32 i = 0; i < m_channelCount; i++) {
        l.insert(i, 0.0);
    }
    m_avgAmplitude = m_expAmplitude = m_peakAmplitude = l;
}

/**
 * Process next audio buff. Data is expected to be formatted according to our QAudioFormat.
 * All levels are normalized to maxAmplitud for convenience - between [0,1].
 */
int AudioLevelMeter::read(const QAudioBuffer &buffer) {
    if(!isValid()) {
        return 0;
    }
    const char * data = static_cast<const char*>(buffer.data());
    qint32 len = buffer.byteCount();
    const qint32 channelBytes = m_audioFormat.sampleSize() / 8;
    const qint32 sampleBytes = m_channelCount * channelBytes;
    const qint32 numSamples = len / sampleBytes;
    Q_ASSERT(len % sampleBytes == 0);

    for (qint32 j = 0; j < m_channelCount; ++j) {
        const unsigned char *ptr = reinterpret_cast<const unsigned char *>(data);
        quint32 maxAmpValue = 0;  // Max amplitude value in seen in working channel.
        quint32 sumAmpValue = 0;  // Sum amplitude values in working channel.
        ptr += j*channelBytes;
        for(qint32 i = 0; i < numSamples; ++i) {
            qint64 value = 0;
            if(m_audioFormat.sampleSize() == 8 && m_audioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                value = *reinterpret_cast<const quint8*>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 8 && m_audioFormat.sampleType() == QAudioFormat::SignedInt) {
                value = *reinterpret_cast<const qint8*>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 16 && m_audioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                if(m_audioFormat.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint16>(ptr);
                else
                    value = qFromBigEndian<quint16>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 16 && m_audioFormat.sampleType() == QAudioFormat::SignedInt) {
                if(m_audioFormat.byteOrder() == QAudioFormat::LittleEndian) {
                    value = qFromLittleEndian<qint16>(ptr);
                }
                else
                    value = qFromBigEndian<qint16>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 32 && m_audioFormat.sampleType() == QAudioFormat::UnSignedInt) {
                if(m_audioFormat.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<quint32>(ptr);
                else
                    value = qFromBigEndian<quint32>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 32 && m_audioFormat.sampleType() == QAudioFormat::SignedInt) {
                if(m_audioFormat.byteOrder() == QAudioFormat::LittleEndian)
                    value = qFromLittleEndian<qint32>(ptr);
                else
                    value = qFromBigEndian<qint32>(ptr);
            }
            else if(m_audioFormat.sampleSize() == 32 && m_audioFormat.sampleType() == QAudioFormat::Float) {
                value = *reinterpret_cast<const float*>(ptr) * 0x80000000; // assumes 0-1.0
            }
            sumAmpValue += qAbs(value);
            maxAmpValue = qMax(quint32(qAbs(value)), maxAmpValue);
            ptr += sampleBytes;
        }
        m_avgAmplitude[j] = (qreal(sumAmpValue) / (numSamples * m_channelCount)) / m_maxAmplitude;
        m_expAmplitude[j] = qreal(m_expAmplitude[j]*m_alphaExp + m_avgAmplitude[j]*(1.0-m_alphaExp));
        m_peakAmplitude[j] = qreal(maxAmpValue) / m_maxAmplitude;
    }
    updateThresholLevel();
    emit update(len);
    return len;
}

qreal AudioLevelMeter::setLowThreshold(qreal l) {
    m_lowLevelThreshold = qMax(0.0, qMin(1.0, l));
    if(m_lowLevelThreshold > m_highLevelThreshold) {
        m_highLevelThreshold = m_lowLevelThreshold;
    }
    return m_lowLevelThreshold;
}

qreal AudioLevelMeter::setHighThreshold(qreal h) {
    m_highLevelThreshold = qMax(0.0, qMin(1.0, h));
    if(m_lowLevelThreshold > m_highLevelThreshold) {
        m_lowLevelThreshold = m_highLevelThreshold;
    }
    return m_highLevelThreshold;
}

quint32 AudioLevelMeter::maxAmplitude() const {
    return m_maxAmplitude;
}

QList<qreal> AudioLevelMeter::avgAmplitude() const {
    return m_avgAmplitude;
}

QList<qreal> AudioLevelMeter::expAmplitude() const {
    return m_expAmplitude;
}

QList<qreal> AudioLevelMeter::peakAmplitude() const {
    return m_peakAmplitude;
}

qreal AudioLevelMeter::channelAvgAmplitude() const {
    qreal v = 0;
    foreach(qreal s, m_avgAmplitude) {
        v += s;
    }
    return v/m_channelCount;
}

qreal AudioLevelMeter::channelExpAmplitude() const {
    qreal v = 0;
    foreach(qreal s, m_expAmplitude) {
        v += s;
    }
    return v/m_channelCount;
}

qreal AudioLevelMeter::channelPeakAmplitude() const {
    qreal v = 0;
    foreach(qreal s, m_peakAmplitude) {
        v += s;
    }
    return v/m_channelCount;
}

void AudioLevelMeter::updateThresholLevel() {
    if(m_thresholdState == HIGH) {
        if(channelExpAmplitude() < m_lowLevelThreshold) {
            m_thresholdState = LOW;
            emit levelChange(LOW);
        }
    }
    else {
        if(channelExpAmplitude() > m_highLevelThreshold) {
            m_thresholdState = HIGH;
            emit levelChange(HIGH);
        }
    }
}

QDebug operator<<(QDebug dbg, AudioLevelMeter &s) {
    QDebugStateSaver saver(dbg);
    dbg.nospace()  << "STATE:"          << ((s.thresholdState() == AudioLevelMeter::LOW) ? "LOW" : "HIGH") << "; "
                   << "THRESHOLDS:"     << s.thresholds() << "; "
                   << "TRIGGERLEVEL:"   << s.channelExpAmplitude() << "; "
                   << "MAX_AMP:"        << s.maxAmplitude() << "; "
                   << "PEAK_AMP:"       << s.peakAmplitude() << "; "
                   << "AVG_AMP:"        << s.avgAmplitude() << "; "
                   << "EXP_AMP:"        << s.expAmplitude() << "; ";
    return dbg;
}
