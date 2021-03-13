#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioEncoderSettings>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include "audiolevelmeter.h"
#include "qaudiorecordermanager.h"

/**
 * Wrapper over QAudioRecordingManager - which is wrapper over QAudioRecorder.
 * Implement triggering recording based on auio level reading and other app logic.
 * This class has four main states: activated/deactived and recording/stopped.
 * When activated the underlyig recorder is always "recording" to /dev/null to allow probing the signal.
 * Whe the signal goes high/low the state here changes recording/stopped.
 */
class AudioTrapRecorder : public QObject
{
    Q_OBJECT
public:
    explicit AudioTrapRecorder(QObject *parent = nullptr);
    ~AudioTrapRecorder();
    QString	audioInput() const { return recorderManager.audioInput(); }
    QString	audioInputDescription(const QString & name) const { return recorderManager.audioInputDescription(name); }
    QStringList	audioInputs() { return recorderManager.audioInputs(); }
    void setAudioInput(const QString& name) { recorderManager.setAudioInput(name); }
    QString	defaultAudioInput() const { return recorderManager.defaultAudioInput(); }
    QStringList	supportedContainers() const { return recorderManager.supportedContainers(); }
    QAudioRecorderManager& getRecorderManager() { return recorderManager; }
    AudioLevelMeter& getAudioLevelMeter() { return levelMeter; }
    QString getDeviceName() const;
    void setDeviceName(const QString &value);
    QString getOutputDir() const;
    void setOutputDir(const QString &value);
    QString getContainerFormat() const;
    void setContainerFormat(const QString &value);
    qint32 getTailTime() const;
    void setTailTime(int value);
    bool isRecording() const { return m_recording; }
    bool isActive() const { return m_active; }

private:
    void checkAudioViable();
    void readSettings();
    void saveSettings();
    QString nextFile();
    QAudioRecorderManager recorderManager;
    AudioLevelMeter levelMeter;        /* Measures RT ausio statistics */
    QString deviceName;                 /* ALSA/Pulse name of device - not all device mapping to same HW are equal! Some are ~broken.." */
    QString outputDir;                  /* Where to stick audio files */
    QString containerFormat;            /* Example wav */
    qint32 tailTime;                   /* Time to keep recording after level goes low. */
    QTimer tailTimer;
    AudioLevelMeter::ThresholdState m_level = AudioLevelMeter::LOW;
    bool m_recording = false;
    bool m_active = false;

public slots:
    void activate();
    void deactivate();
    void nullRecording();
    void newRecording();
    void tailOut();
    void levelChange(AudioLevelMeter::ThresholdState level);
    void update(int read);
    void audioInputChanged(const QString name);
    void stateChanged(QMediaRecorder::State state);
    void statusChanged(QMediaRecorder::Status status);
    void availabilityChanged(bool available);
    void actualLocationChanged(QUrl url);
    void error(QMediaRecorder::Error error);

private slots:
    void quit();

signals:
    void activated();
    void deactivated();
    void recording(QString file);
    void stopped(QString file);

};

#endif // AUDIORECORDER_H
