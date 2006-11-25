#############################################################################
##
## Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
##
## This file is part of "GT-8 FX FloorBoard".
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License along
## with this program; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
##
#############################################################################

TEMPLATE = app
TARGET = GT-8FxFloorBoard
DESTDIR = ./Release
QT += xml
CONFIG += release
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/Release \
    .
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles

#Include file(s)

win32 {
	HEADERS += ./windows/midiIO.h
	SOURCES += ./windows/midiIO.cpp
	LIBS +=  c:/Progra~1/Micros~2/Lib/WinMM.Lib
	LIBS +=  ./WinMM.Lib
	message(Including Windows specifique headers and sources...)
}
unix {
	HEADERS += ./linux/midiIO.h
	SOURCES += ./linux/midiIO.cpp
	message(Including Linux specifique headers and sources...)
}
macx {
	HEADERS += ./macosx/midiIO.h
	SOURCES += ./macosx/midiIO.cpp
	message(Including Mac OS X specifique headers and sources...)
}

#Header files
HEADERS += ./bankTreeList.h \
    ./customButton.h \
    ./customDial.h \
    ./customLed.h \
    ./customSplashScreen.h \
    ./dragBar.h \
    ./floorBoard.h \
    ./floorBoardDisplay.h \
    ./floorPanelBar.h \
    ./floorPanelBarButton.h \
    ./mainWindow.h \
    ./midiTableParser.h \
    ./namedArray.h \
    ./Preferences.h \
    ./PreferencesDestroyer.h \
    ./preferencesDialog.h \
    ./preferencesPages.h \
    ./stompBox.h \
    ./XMLWriter.h

#Source files
SOURCES += ./bankTreeList.cpp \
    ./customButton.cpp \
    ./customDial.cpp \
    ./customLed.cpp \
    ./customSplashScreen.cpp \
    ./dragBar.cpp \
    ./floorBoard.cpp \
    ./floorBoardDisplay.cpp \
    ./floorPanelBar.cpp \
    ./floorPanelBarButton.cpp \
    ./main.cpp \
    ./mainWindow.cpp \
    ./midiTableParser.cpp \
    ./namedArray.cpp \
    ./Preferences.cpp \
    ./PreferencesDestroyer.cpp \
    ./preferencesDialog.cpp \
    ./preferencesPages.cpp \
    ./stompBox.cpp \
    ./XMLWriter.cpp

#Resource file(s)
RESOURCES += .\application.qrc

#Windows resource file
win32:RC_FILE = GT-8FxFloorBoard.rc
