QT	+= core
QT	+= gui
QT	+= network
QT	+= xml
QT	+= sql


TARGET = server
TEMPLATE = app

CONFIG += warn_on


#debug {
#        DESTDIR = ../bin/exe/debug/server
#        OBJECTS_DIR = ../bin/obj/debug/server
#        RCC_DIR = ../bin/res/debug/server
#        MOC_DIR = ../bin/moc/debug/server
#}

#!debug {
#        DESTDIR = ../bin/exe/release/server
#        OBJECTS_DIR = ../bin/obj/release/server
#        RCC_DIR = ../bin/res/release/server
#        MOC_DIR = ../bin/moc/release/server
#}


SOURCES +=  logic/server.cpp \
	    logic/mmf.cpp \
	    ui/mainwindow.cpp \
    main.cpp \
    ui/configdialog.cpp \
    ui/lineediteventfilter.cpp \
    logic/xmlsettings.cpp \
    logic/database.cpp

HEADERS  += logic/server.h \
	    logic/mmf.h \
	    ui/mainwindow.h \
    ui/configdialog.h \
    ui/lineediteventfilter.h \
    logic/xmlsettings.h \
    logic/database.h

RC_FILE = resources.rc

RESOURCES += \
    resources.qrc
