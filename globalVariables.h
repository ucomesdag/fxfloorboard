/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8 Fx FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

/****************************************************************************
* These are general settings keep in mind that number of fx and fx setting 
* are per item setting to change them you will need to modify the following 
* files:
*     -> floorBoard.cpp
*     -> stompBox.cpp
*     -> stompbox_{fx abbrv.}.cpp
*
****************************************************************************/

/* General Parameters */
const QString idRequestString = "F07E000601F7";		// Indentity Request (GT-8).
const QString idReplyPatern = "F07E000602";			// Returned device id message must contain/match this (QRegExp or a string without spaces and all caps).
const int buttonBlinkInterval = 250;				// The interval (ms) the led on buttons blink.

/* Sysex Message Parameters */
const int sysxAddressOffset = 7;	// Offset (starts at 0) where the address information starts in a sysx message.
const int sysxDataOffset = 11;		// Offset (starts at 0) where the data starts in a sysx message.
const int checksumOffset = 7;		// Offset (starts at 0) where we start calculating the checksum (Normally this is the address offset).

/* Patches and Banks */
const int bankTotalUser = 35;		// Number of user (editable) banks.
const int bankTotalAll = 85;		// Number of total banks.
const int patchPerBank = 4;			// Number of patches in a bank.

/* Midi Send & Receive */
const int midiSendTimeout = 25;	    // Time (ms) the device needs to process a MIDI message before sending the next.
const int midiTimeout = 250;		// Time (ms) the device needs to process a MIDI message before closing the device.
const int sendTimeout = 25;			// Time (ms) the device needs to process a SYSEX message before sending the next.
const int receiveTimeout = 100;		// Time (ms) we wait between SYSEX messages for the next one to be received.
const int maxWait = 4;				// Maximum times we loop through the receive handel before we give up the waiting.
const int minWait = 2;				// Minimum times we loop through the receive handel before we give up the waiting.
const int maxRetry = 10;			// Maximum times we retry to load a patch in case of a transfer error .

/* Patch Sellection (Used when copying patches) */
const int sellectionBlinks = 5;				// Times we blink to indicate we have sellected a patch before returning.
const int sellectionBlinkInterval = 500;	// Interval (ms) the item blinks.

#endif // GLOBALVARIABLES_H
