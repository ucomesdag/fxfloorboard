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

};

/*********************** queryMidiOutDevices() ****************************
 * Retrieves all MIDI Out devices installed on your system and stores them 
 * as a QVector of QStrings and device id's. 
 *************************************************************************/
void midiIO::queryMidiOutDevices()
{

};

QVector<QString> midiIO::getMidiInDevices()
{
	return this->MidiInDevices;
};

QVector<QString> midiIO::getMidiOutDevices()
{
	return this->MidiOutDevices;
};