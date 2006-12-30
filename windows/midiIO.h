/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
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

#ifndef midiIO_H
#define midiIO_H

#include <QString>
#include <QList>
using namespace std;

class midiIO
{
	public:
		midiIO();
		~midiIO();

		void queryMidiInDevices();
		void queryMidiOutDevices();
		QList<QString> getMidiInDevices();
		QList<QString> getMidiOutDevices();
		
		/*void setMidiOutDevices();
		QList<QString> getMidiOutDevices();
		void setMidiOut(unsigned int MidiOut);
		unsigned int getMidiOut();

		void setMidiInDevices();
		QList<QString> getMidiInDevices();
		void setMidiIn(unsigned int MidiIn);
		unsigned int getMidiIn();

		void sendsysxOut(QString sysxOut);
		QString getsysxOut();
		QString getsysxIn();
		int getMsgLengthOut();
		int getMsgLengthIn();*/

	private:
		QList<QString> MidiOutDevices;
		QList<QString> MidiInDevices;
		/*QString sysxOut;
		int msgLengthOut;
		QString sysxIn;
		int msgLengthIn;*/
};

#endif // midiIO_H