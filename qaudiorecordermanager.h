#ifndef QAUDIORECORDERMANAGER_H
#define QAUDIORECORDERMANAGER_H

#include <QObject>
#include <QAudioRecorder>
#include <QAudioEncoderSettings>
#include <QAudioDeviceInfo>
#include <QAudioProbe>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include <QPointer>

/**
 * A wrapper over QAudioRecorder to make it work reliably.
 * Alot but not all methods+signals of QAudioRecorder are proxied.
 */
class QAudioRecorderManager : public QObject
{
    Q_OBJECT
public:
    explicit QAudioRecorderManager(QObject *parent = nullptr);
    ~QAudioRecorderManager();
    QString	audioInput() const { return m_recorder != nullptr ? m_recorder->audioInput() : QString(); }
    QString audioInputDescription(const QString & name) const { return m_recorder != nullptr ? m_recorder->audioInputDescription(name) : QString(); }
    QStringList	audioInputs() const { return m_recorder != nullptr ? m_recorder->audioInputs() : QStringList(); }
    QString	defaultAudioInput() const { return m_recorder != nullptr ? m_recorder->defaultAudioInput() : QString(); }
    void setAudioInput(const QString& name) { m_name = name; }
    QStringList	supportedAudioCodecs() const { return m_recorder != nullptr ? m_recorder->supportedAudioCodecs() : QStringList(); }
    QStringList	supportedContainers() const { return m_recorder != nullptr ? m_recorder->supportedContainers() : QStringList(); }
    QUrl outputLocation() const { return m_location; }
    bool setOutputLocation(const QUrl & location) { m_location = location; return true; }
    void setContainerFormat(const QString & container) { m_container = container; }
    void setAudioSettings(const QAudioEncoderSettings & settings) { m_settings = settings; }
    QAudioRecorder::State state() const { return m_recorder->state(); }
    QAudioRecorder::Status status() const { return m_recorder->status(); }
    QPointer<QAudioRecorder> getRecorder() { return QPointer<QAudioRecorder>(m_recorder); }
    QAudioProbe& getAudioProbe() { return m_probe; }
    void initAudioRecorder();

private:
    QAudioRecorder * m_recorder = nullptr;
    QAudioProbe m_probe;
    QString m_name;
    QUrl m_location;
    QString m_container;
    QAudioEncoderSettings m_settings;

public slots:
    void pause();
    void stop();
    void record();
    void newRecording();

signals:
    // Proxied from QAudioRecorder.
    void audioInputChanged(const QString & name);
    void availableAudioInputsChanged();
    // Proxied from QMedia Recorder.
    void actualLocationChanged(const QUrl & location);
    void availabilityChanged(bool available);
    void availabilityChanged(QMultimedia::AvailabilityStatus availability);
    void durationChanged(qint64 duration);
    void error(QMediaRecorder::Error error);
    void metaDataAvailableChanged(bool available);
    void metaDataChanged();
    void metaDataChanged(const QString & key, const QVariant & value);
    void metaDataWritableChanged(bool writable);
    void mutedChanged(bool muted);
    void stateChanged(QMediaRecorder::State state);
    void statusChanged(QMediaRecorder::Status status);
    void volumeChanged(qreal volume);
};

#endif // QAUDIORECORDERMANAGER_H
