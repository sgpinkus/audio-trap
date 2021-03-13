#include "audioutils.h"
#include <utility>
#include <QTextStream>
#include <QAudioRecorder>
#include <QUrl>

using namespace std;

const QMap<QMediaRecorder::State,QString> AudioUtils::MEDIA_STATE_STRING {
   pair<QMediaRecorder::State,QString>(QMediaRecorder::StoppedState,"Stopped"),
   pair<QMediaRecorder::State,QString>(QMediaRecorder::RecordingState, "Recording"),
   pair<QMediaRecorder::State,QString>(QMediaRecorder::PausedState, "Paused")
};

const QMap<QMediaRecorder::Status,QString> AudioUtils::MEDIA_STATUS_STRING {
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::UnavailableStatus,"UnavailableStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::UnloadedStatus, "UnloadedStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::LoadingStatus, "LoadingStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::LoadedStatus, "LoadedStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::StartingStatus, "StartingStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::RecordingStatus, "RecordingStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::PausedStatus, "PausedStatus"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::FinalizingStatus, "FinalizingStatus")
};

const QMap<QMediaRecorder::Status,QString> AudioUtils::MEDIA_STATUS_MSG_STRING {
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::UnavailableStatus,"The recorder is not available or not supported by connected media object"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::UnloadedStatus, "The recorder is avilable but not loaded"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::LoadingStatus, "The recorder is initializing"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::LoadedStatus, "The recorder is initialized and ready to record media"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::StartingStatus, "Recording is requested but not active yet"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::RecordingStatus, "Recording is active"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::PausedStatus, "Recording is paused"),
   pair<QMediaRecorder::Status,QString>(QMediaRecorder::FinalizingStatus, "Recording is stopped with media being finalized")

};

const QMap<QMediaRecorder::Error,QString> AudioUtils::MEDIA_ERROR_STRING {
    pair<QMediaRecorder::Error,QString>(QMediaRecorder::ResourceError, "ResourceError"),
    pair<QMediaRecorder::Error,QString>(QMediaRecorder::FormatError, "FormatError"),
    pair<QMediaRecorder::Error,QString>(QMediaRecorder::OutOfSpaceError, "OutOfSpaceError")
};

/**
 * Get QAudioDeviceInfo from string name - no such function in shitty QAudo libs..
 * QAudioRecorder uses a different device naming scheme. Seems to have optional "alsa:" prefix and ":" suffix.
 */
QAudioDeviceInfo AudioUtils::getAudioDeviceInfo(QString name) {
    QAudioDeviceInfo device;
    QList<QAudioDeviceInfo> devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    for(int i = 0; i < devices.size(); ++i) {
        if(devices.at(i).deviceName() == name
        || devices.at(i).deviceName() + ":" == name
        || "alsa:" + devices.at(i).deviceName() == name) {
            device = devices.at(i);
            break;
        }
    }
    return device;
}

QString AudioUtils::recorderInfo(const QAudioRecorder &recorder)
{
    QString info;
    QTextStream stream(&info);
    stream << "CURRENT INPUT: " << recorder.audioInput() << "\n";
    stream << "VOLUME:        " << recorder.volume() << "\n";
    stream << "MUTED:         " << recorder.isMuted() << "\n";
    stream << "STATE:         " << MEDIA_STATE_STRING[recorder.state()] << "\n";
    stream << "STATUS:        " << MEDIA_STATUS_STRING[recorder.status()] << "\n";
    stream << "OUTPUT LOC:    " << recorder.outputLocation().toString() << "\n";
    stream << "ACTUAL LOC:    " << recorder.actualLocation().toString() << "\n";
    stream << "AVAILABILITY:  " << recorder.availability() << "\n";
    return info;
}

QString AudioUtils::audioEnvInfo() {
    QAudioRecorder recorder;
    QString info;
    QTextStream stream(&info);
    stream << "DEFAULT INPUT:          " << recorder.defaultAudioInput() << "\n";
    stream << "CURRENT INPUT:          " << recorder.audioInput() << "\n";
    stream << "VOLUME:                 " << recorder.volume() << "\n";
    stream << "MUTED:                  " << recorder.isMuted() << "\n";
    stream << "INPUTS (QAudioRecorder):\n";
    foreach(const QString &input, recorder.audioInputs()) {
        stream << "\t" << input << "\n";
    }
    stream << "SUPPORTED CODECS:\n";
    foreach (const QString &name, recorder.supportedAudioCodecs()) {
        stream << "\t" << name << "\n";
    }
    stream << "SUPPORTED CONTAINERS:\n";
    foreach (const QString &name, recorder.supportedContainers()) {
        stream << "\t" << name << "\n";
    }
    stream << "INPUTS (QAudioInput):\n";
    for(QAudioDeviceInfo & device: QAudioDeviceInfo::availableDevices(QAudio::AudioInput)) {
        // 8000Hz, 16bit, channelCount=2, sampleType=SignedInt, byteOrder=LittleEndian, codec="audio/pcm"
        QAudioFormat fmt = device.preferredFormat();
        stream << "\t" << device.deviceName();
        stream << " (";
        stream << fmt.sampleRate() << "Hz, ";
        stream << fmt.sampleSize() << "bit, ";
        stream << "channelCount="  << fmt.channelCount() << ", ";
        stream << "sampleType="    << fmt.sampleType() << ", ";
        stream << "byteOrder="     << fmt.byteOrder() << ", ";
        stream << "codec="         << fmt.codec();
        stream << ")\n";
    }
    return info;
}
