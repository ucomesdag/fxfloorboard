#############################################################################
##
## Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
##
## This file is part of "GT-8 Fx FloorBoard".
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
HEADERS += ./editPage.h \
    ./editWindow.h \
    ./globalVariables.h \
    ./mainWindow.h \
    ./bankTreeList.h \
    ./customButton.h \
    ./customDial.h \
    ./customLed.h \
    ./customSlider.h \
    ./customSplashScreen.h \
    ./customSwitch.h \
    ./dragBar.h \
    ./floorBoard.h \
    ./floorBoardDisplay.h \
    ./floorPanelBar.h \
    ./floorPanelBarButton.h \
    ./renameDialog.h \
    ./renameWidget.h \
    ./statusBarSymbol.h \
    ./statusBarWidget.h \
    ./stompBox.h \
    ./stompbox_ce.h \
    ./stompbox_cs.h \
    ./stompbox_dd.h \
    ./stompbox_dgt.h \
    ./stompbox_eq.h \
    ./stompbox_fv.h \
    ./stompbox_fx1.h \
    ./stompbox_fx2.h \
    ./stompbox_lp.h \
    ./stompbox_ns.h \
    ./stompbox_od.h \
    ./stompbox_pre.h \
    ./stompbox_rv.h \
    ./stompbox_wah.h \
    ./MidiTable.h \
    ./MidiTableDestroyer.h \
    ./SysxIO.h \
    ./SysxIODestroyer.h \
    ./sysxWriter.h \
    ./XMLWriter.h \
    ./Preferences.h \
    ./PreferencesDestroyer.h \
    ./preferencesDialog.h \
    ./preferencesPages.h \
    ./customControlKnob.h \
    ./customControlSwitch.h \
    ./customControlListMenu.h \
    ./customControlLabel.h

#Source files
SOURCES += ./editPage.cpp \
    ./editWindow.cpp \
    ./main.cpp \
    ./mainWindow.cpp \
    ./bankTreeList.cpp \
    ./customButton.cpp \
    ./customDial.cpp \
    ./customLed.cpp \
    ./customSlider.cpp \
    ./customSplashScreen.cpp \
    ./customSwitch.cpp \
    ./dragBar.cpp \
    ./floorBoard.cpp \
    ./floorBoardDisplay.cpp \
    ./floorPanelBar.cpp \
    ./floorPanelBarButton.cpp \
    ./renameDialog.cpp \
    ./renameWidget.cpp \
    ./statusBarSymbol.cpp \
    ./statusBarWidget.cpp \
    ./stompBox.cpp \
    ./stompbox_ce.cpp \
    ./stompbox_cs.cpp \
    ./stompbox_dd.cpp \
    ./stompbox_dgt.cpp \
    ./stompbox_eq.cpp \
    ./stompbox_fv.cpp \
    ./stompbox_fx1.cpp \
    ./stompbox_fx2.cpp \
    ./stompbox_lp.cpp \
    ./stompbox_ns.cpp \
    ./stompbox_od.cpp \
    ./stompbox_pre.cpp \
    ./stompbox_rv.cpp \
    ./stompbox_wah.cpp \
    ./MidiTable.cpp \
    ./MidiTableDestroyer.cpp \
    ./SysxIO.cpp \
    ./SysxIODestroyer.cpp \
    ./sysxWriter.cpp \
    ./XMLWriter.cpp \
    ./Preferences.cpp \
    ./PreferencesDestroyer.cpp \
    ./preferencesDialog.cpp \
    ./preferencesPages.cpp \
    ./customControlKnob.cpp \
    ./customControlSwitch.cpp \
    ./customControlListMenu.cpp \
    ./customControlLabel.cpp

#Resource file(s)
RESOURCES += GT-8FxFloorBoard.qrc
