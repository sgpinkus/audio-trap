# Overview
Audio Trap monitors some audio input device and records for a set period of time when the sound level from the source exceeds a threshold, then stops recording a few second after the sound level from the device drops below another lower threshold. It can run in command line or GUI mode. The GUI allows you to configure setting and test sound level thresholds to get them right.

# Requirements

  + >= Qt 5.2
  + UNIX/Linux only.

# Configuration
There is a system dependent configuration file stored somewhere that backs the settings seen in GUI:

    $ cat ~/.config/audio-trap/audio-trap.conf
    [General]
    containerFormat=avi
    dampening=0.97
    deviceName="alsa:hw:CARD=PCH,DEV=0"
    highThreshold=0.3
    lowThreshold=0.2
    outputDir=/home/sam

Program is launched from command line. There is only one optional command line option `--ui` to start the GUI.

# Build and Installation

    qmake
    make
    cp audio-trap /usr/local/bin/

# TODO
See [TODO.md](docs/TODO.md).
