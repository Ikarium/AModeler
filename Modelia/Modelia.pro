# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = Modelia
DESTDIR = ./Build/Win32/Debug
QT += core widgets gui
CONFIG += debug
DEFINES += WIN64 QT_DLL QT_WIDGETS_LIB
INCLUDEPATH += $(ProjectDir)/Sources \
    ../../../../Boost/boost_1_55_0 \
    ../../../Karma/Sources \
    ./Build/temp/Win32/Debug/Moc \
    ./Build/temp/Win32/Debug/Uic \
    ./Build/temp/Win32/Debug/Rcc
DEPENDPATH += .
MOC_DIR += ./Build/temp/win32/debug/Moc
OBJECTS_DIR += debug
UI_DIR += ./Build/temp/$(PlatformName)/$(ConfigurationName)/Uic
RCC_DIR += ./Build/temp/$(PlatformName)/$(ConfigurationName)/Rcc
include(Modelia.pri)

