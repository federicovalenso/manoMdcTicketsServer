#-------------------------------------------------
#
# Project created by QtCreator 2018-02-15T10:00:00
#
#-------------------------------------------------

QT += core sql network

CONFIG(gui_mode) {
message(GUI mode)
QT += gui svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    gui/dlgdbconnection.cpp \
    gui/mainwindow.cpp \
    gui/dlgusers.cpp \
    gui/dlgusereditor.cpp

HEADERS  +=  \
    gui/dlgdbconnection.h \
    gui/mainwindow.h \
    gui/dlgusers.h \
    gui/dlgusereditor.h \

win32:RC_ICONS += icon.ico

RESOURCES += \
    icons.qrc

FORMS += \
    gui/dlgdbconnection.ui \
    gui/mainwindow.ui \
    gui/dlgusers.ui \
    gui/dlgusereditor.ui
} else {
message(non-GUI mode)
QT -= gui
}

TARGET = ClinicTicketsServer
TEMPLATE = app

QMAKE_CXXFLAGS_RELEASE += -O2
CONFIG += c++1z

SOURCES += main.cpp\
    requestmapper.cpp \
    controllers/modelcontroller.cpp \
    routing/route.cpp \
    routing/router.cpp \
    routing/routekey.cpp \
    database.cpp \
    models/model.cpp \
    controllers/ticketcontroller.cpp \
    models/ticketmodel.cpp \
    models/ticketactionmodel.cpp \
    models/usermodel.cpp \
    controllers/usercontroller.cpp \
    controllers/API/apiticketcontroller.cpp \
    validators/requestvalidator.cpp \
    validators/rules/validationrule.cpp \
    validators/rules/idrule.cpp \
    validators/rules/windowrule.cpp \
    validators/rules/boolrule.cpp \
    validators/rules/emptyvaluerule.cpp \
    validators/rules/intrule.cpp \
    serversettings.cpp

HEADERS  += requestmapper.h \
    database.h \
    controllers/modelcontroller.h \
    routing/route.h \
    routing/router.h \
    routing/routekey.h \
    models/model.h \
    controllers/ticketcontroller.h \
    models/ticketmodel.h \
    models/ticketactionmodel.h \
    models/usermodel.h \
    controllers/usercontroller.h \
    entities/ticket.h \
    entities/ticketaction.h \
    entities/entity.h \
    entities/user.h \
    controllers/API/apiticketcontroller.h \
    validators/requestvalidator.h \
    validators/rules/validationrule.h \
    validators/rules/idrule.h \
    validators/rules/ifrule.h \
    validators/rules/windowrule.h \
    validators/rules/boolrule.h \
    validators/rules/emptyvaluerule.h \
    validators/rules/intrule.h \
    serversettings.h

OTHER_FILES += etc/* etc/docroot/* etc/templates/* etc/ssl/* logs/*

include(logging/logging.pri)
include(httpserver/httpserver.pri)
include(templateengine/templateengine.pri)
