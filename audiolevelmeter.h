#ifndef AUDIOLEVELMONITOR_H
#define AUDIOLEVELMONITOR_H

#include <QObject>
#include <QAudioBuffer>


/**
 * An audio level meter.
 * Reads data from a QAudioBuffer and updates audio level readings.
 * Also implements audio a heuristic trigger signal based on the average level of the audio.
 */
class AudioLevelMeter : public QObject
{
    Q_OBJECT

public:
    typedef enum { LOW, HIGH } ThresholdState;

    AudioLevelMeter(QObject *parent = NULL);
    ~AudioLevelMeter();
    void init();
    QAudioFormat format() const { return m_audioFormat; }
    bool setFormat(const QAudioFormat &format);
    void setDampening(qreal d) { m_alphaExp = qMax(0.0, qMin(1.0, d)); }
    qreal getDampening() const {  return m_alphaExp; }
    qreal getLowThreshold() const { return m_lowLevelThreshold; }
    qreal setLowThreshold(qreal l);
    qreal getHighThreshold() const { return m_highLevelThreshold; }
    qreal setHighThreshold(qreal h);
    QPair<qreal,qreal> thresholds() const { return QPair<qreal,qreal>(m_lowLevelThreshold, m_highLevelThreshold); }
    bool isValid() { return m_validAudioFormat; }
    quint32 maxAmplitude() const;
    QList<qreal> avgAmplitude() const;
    QList<qreal> expAmplitude() const;
    QList<qreal> peakAmplitude() const;
    qreal channelAvgAmplitude() const;
    qreal channelExpAmplitude() const;
    qreal channelPeakAmplitude() const;
    ThresholdState thresholdState() const { return m_thresholdState; }

private slots:
    int read(const QAudioBuffer &buffer);

private:
    void updateThresholLevel();
    QAudioFormat m_audioFormat;
    bool m_validAudioFormat = false;
    quint32 m_channelCount = 0;
    qreal m_alphaExp = 0.990;
    quint32 m_maxAmplitude = 0;
    QList<qreal> m_peakAmplitude;
    QList<qreal> m_avgAmplitude;
    QList<qreal> m_expAmplitude;
    ThresholdState m_thresholdState = LOW;
    qreal m_lowLevelThreshold = 0.2;
    qreal m_highLevelThreshold = 0.4;

signals:
    void levelChange(AudioLevelMeter::ThresholdState level);
    void update(int read);
};

QDebug operator<<(QDebug dbg, AudioLevelMeter &s);

#endif // AUDIOLEVELMONITOR_H
