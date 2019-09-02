TARGET = ClinicTicketsServer
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O2
CONFIG += c++1z

win32 {
  ETCDIR = $$OUT_PWD/etc
} else {
  ETCDIR = /etc
}

ASSETSDIR = $$_PRO_FILE_PWD_/src/etc
webpack_commands = cd
win32 {
  webpack_commands += /d
}
webpack_commands += $$ASSETSDIR && npm run
debug {
  webpack_commands += dev
} else {
  webpack_commands += prod
}
webpack_commands += && $(COPY_DIR) \"$$ASSETSDIR/docroot\" \"$$ETCDIR/docroot\"
webpack_commands += && $(COPY_DIR) \"$$ASSETSDIR/templates\" \"$$ETCDIR/templates\"
webpack.target = webpack
webpack.commands = $$webpack_commands

QMAKE_EXTRA_TARGETS += webpack

unix {
  target.files += $$OUT_PWD/$(DESTDIR)/$(TARGET)
  target.files += $$ETCDIR
  target.path += /usr/bin/cts
  INSTALLS += target
}

include(src/ClinicTicketsServer.pri)

SOURCES += src/main.cpp
