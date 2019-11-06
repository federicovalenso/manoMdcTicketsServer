INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core sql network

SOURCES += $$PWD/controllers/ticketcountercontroller.cpp
SOURCES += $$PWD/controllers/statisticscontroller.cpp
SOURCES += $$PWD/entities/ticket.cpp
SOURCES += $$PWD/models/statisticsmodel.cpp
SOURCES += $$PWD/ticketcounter.cpp
SOURCES += $$PWD/requestmapper.cpp
SOURCES += $$PWD/controllers/modelcontroller.cpp
SOURCES += $$PWD/routing/route.cpp
SOURCES += $$PWD/routing/router.cpp
SOURCES += $$PWD/routing/routekey.cpp
SOURCES += $$PWD/database.cpp
SOURCES += $$PWD/models/model.cpp
SOURCES += $$PWD/controllers/ticketcontroller.cpp
SOURCES += $$PWD/models/ticketmodel.cpp
SOURCES += $$PWD/models/ticketactionmodel.cpp
SOURCES += $$PWD/models/usermodel.cpp
SOURCES += $$PWD/controllers/usercontroller.cpp
SOURCES += $$PWD/validators/rules/validationrule.cpp
SOURCES += $$PWD/validators/rules/idrule.cpp
SOURCES += $$PWD/validators/rules/windowrule.cpp
SOURCES += $$PWD/validators/rules/boolrule.cpp
SOURCES += $$PWD/validators/rules/emptyvaluerule.cpp
SOURCES += $$PWD/validators/rules/intrule.cpp
SOURCES += $$PWD/serversettings.cpp
SOURCES += $$PWD/databasemigrator.cpp

HEADERS += $$PWD/requestmapper.h
HEADERS += $$PWD/controllers/statisticscontroller.h
HEADERS += $$PWD/models/statisticsmodel.h
HEADERS += $$PWD/controllers/ticketcountercontroller.h
HEADERS += $$PWD/database.h
HEADERS += $$PWD/controllers/modelcontroller.h
HEADERS += $$PWD/routing/route.h
HEADERS += $$PWD/routing/router.h
HEADERS += $$PWD/routing/routekey.h
HEADERS += $$PWD/models/model.h
HEADERS += $$PWD/controllers/ticketcontroller.h
HEADERS += $$PWD/models/ticketmodel.h
HEADERS += $$PWD/models/ticketactionmodel.h
HEADERS += $$PWD/models/usermodel.h
HEADERS += $$PWD/controllers/usercontroller.h
HEADERS += $$PWD/entities/ticket.h
HEADERS += $$PWD/entities/ticketaction.h
HEADERS += $$PWD/entities/entity.h
HEADERS += $$PWD/entities/user.h
HEADERS += $$PWD/ticketcounter.h
HEADERS += $$PWD/validators/rules/validationrule.h
HEADERS += $$PWD/validators/rules/idrule.h
HEADERS += $$PWD/validators/rules/ifrule.h
HEADERS += $$PWD/validators/rules/windowrule.h
HEADERS += $$PWD/validators/rules/boolrule.h
HEADERS += $$PWD/validators/rules/emptyvaluerule.h
HEADERS += $$PWD/validators/rules/intrule.h
HEADERS += $$PWD/serversettings.h
HEADERS += $$PWD/databasemigrator.h

OTHER_FILES += $$PWD/etc/package.json
OTHER_FILES += $$PWD/etc/webpack.mix.js
OTHER_FILES += $$PWD/etc/config.example.ini
OTHER_FILES += $$PWD/etc/docroot/index.html
OTHER_FILES += $$PWD/etc/docroot/img/*
OTHER_FILES += $$PWD/etc/templates/*
OTHER_FILES += $$PWD/etc/ssl/*
OTHER_FILES += $$PWD/logs/*
OTHER_FILES += $$PWD/etc/migrations/*
OTHER_FILES += $$PWD/etc/js/*
OTHER_FILES += $$PWD/etc/js/components/*
OTHER_FILES += $$PWD/etc/css/*

include(logging/logging.pri)
include(httpserver/httpserver.pri)
include(templateengine/templateengine.pri)

RESOURCES += \
    $$PWD/migrations.qrc

DISTFILES += \
    $$PWD/rules
