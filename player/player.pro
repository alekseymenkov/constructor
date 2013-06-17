#-------------------------------------------------
#
# Project created by QtCreator 2011-12-30T23:19:04
#
#-------------------------------------------------

QT       += core
QT	 += gui
QT	 += network
QT	 += xml
QT	 += svg

CONFIG += exceptions
CONFIG += warn_on


#debug {
#        DESTDIR = ../bin/exe/debug/player
#        OBJECTS_DIR = ../bin/obj/debug/player
#        RCC_DIR = ../bin/res/debug/player
#        MOC_DIR = ../bin/moc/debug/player
#}

#!debug {
#        DESTDIR = ../bin/exe/release/player
#        OBJECTS_DIR = ../bin/obj/release/player
#        RCC_DIR = ../bin/res/release/player
#        MOC_DIR = ../bin/moc/release/player
#}

TARGET = player
TEMPLATE = app


SOURCES += main.cpp\
    ui/mainwindow.cpp \
    ../constructor-player/network/client.cpp \
    ../constructor-player/ui/lineediteventfilter.cpp \
    ../constructor-player/ui/filedialog.cpp \
    ../constructor-player/ui/configdialog.cpp \
    ../constructor-player/ui/elementeditdialog.cpp \
    ../constructor-player/ui/lineeditdelegate.cpp \
    ../constructor-player/module/constants.cpp \
    ../constructor-player/module/gdiobject.cpp \
    ../constructor-player/module/figure.cpp \
    ../constructor-player/module/record.cpp \
    ../constructor-player/module/elementlogic.cpp \
    ../constructor-player/module/elementinterface.cpp \
    ../constructor-player/project/logicloader.cpp \
    ../constructor-player/project/interfaceloader.cpp \
    logic/project/controller.cpp \
    ../constructor-player/ui/graphicsview.cpp \
    ../constructor-player/ui/tooltip.cpp \
    ../constructor-player/module/parameter.cpp \
    ../constructor-player/ui/graphicsobject.cpp \
    ../constructor-player/ui/previewwidget.cpp \
    ../constructor-player/ui/colorlabel.cpp \
    ../constructor-player/ui/networkparametersdialog.cpp \
    ../constructor-player/project/scenesettingsloader.cpp \
    ../constructor-player/project/playersettingsloader.cpp \
    ../constructor-player/project/projectsettingsloader.cpp \
    logic/project/project.cpp

HEADERS  += ui/mainwindow.h \
    ../constructor-player/network/client.h \
    ../constructor-player/ui/filedialog.h \
    ../constructor-player/ui/lineediteventfilter.h \
    ../constructor-player/ui/configdialog.h \
    ../constructor-player/ui/elementeditdialog.h \
    ../constructor-player/ui/lineeditdelegate.h \
    ../constructor-player/module/constants.h \
    ../constructor-player/module/gdiobject.h \
    ../constructor-player/module/figure.h \
    ../constructor-player/module/record.h \
    ../constructor-player/module/elementlogic.h \
    ../constructor-player/module/elementinterface.h \
    ../constructor-player/project/logicloader.h \
    ../constructor-player/project/interfaceloader.h \
    logic/project/controller.h \
    ../constructor-player/ui/graphicsview.h \
    ../constructor-player/ui/tooltip.h \
    ../constructor-player/module/parameter.h \
    ../constructor-player/ui/graphicsobject.h \
    ../constructor-player/ui/previewwidget.h \
    ../constructor-player/ui/colorlabel.h \
    ../constructor-player/ui/networkparametersdialog.h \
    ../constructor-player/project/scenesettingsloader.h \
    ../constructor-player/project/playersettingsloader.h \
    ../constructor-player/project/projectsettingsloader.h \
    logic/project/project.h

RESOURCES += \
    resources.qrc

RC_FILE = resources.rc
