# Overview
"Qt Multimedia offers a range of audio classes, covering both low and high level approaches to audio input, output and processing" -- https://doc.qt.io/qt-5/audiooverview.html.

**QMediaPlayer**
The high level and easiest way to play sound and video. Use with https://doc.qt.io/qt-5/qvideowidget.html. You don't have to set an audio device to send to. That's all automatic. Supports playlists. https://doc.qt.io/qt-5/qmediaplayer.html:

    QMediaPlayer * player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile("/usr/share/sounds/alsa/Noise.wav"));
    player->setVolume(50);
    player->play();

Operative state machine methods are:

    setMedia(), play(), pause(), stop()

You set the media to play with `void QMediaPlayer::setMedia(const QMediaContent &media, QIODevice *stream = NULL)`. `QMediaContent` is a media container. It can contain one or more `QMediaResource` objects. QMediaResource *must* have a URL but that is it. The other data is like a union of `QAudioFormat` and `QVideoFormat` plus mime and media meta data. Example has these for audio and the equiv for video, plus `mimeType()`, `language()` etc:

    void 	setAudioBitRate(int rate)
    void 	setAudioCodec(const QString &codec)
    void 	setChannelCount(int channels)
    void 	setSampleRate(int sampleRate)

Alternatively if you provide a `QIODevice` and QMediaPlayer will stream content from the (opened and readable), QIODevice and use `QMediaContent` as information describing the data coming out of the QIODevice.

Note `QMediaContent` is stupid: It can represent one or collection of QMediaResource. I don't see why it needs to exist. Anyway it's just a dumb container.

**QMediaObject**
`QAudioDecoder`, `QCamera`, `QMediaPlayer`, `QRadioTuner` all inherit QMediaObject for some reason. It's hard to see what these objects really have common. One section (needs some segregation really ) of the QMediaObject interface is:

    bind(), unbind(), and isAvailable()

It seems media object have a relationship with a single (?) other consumer object, and these methods manage that.

**QAudioDecoder**
As opposed to QMediaPlayer, audio is read from disk and provided to the client via a `QAudioBuffer`. Docs say its "a high class for decoding local audio media files" but it will also read data from a stream.

**QAudioRecorder**
The high level interface to audio recording. It allows you to go directly from some device to a file:

    QAudioRecorder *audioRecorder = new QAudioRecorder;
    QAudioEncoderSettings audioSettings;
    audioSettings.setCodec("audio/FLAC");
    audioSettings.setQuality(QMultimedia::HighQuality);
    audioRecorder->setEncodingSettings(audioSettings);
    audioRecorder->setOutputLocation(QUrl::fromLocalFile("/tmp/test.flac"));
    audioRecorder->record();

QAudioRecorder requires:

  1. An audio input device name.
  2. A settings object.

Unclear whether is will use reasonable defaults.

**QAudioEncoderSettings** All QAudioRecorder settings are passed via QAudioRecorder::setEncodingSettings(QAudioEncoderSettings settings, ...) or setAudioSettings(QAudioEncoderSettings settings). The latter does not allow you to set a "container format" (example AVI):

    void	setBitRate(int rate)
    void	setChannelCount(int channels)
    void	setCodec(const QString & codec)
    void	setEncodingMode(QMultimedia::EncodingMode mode)
    void	setEncodingOption(const QString & option, const QVariant & value)
    void	setEncodingOptions(const QVariantMap & options)
    void	setQuality(QMultimedia::EncodingQuality quality)
    void	setSampleRate(int rate)

QAudioRecorder extends QMediaRecorder and ~all the functionality is in the base class. QAudioRecorder only provides ways to set and enumerate the audio devices. QMediaRecorder provides the necessary methods to enumerate supported codecs, rates etc:

    QStringList 	supportedAudioCodecs() const
    QList<int> 	  supportedAudioSampleRates(const QAudioEncoderSettings &settings = QAudioEncoderSettings(), bool *continuous = Q_NULLPTR) const
    QStringList 	supportedContainers() const

The workflow model of QAudio|MediaRecorder is:

    record,stop,pause

Can't actually access the bytes so not useful for me ... Grrr.

**QAudioProbe**
Can be used to listen in on any QMediaRecorder, or QMediaObject and receive buffers (roughly) as they are pushed into the given object. But it *only works when data is being recorded or generated*.

    audioProbe = new QAudioProbe(this);
    audioProbe->setSource(audioRecorder
    connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer), ...)

Also see https://doc.qt.io/qt-5/qaudiobuffer.html.

**QAudioInput**
QAudio(Input|Output) have two modes, push and pull (They should have called in pipe or auto mode ...
):

  Output+Pull: You pass it a QIODevice and it pulls data from it automatically when data is ready.
  Output+Push: You have to push data to it.
  Input+Pull: "for pull mode, when audio data is available then the data will be written directly to the QIODevice."
  Input+Push: ???

