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

#Header files
HEADERS += ./bankTreeList.h \
    ./customButton.h \
    ./customDial.h \
    ./customLed.h \
    ./dragBar.h \
    ./floorBoard.h \
    ./floorBoardDisplay.h \
    ./floorPanelBar.h \
    ./floorPanelBarButton.h \
    ./mainWindow.h \
    ./midiTableParser.h \
    ./namedArray.h \
    ./Preferences.h \
    ./stompBox.h \
    ./XMLWriter.h \
    ./PreferencesDestroyer.h

#Source files
SOURCES += ./bankTreeList.cpp \
    ./customButton.cpp \
    ./customDial.cpp \
    ./customLed.cpp \
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
    ./stompBox.cpp \
    ./XMLWriter.cpp \
    ./PreferencesDestroyer.cpp

#Resource file(s)
RESOURCES += .\application.qrc
