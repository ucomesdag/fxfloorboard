# ############################################################################
# #
# # Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
# #
# # This file is part of "GT-8 Fx FloorBoard".
# #
# # This program is free software; you can redistribute it and/or modify
# # it under the terms of the GNU General Public License as published by
# # the Free Software Foundation; either version 2 of the License, or
# # (at your option) any later version.
# #
# # This program is distributed in the hope that it will be useful,
# # but WITHOUT ANY WARRANTY; without even the implied warranty of
# # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# # GNU General Public License for more details.
# #
# # You should have received a copy of the GNU General Public License along
# # with this program; if not, write to the Free Software Foundation, Inc.,
# # 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
# #
# ############################################################################
TEMPLATE = app
CONFIG += static
CONFIG += embed_manifest_exe
CONFIG += debug_and_release
CONFIG(debug, debug|release) {
    TARGET = "GT-8FxFloorBoard[DEBUG]"
    DESTDIR = ./debug
    OBJECTS_DIR += debug
    UI_DIR += ./generatedfiles
    MOC_DIR += ./generatedfiles/debug
    INCLUDEPATH += ./generatedfiles \
        ./generatedfiles/debug \
        .
}
else {
    TARGET = "GT-8FxFloorBoard"
    DESTDIR = ./release
    OBJECTS_DIR += release
    UI_DIR += ./generatedfiles
    MOC_DIR += ./generatedfiles/release
    INCLUDEPATH += ./generatedfiles \
        ./generatedfiles/release \
        .
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter \
        -Wno-return-type \
        -Wno-unused-value \
        -Wno-unused-variable \
        -Wno-for-loop-analysis \
        -Wno-logical-op-parentheses \
        -Wno-self-assign-field \
        -Wno-logical-op-parentheses
}
DEPENDPATH += .
QT += xml \
    widgets

# Platform dependent file(s)
win32 {
    # You can use DIR /X to find the 8.3 DOS name.
    # If after linking with correct path below with: LIBS += /path/to/your/lib/YourLibrary.lib
    # you still get a lot of "undefined reference to ..." error's try the following syntax instead:
    # LIBS += -L/path/to/your/lib -lYourLibrary
    # exists("C:/PROGRA~1/MI2578~1/WINDOWS/V6.0/LIB/WINMM.LIB"){  # <-- Change the path to WinMM.Lib here!
    # 	LIBS += C:/PROGRA~1/MICROS~4/WINDOWS/V6.0/LIB/WINMM.LIB   # <-- Change the path here also!
    # }
        exists("C:/PROGRA~1/MI2578~1/WINDOWS/V6.0/LIB/WINMM.LIB"):LIBS += -LC:/PROGRA~1/MI2578~1/WINDOWS/V6.0/LIB -lWINMM
    else {
        LIBS += .\WinMM.Lib
        message("WINMM.LIB IS REQUIRED. IF NOT INSTALLED THEN")
        message("PLEASE DOWNLOAD AND INSTALL THE LATEST PLATFORM SDK")
        message("FROM MICROSOFT.COM AND AFTER INSTALLATION")
        message("CHANGE THE CORRECT (DOS) PATH TO WinMM.lib")
        message("IN THIS (GT-8FxFloorBoard.pro) FILE WHERE INDICATED")
    }
    HEADERS += ./windows/midiIO.h
    SOURCES += ./windows/midiIO.cpp
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

# Artisic Style configuration file
RESOURCES += options.astylerc

# Uncrustify configuration file
RESOURCES += uncrustify.cfg

# Include file(s)
include(GT-8FxFloorBoard.pri)

# Windows resource file
win32:RC_FILE = GT-8FxFloorBoard.rc

DISTFILES += \
  options.astylerc

