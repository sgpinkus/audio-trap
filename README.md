# Overview
Audio Trap monitors some audio input device and records for a set period of time when the sound level from the source exceeds a threshold, then stops recording a few second after the sound level from the device drops below another lower threshold. It can run in command line or GUI mode. The GUI allows you to configure setting and test sound level thresholds to get them right.

# Requirements
Only test on Linux.

  + Qt 5.2+, with qtmultimedia5 library.
  + Working [audio sub-system](https://wiki.qt.io/Qt_5.4_Multimedia_Backends) and an audio input device.

# Configuration
There is a single configuration file that backs the settings seen in GUI. Location of the file is system dependent (Debian Linux it is `$HOME/.config/audio-trap/audio-trap.conf`):

    $ cat ~/.config/audio-trap/audio-trap.conf
    [General]
    containerFormat=avi
    dampening=0.97
    deviceName="alsa:hw:CARD=PCH,DEV=0"
    highThreshold=0.3
    lowThreshold=0.2
    outputDir=/home/sam

# Usage

    audio-trap [--ui]

Program is launched from command line via executable. There is only one optional command line option `--ui` to start the GUI which allows you to easily change the configuration and see status.

# Build and Installation

    qmake
    make
    cp audio-trap /usr/local/bin/

# TODO
See [TODO.md](docs/TODO.md).
