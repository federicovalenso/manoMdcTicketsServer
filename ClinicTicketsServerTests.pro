QT += testlib
QT -= gui

TARGET = ClinicTicketsServerTests
TEMPLATE = app

CONFIG += qt warn_on depend_includepath testcase c++1z

include(src/ClinicTicketsServer.pri)
include(tests/ClinicTicketsServerTest.pri)
include(Webpack.pri)
