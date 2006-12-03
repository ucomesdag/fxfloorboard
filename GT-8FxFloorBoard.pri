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
    ./namedArray.h \
    ./Preferences.h \
    ./PreferencesDestroyer.h \
    ./preferencesDialog.h \
    ./preferencesPages.h \
    ./stompBox.h \
    ./MidiTable.h \
    ./sysxWriter.h \
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
    ./namedArray.cpp \
    ./Preferences.cpp \
    ./PreferencesDestroyer.cpp \
    ./preferencesDialog.cpp \
    ./preferencesPages.cpp \
    ./stompBox.cpp \
    ./MidiTable.cpp \
    ./sysxWriter.cpp \
    ./XMLWriter.cpp

#Resource file(s)
RESOURCES += .\application.qrc