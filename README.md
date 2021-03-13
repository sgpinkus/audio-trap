# Audio Trap
Simply monitors some audio input device and records for a set period of time when the sound level from the source exceeds a threshold, then stops recording a few second after the sound level drops below another, lower threshold. It can run in command line or GUI mode. The GUI allows you to configure settings and test sound level thresholds to get them right.

# Requirements

  + Qt 5.2+, with qtmultimedia5 library.
  + Working [audio sub-system](https://wiki.qt.io/Qt_5.4_Multimedia_Backends) and an audio input device.
  + Only tested on Linux.
  
# Usage

    audio-trap [--ui]

There is only one optional command line option `--ui` to start the GUI which allows you to easily change/tune the configuration and see output status. Once tuned can run the recorder in the background or what ever. Each recording will be dumped to `<outputDir>/audio-<datetime>.<ext>` indefinitely.

# Configuration
There is a single configuration file that backs the settings seen in GUI. Location of the file is system dependent (on Debian Linux it is `$HOME/.config/audio-trap/audio-trap.conf`):

    $ cat ~/.config/audio-trap/audio-trap.conf
    [General]
    containerFormat=avi
    dampening=0.97
    deviceName="alsa:hw:CARD=PCH,DEV=0"
    highThreshold=0.3
    lowThreshold=0.2
    outputDir=/home/sam

# Build & Installation
Install Qt 5 development packages and qtmultimedia5 then:

    qmake
    make
    cp audio-trap /usr/local/bin/

Binary executable build is stashed in this git repo so can just use that also without the build.

# TODO
See [TODO.md](docs/TODO.md).
