#include "audiotraprecorder.h"
#include "audioutils.h"
#include <signal.h>
#include <QCoreApplication>
#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QDir>
#include <QDateTime>
#include <QMimeDatabase>
#include <unistd.h>

AudioTrapRecorder::AudioTrapRecorder(QObject *parent) : QObject(parent) {
    qDebug().noquote() << AudioUtils::audioEnvInfo() << "\n";
    checkAudioViable();
    readSettings();
    connect(&recorderManager, SIGNAL(audioInputChanged(const QString)), this, SLOT(audioInputChanged(const QString)), Qt::UniqueConnection);
    connect(&recorderManager, SIGNAL(availabilityChanged(bool)), this, SLOT(availabilityChanged(bool)), Qt::UniqueConnection);
    connect(&recorderManager, SIGNAL(actualLocationChanged(QUrl)), this, SLOT(actualLocationChanged(QUrl)), Qt::UniqueConnection);
    connect(&recorderManager, SIGNAL(stateChanged(QMediaRecorder::State)), this, SLOT(stateChanged(QMediaRecorder::State)), Qt::UniqueConnection);
    connect(&recorderManager, SIGNAL(statusChanged(QMediaRecorder::Status)), this, SLOT(statusChanged(QMediaRecorder::Status)), Qt::UniqueConnection);
    connect(&recorderManager, SIGNAL(error(QMediaRecorder::Error)), this, SLOT(error(QMediaRecorder::Error)), Qt::UniqueConnection);
    connect(&recorderManager.getAudioProbe(), SIGNAL(audioBufferProbed(QAudioBuffer)), &levelMeter, SLOT(read(QAudioBuffer)), Qt::UniqueConnection);
    connect(&levelMeter, SIGNAL(levelChange(AudioLevelMeter::ThresholdState)), this, SLOT(levelChange(AudioLevelMeter::ThresholdState)), Qt::UniqueConnection);
    connect(&levelMeter, SIGNAL(update(int)), this, SLOT(update(int)), Qt::UniqueConnection);
}

AudioTrapRecorder::~AudioTrapRecorder() {
    quit();
}

void AudioTrapRecorder::quit() {
    qDebug() << "Audio trap recorder quitting";
    nullRecording();
    saveSettings();
}

void AudioTrapRecorder::checkAudioViable() {
    QAudioRecorder recorder;
    if(!recorder.defaultAudioInput().size()) {
        qFatal("Could not find an audio device. Check audio configuration and try again.");
    }
    if(!recorder.supportedAudioCodecs().size()) {
        qFatal("List of supported audio codecs is empty. Check audio configuration and try again.");
    }
}

/**
 * Read settings from disk or fall back on defaults. Bit laborious.
 */
void AudioTrapRecorder::readSettings() {
    QSettings settings;
    setOutputDir(settings.value("outputDir", QDir::homePath()).toString());
    setDeviceName(settings.value("deviceName", ":default").toString());
    setContainerFormat(settings.value("containerFormat", "audio/wav").toString());;
    levelMeter.setLowThreshold(settings.value("lowThreshold", 0.2).toReal());
    levelMeter.setHighThreshold(settings.value("highThreshold", 0.3).toReal());
    levelMeter.setDampening(settings.value("dampening", 0.90).toReal());
    setTailTime(settings.value("tailTime", 5000).toInt());
}

void AudioTrapRecorder::saveSettings() {
    QSettings settings;
    qDebug() << "Savinng settings";
    settings.setValue("outputDir", outputDir);
    settings.setValue("deviceName", deviceName);
    settings.setValue("containerFormat", containerFormat);
    settings.setValue("lowThreshold", levelMeter.getLowThreshold());
    settings.setValue("highThreshold", levelMeter.getHighThreshold());
    settings.setValue("dampening", levelMeter.getDampening());
}

void AudioTrapRecorder::activate() {
    qDebug() << "Audio trap recorder activated";
    m_active = true;
    nullRecording(); // Prime the recorder.
    emit activated();
}