Construction:

    QAudioInput(const QAudioFormat &format = QAudioFormat(), QObject *parent = Q_NULLPTR)
	  QAudioInput(const QAudioDeviceInfo &audioDevice, const QAudioFormat &format = QAudioFormat(), QObject *parent = Q_NULLPTR)

If you do't specify a QAudioDeviceInfo or QAudioFormat you get the defaults. The default input audio device is retreivable via `QAudioDeviceInfo QAudioDeviceInfo::defaultInputDevice()`. Weirdly QAudioDeviceInfo is used to represent devices - "used by Qt to construct classes that communicate with the device--such as QAudioInput, and QAudioOutput." - https://doc.qt.io/qt-5/qaudiodeviceinfo.html#details. The default is retreivable via `QAudioInput::format()`.

**QAudioFormat** is similar to QAudioEncoderSettings but different. Former is configuring an source, the latter is setting what the output should look like:

    void 	setByteOrder(QAudioFormat::Endian byteOrder)
      // QAudioFormat::Big|LittleEndian
    void 	setChannelCount(int channels)
      // An integer. QList<int> QAudioDeviceInfo::supportedChannelCounts()
    void 	setCodec(const QString &codec)
      // All platform and plugin implementations should provide support for "audio/pcm" - Linear PCM.
      // QStringList QAudioDeviceInfo::supportedCodecs()
    void 	setSampleRate(int samplerate)
      // Hz. QList<int> QAudioDeviceInfo::supportedSampleRates()
    void 	setSampleSize(int sampleSize)
      // Size in *bits*. QList<int> QAudioDeviceInfo::supportedSampleSizes().
    void 	setSampleType(QAudioFormat::SampleType sampleType)
      // QAudioFormat::Unknown|SignedInt|UnSignedInt|Float.
      // QList<QAudioFormat::SampleType> QAudioDeviceInfo::supportedSampleTypes()

A given format may not be supported by a given device. Qt (stupidly) reps devices by `QAudioDeviceInfo`; you use `bool QAudioDeviceInfo::isFormatSupported(const QAudioFormat &settings)` to test if a given device supports the given.

  + At any point in time, QAudioInput will be in one of four states: active, suspended, stopped, or idle. These states are specified by the QAudio::State enum.
  + If an error should occur ... the QAudioInput will enter the StoppedState. You can fetch its reason with error().
  + Starting up the QAudioInput is simply a matter of calling start() with a QIODevice opened for writing.

There is actually two ways to start - corresponding to "pull"/"push" modes. Pull mode is:

    IODevice * device = audioInput->start();
    connect(device, SIGNAL(readyRead()), SLOT(read()));

"Push" mode is:

    MyIODevice x = new MyIODevice(...);
    audioInput->start(x);

# Links

  + https://doc.qt.io/qt-5/qtmultimedia-index.html
  + https://doc.qt.io/qt-5/qaudiodeviceinfo.html#defaultInputDevice
  + https://doc.qt.io/qt-5/qaudioinput.html#details
  + https://doc.qt.io/qt-5/qtdatavisualization-audiolevels-example.html
  + https://doc.qt.io/qt-5/qtmultimedia-multimedia-audiorecorder-audiorecorder-cpp.html

# Critisisms

  + Audio API's are generally complicated. For some reason it seems like audio API designers insist on exposing you to all the fine details of the sound device they are an abstraction over. Case in point QAudioFormat. You need know what the "Sample Type" is (float, [un]signed int), and what the Byte Order of the input your reading is. Taking care of Byte order is *your* job API.. And why would I want floats or even unsigned ints?
  + Who came up with the  "push" "pull" analogy? It is not a good one for input devices. The QAudioInput pushes data to a IODevice. *Period*. You provide it or Qt provides one for you... That is not push vs pull.
  + Why isn't Qt using a stream paradigm like ~Java. I want QStream<AudioSample> =  new QStream<AudioSample>. Instead I have to demarshall ...

# Notes

         switch(recorder.status()) {
            case QMediaRecorder::UnavailableStatus: {
                break;
            }
            case QMediaRecorder::UnloadedStatus: {
                break;
            }
            case QMediaRecorder::LoadingStatus: {
                break;
            }
            case QMediaRecorder::LoadedStatus: {
                break;
            }
            case QMediaRecorder::StartingStatus: {
                break;
            }
            case QMediaRecorder::RecordingStatus: {
                break;
            }
            case QMediaRecorder::PausedStatus: {
                break;
            }
            case QMediaRecorder::FinalizingStatus: {
                break;
            }
        }
