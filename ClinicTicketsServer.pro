include(src/ClinicTicketsServer.pri)
include(Webpack.pri)

TARGET = ClinicTicketsServer
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O2
CONFIG -= debug_and_release debug_and_release_target
CONFIG += c++1z

target.files += $$ETCDIR
target.path += $$DESTDIR
INSTALLS += target

SOURCES += src/main.cpp
