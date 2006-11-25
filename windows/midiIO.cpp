/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8 FX FloorBoard".
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

#include <string>
#include <windows.h> // Needed to acces midi and linking to winmm.lib also needed!!!
#include "midiIO.h"

midiIO::midiIO()
{
	queryMidiInDevices();
	queryMidiOutDevices();
};

/*********************** queryMidiOutDevices() ****************************
 * Retrieves all MIDI In devices installed on your system and stores them 
 * as a QVector of QStrings and device id's. 
 *************************************************************************/
void midiIO::queryMidiInDevices()
{
	#define BUFFER_SIZE	100
	MIDIINCAPS mic;

	int iNumDevs = midiInGetNumDevs();
	for (int i = 0; i < iNumDevs; i++)
	{
		if (!midiInGetDevCaps(i, &mic, sizeof(MIDIINCAPS)))
		{
			/* Convert WCHAR to QString */
			size_t converted;
			char *pMBBuffer = new char[BUFFER_SIZE];
			wcstombs_s(&converted, pMBBuffer, BUFFER_SIZE, mic.szPname, BUFFER_SIZE);
			this->MidiInDevices.push_back(QString::fromStdString(string(pMBBuffer)));
			delete[] pMBBuffer;
		};
	};
};

/*********************** queryMidiOutDevices() ****************************
 * Retrieves all MIDI Out devices installed on your system and stores them 
 * as a QVector of QStrings and device id's. 
 *************************************************************************/
void midiIO::queryMidiOutDevices()
{
	#define BUFFER_SIZE	100
	MIDIOUTCAPS moc;

	int iNumDevs = midiOutGetNumDevs();
	for (int i = 0; i < iNumDevs; i++)
	{
		if (!midiOutGetDevCaps(i, &moc, sizeof(MIDIOUTCAPS)))
		{
			/* Convert WCHAR to QString */
			size_t converted;
			char *pMBBuffer = new char[BUFFER_SIZE];
			wcstombs_s(&converted, pMBBuffer, BUFFER_SIZE, moc.szPname, BUFFER_SIZE);
			this->MidiOutDevices.push_back(QString::fromStdString(string(pMBBuffer)));
			delete[] pMBBuffer;
		};
	};
};

QVector<QString> midiIO::getMidiInDevices()
{
	return this->MidiInDevices;
};

QVector<QString> midiIO::getMidiOutDevices()
{
	return this->MidiOutDevices;
};