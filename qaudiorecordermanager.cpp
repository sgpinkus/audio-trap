#include "qaudiorecordermanager.h"

QAudioRecorderManager::QAudioRecorderManager(QObject *parent) :
    QObject(parent)
{
    m_settings.setQuality(QMultimedia::HighQuality);
    m_location = "/dev/null";
    initAudioRecorder();
}

QAudioRecorderManager::~QAudioRecorderManager()
{
    stop();
}

/**
 * Create a new QAudioRecorder instance, pass in setting from this object, and hook up signals.
 * For some reason need to create new heap dynamic recorder with every recording - otherwise hangy shit happens.
 * Set up QAudioProbe too since if client want this it's a PITA to constantly reattach to new recorder.
 */
void QAudioRecorderManager::initAudioRecorder()
{
    // qDebug() << "Init new QAudioRecorder" << m_name << m_container << m_location;
    if(m_recorder != nullptr) {
        delete m_recorder;
    }
    m_recorder = new QAudioRecorder(this);
    m_recorder->setAudioInput(m_name);
    m_recorder->setAudioSettings(m_settings);
    m_recorder->setContainerFormat(m_container);
    m_recorder->setOutputLocation(m_location);
    m_probe.setSource(m_recorder);
    // Proxy signals.
    connect(m_recorder, SIGNAL(audioInputChanged(const QString)), this, SIGNAL(audioInputChanged(const QString)), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(availableAudioInputsChanged()), this, SIGNAL(availableAudioInputsChanged()), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(actualLocationChanged(QUrl)), this, SIGNAL(actualLocationChanged(QUrl)), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(availabilityChanged(bool)), this, SIGNAL(availabilityChanged(bool)), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(error(QMediaRecorder::Error)), this, SIGNAL(error(QMediaRecorder::Error)), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(stateChanged(QMediaRecorder::State)), this, SIGNAL(stateChanged(QMediaRecorder::State)), Qt::UniqueConnection);
    connect(m_recorder, SIGNAL(statusChanged(QMediaRecorder::Status)), this, SIGNAL(statusChanged(QMediaRecorder::Status)), Qt::UniqueConnection);
}

void QAudioRecorderManager::pause()
{
    if(m_recorder != nullptr && m_recorder->state() != QAudioRecorder::PausedState) {
        m_recorder->pause();
    }
}

void QAudioRecorderManager::stop()
{
    if(m_recorder != nullptr && m_recorder->state() != QAudioRecorder::StoppedState) {
        m_recorder->stop();
    }
}

void QAudioRecorderManager::record()
{
    if(m_recorder != nullptr && m_recorder->state() != QAudioRecorder::RecordingState) {
        m_recorder->record();
    }
}

void QAudioRecorderManager::newRecording()
{
    stop();
    initAudioRecorder();
    m_recorder->record();
}
