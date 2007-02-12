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

/* Sysex Message Parameters */
const int sysxAddressOffset = 8;	// Offset where the address information starts in a sysx message.
const int sysxDataOffset = 12;		// Offset where the data starts in a sysx message.

/* Patches and Banks */
const int bankTotalUser = 35;		// Number of user (editable) banks.
const int bankTotalAll = 85;		// Number of total banks.
const int patchPerBank = 4;			// Number of patches in a bank.

/* Midi Send & Receive 
const int processTimeout = 100;		// Time (ms) the device needs to process a message before sending the next.
const int waitTimeout = 100;		// Time (ms) we wait between sysex messages for the next one to be received.
const int maxTimeoutCount = 4;*/	// Maximum times we loop through the receive handel before we give up the waiting.

/* Patch Sellection (Used when copying patches) */
const int sellectionBlinks = 5;		// Times we blink to indicate we have sellected a patch before returning.
const int sellectionBlinkInterval = 500;		// Interval (ms) the item blinks.

#endif // GLOBALVARIABLES_H

