# Overview
Software will be developed incrementally. There are many incremental objectives:

  + Record sound to dated files based on the presence of sound over some harcoded threshold.
  + Add learning elements.
  + Define events and push events to some event bus - probably DBus. Maybe a pipe.

# Constraints

  + Doing it in Qt.

# Qt's Multimedia Audio Modules
See qt-audio.md

# Design
Influenced by Qt's audio API obviously. Class hierarchy and responsibilities:

  AudioController
    QAudioInput
    AudioMonitor
      QIODevice
      QAudioFormat
    AudioRecorder
      QAudioDeviceInfo
      QAudioFormat

*AudioController:* Manages the QAudioInput (essentially the device) events. It also hooks everything up.

*AudioMonitor:* Is a sink for bytes coming off the AudioInput which actually come via a QIODevice which itself is just a raw byte stream. Thus it also requires a QAudioFormat.

*AudioRecorder:* Starts and stops a QAudioRecorder which simultaneously reads byte from the audio device (hopefully this is possible!!). This is needed because it not easy (AFAIK) to turn bytes read from the IODevice into encoded data on disk in .wav or something - kind of shit.

---

Update: This did not work for the predicted reason. QAudioRecorder seems to snatch the device so we can't monitor it any more. Next up we'll try "AudioProbe" and setting / unsetting a output file base on probe readings.
