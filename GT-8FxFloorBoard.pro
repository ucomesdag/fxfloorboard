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

TEMPLATE = app
TARGET = GT-8FxFloorBoard
DESTDIR = ./Release
QT += xml
CONFIG += release
CONFIG += static
CONFIG += embed_manifest_exe
INCLUDEPATH += ./GeneratedFiles \
    ./GeneratedFiles/Release \
    .
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles

#Platform dependent file(s)
win32 {
	exists("c:/Progra~1/Micros~2/Lib/WinMM.Lib") {		# <-- Change the path to WinMM.Lib here!
		LIBS +=  c:/Progra~1/Micros~2/Lib/WinMM.Lib		# <-- Change the path here also!
	} else {
		LIBS +=  WinMM.Lib
		message("WINMM.LIB IS REQUIRED. IF NOT INSTALLED")
		message("PLEASE DOWNLOAD AND INSTALL PLATFORM SDK FROM:")
		message("http://www.microsoft.com/downloads/details.aspx?familyid=0BAF2B35-C656-4969-ACE8-E4C0C0716ADB&displaylang=en")
		message("AFTER INSTALLATION CHANGE THE CORRECT (DOS) PATH IN THE "GT-8FxFloorBoard.pro" FILE")
	}
	HEADERS += ./windows/midiIO.h
	HEADERS += ./windows/midiIODestroyer.h
	SOURCES += ./windows/midiIO.cpp
	SOURCES += ./windows/midiIODestroyer.cpp
	INCLUDEPATH += ./windows
	message(Including Windows specifique headers and sources...)
}
linux-g++ {
	HEADERS += ./linux/midiIO.h
	SOURCES += ./linux/midiIO.cpp
	INCLUDEPATH += ./linux
	message(Including Linux specifique headers and sources...)
}
macx {
	HEADERS += ./macosx/midiIO.h
	SOURCES += ./macosx/midiIO.cpp
	INCLUDEPATH += ./macosx
	message(Including Mac OS X specifique headers and sources...)
}

#Include file(s)
include(GT-8FxFloorBoard.pri)

#Windows resource file
win32:RC_FILE = GT-8FxFloorBoard.rc
