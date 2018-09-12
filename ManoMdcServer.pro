#-------------------------------------------------
#
# Project created by QtCreator 2018-02-15T10:00:00
#
#-------------------------------------------------

QT       += core gui sql network svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ManoMdcServer
TEMPLATE = app

CONFIG += c++1z

SOURCES += main.cpp\
    requestmapper.cpp \
    controllers/modelcontroller.cpp \
    controllers/sessioncontroller.cpp \
    controllers/templatecontroller.cpp \
    routing/route.cpp \
    routing/router.cpp \
    routing/routekey.cpp \
    database.cpp \
    appsettings.cpp \
    models/model.cpp \
    gui/dlgdbconnection.cpp \
    gui/mainwindow.cpp \
    controllers/ticketcontroller.cpp \
    models/ticketmodel.cpp \
    models/ticketactionmodel.cpp \
    models/usermodel.cpp \
    controllers/usercontroller.cpp \
    gui/dlgusers.cpp \
    gui/dlgusereditor.cpp \
    controllers/API/apiticketcontroller.cpp \
    validators/requestvalidator.cpp \
    validators/rules/validationrule.cpp \
    validators/rules/idrule.cpp \
    validators/rules/windowrule.cpp \
    validators/rules/checkintrule.cpp \
    validators/rules/boolrule.cpp


HEADERS  += requestmapper.h \
    appsettings.h \
    database.h \
    controllers/modelcontroller.h \
    controllers/sessioncontroller.h \
    controllers/templatecontroller.h \
    routing/route.h \
    routing/router.h \
    routing/routekey.h \
    models/model.h \
    gui/dlgdbconnection.h \
    gui/mainwindow.h \
    controllers/ticketcontroller.h \
    models/ticketmodel.h \
    models/ticketactionmodel.h \
    models/usermodel.h \
    controllers/usercontroller.h \
    gui/dlgusers.h \
    gui/dlgusereditor.h \
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
    validators/rules/checkintrule.h \
    validators/rules/boolrule.h

win32:RC_ICONS += icon.ico

RESOURCES += \
    icons.qrc

OTHER_FILES += etc/* etc/docroot/* etc/templates/* etc/ssl/* logs/*

include(logging/logging.pri)
include(httpserver/httpserver.pri)
include(templateengine/templateengine.pri)

FORMS += \
    gui/dlgdbconnection.ui \
    gui/mainwindow.ui \
    gui/dlgusers.ui \
    gui/dlgusereditor.ui
