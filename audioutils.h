#ifndef AUDIOUTILS_H
#define AUDIOUTILS_H

#include <QString>
#include <QMap>
#include <QAudioDeviceInfo>
#include <QAudioFormat>
#include <QMediaRecorder>
#include <QAudioRecorder>
#include <utility>

using namespace std;

class AudioUtils
{
public:
    explicit AudioUtils();
    static QAudioDeviceInfo getAudioDeviceInfo(QString name);
    static QString recorderInfo(const QAudioRecorder &recorder);
    static QString audioEnvInfo();
    const static QMap<QMediaRecorder::State,QString> MEDIA_STATE_STRING;
    const static QMap<QMediaRecorder::Status,QString> MEDIA_STATUS_STRING;
    const static QMap<QMediaRecorder::Status,QString> MEDIA_STATUS_MSG_STRING;
    const static QMap<QMediaRecorder::Error,QString> MEDIA_ERROR_STRING;
};

#endif // AUDIOUTILS_H