void AudioTrapRecorder::deactivate() {
    qDebug() << "Audio trap recorder deactivated";
    m_active = false;
    m_recording = false;
    recorderManager.stop();
    emit deactivated();
}

/**
 * Enter stopped state, setting up recording to /dev/null.
 */
void AudioTrapRecorder::nullRecording() {
    QString last = recorderManager.outputLocation().toLocalFile();
    recorderManager.setOutputLocation(QUrl::fromLocalFile("/dev/null"));
    recorderManager.newRecording();
    m_recording = false;
    if(last != "/dev/null") {
        emit stopped(last);
        qDebug() << "Stopped " << last;
    }
}

/**
 * Enter recording state.
 */
void AudioTrapRecorder::newRecording() {
    recorderManager.setOutputLocation(QUrl::fromLocalFile(nextFile()));
    recorderManager.newRecording();
    m_recording = true;
    emit recording(recorderManager.outputLocation().toString());
    qDebug() << "Recording " << recorderManager.outputLocation().toString();
}

void AudioTrapRecorder::tailOut() {
    if(m_recording) {
        nullRecording();
    }
}


/**
 * React to changing level.
 */
void AudioTrapRecorder::levelChange(AudioLevelMeter::ThresholdState level) {
    qDebug() << "Level change " << level;
    if(!m_active) {
        return;
    }
    else if(level == AudioLevelMeter::HIGH) {
        if(m_recording == false) {
            newRecording();
        }
        tailTimer.stop();
    }
    else {
        tailTimer.singleShot(tailTime, this, SLOT(tailOut()));
    }
}

void AudioTrapRecorder::update(int read) {
    (void)(read);
    static int counter = 0;
    if(++counter % 100 == 0)
        qDebug() << levelMeter;
}

QString AudioTrapRecorder::nextFile() {
    QMimeDatabase db;
    QString ext = db.mimeTypeForName(containerFormat).suffixes()[0];
    return outputDir + "/audio-" + QDateTime::currentDateTimeUtc().toString("yyyy-MM-dd-hh-mm-ss-t") + "." + ext;
}

qint32 AudioTrapRecorder::getTailTime() const {
    return tailTime;
}

void AudioTrapRecorder::setTailTime(int value) {
    tailTime = value;
}


QString AudioTrapRecorder::getDeviceName() const {
    return deviceName;
}

void AudioTrapRecorder::setDeviceName(const QString &value) {
    deviceName = value;
    recorderManager.setAudioInput(deviceName);
    QAudioDeviceInfo deviceInfo = AudioUtils::getAudioDeviceInfo(deviceName);
    levelMeter.setFormat(deviceInfo.preferredFormat());
}

QString AudioTrapRecorder::getContainerFormat() const {
    return containerFormat;
}

void AudioTrapRecorder::setContainerFormat(const QString &value) {
    containerFormat = value;
    recorderManager.setContainerFormat(value);
}

QString AudioTrapRecorder::getOutputDir() const {
    return outputDir;
}

void AudioTrapRecorder::setOutputDir(const QString &value) {
    outputDir = value;
}

void AudioTrapRecorder::audioInputChanged(const QString name) {
    qDebug() << "audioInputChanged: " << name;
}

void AudioTrapRecorder::stateChanged(QMediaRecorder::State state) {
    qDebug() << "stateChanged: " << AudioUtils::MEDIA_STATE_STRING[state];
}

void AudioTrapRecorder::statusChanged(QMediaRecorder::Status status) {
    qDebug() << "statusChanged: " << AudioUtils::MEDIA_STATUS_STRING[status];
}

void AudioTrapRecorder::availabilityChanged(bool available) {
    qDebug() << "availablityChanged" << available;
}

void AudioTrapRecorder::actualLocationChanged(QUrl url) {
    qDebug() << "actualLocationChanged" << url.toString();
}

void AudioTrapRecorder::error(QMediaRecorder::Error error) {
    qCritical() << AudioUtils::MEDIA_ERROR_STRING[error];
    //deactivate();
}
