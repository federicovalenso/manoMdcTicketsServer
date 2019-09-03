TARGET = ClinicTicketsServer
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O2
CONFIG += c++1z

unix {
  target.files += $$OUT_PWD/$(DESTDIR)/$(TARGET)
  target.files += $$ETCDIR
  target.path += /usr/bin/cts
  INSTALLS += target
}

include(src/ClinicTicketsServer.pri)
include(Webpack.pri)

SOURCES += src/main.cpp
