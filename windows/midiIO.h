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

#ifndef MIDIIO_H
#define MIDIIO_H

#include <windows.h> // Needed to acces midi and linking against winmm.lib is also needed!!!
#include <mmsystem.h>

#include <QObject>
#include <QString>
#include <QList>

class midiIO: public QObject
{
	Q_OBJECT

public:
	midiIO();
	~midiIO();
	
	QList<QString> getMidiOutDevices();
	QList<QString> getMidiInDevices();

	QString sendSysxMsg(QString sysxOut, int midiOut, int midiIn);

private:
	void queryMidiInDevices();
	void queryMidiOutDevices();

	QString getMidiOutErrorMsg(unsigned long err);
	QString getMidiInErrorMsg(unsigned long err);

	void showErrorMsg(QString errorMsg, QString type);
	void sendMsg(QString sysxOutMsg, int midiOut);

	QList<QString> midiOutDevices;
	QList<QString> midiInDevices;

	static void CALLBACK midiCallback(HMIDIIN handle, 
		UINT wMsg, DWORD dwInstance, 
		DWORD dwParam1, DWORD dwParam2);

	static QString sysxBuffer;
	static bool dataReceive;
	static unsigned char SysXFlag;
	static int count;
	static unsigned char SysXBuffer[256];
};

#endif // MIDIIO_H
