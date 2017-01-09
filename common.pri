#DEFINES += TESTING
CONFIG -= debug
!CONFIG(debug):DEFINES -= QT_NO_DEBUG_OUTPUT

win32 {
  MOC_DIR = _moc
  UI_DIR =  _ui
  OBJECTS_DIR = _obj
} else {
  MOC_DIR = .moc
  UI_DIR =  .ui
  OBJECTS_DIR = .obj
}

app {
  DESTDIR = $$OUTDIR
  mac {
    CONFIG -= app_bundle
  }
  win32 {
    CONFIG += console
  }
}
