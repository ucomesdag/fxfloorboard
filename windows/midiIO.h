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

#ifndef MIDIIO_H
#define MIDIIO_H

#include <windows.h> // Needed to acces midi and linking against winmm.lib is also needed!!!
#include <mmsystem.h>

#include <QThread>
#include <QString>
#include <QStringList>
#include <QList>

class midiIO: public QThread
{
	Q_OBJECT

public:
	midiIO();
	void run();
	void sendSysxMsg(QString sysxOutMsg, int midiOut, int midiIn);
	void sendMidi(QString midiMsg, int midiOut);
	QList<QString> getMidiOutDevices();
	QList<QString> getMidiInDevices();

signals:
	void errorSignal(QString windowTitle, QString errorMsg);
	void replyMsg(QString sysxInMsg);
	void midiFinished();
	void started();
	void finished();
	void terminated();

	void setStatusSymbol(int value);
	void setStatusProgress(int value);
    void setStatusMessage(QString message);

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
	static void emitProgress(int bytesReceived);

	static QString sysxBuffer;
	static bool dataReceive;
	static unsigned char SysXFlag;
	static int count;
	static unsigned char SysXBuffer[256];
	static int bytesTotal;

	bool multiple;
	int midiOut;
	int midiIn;
	QString sysxOutMsg;
	QString sysxInMsg;

	QString midiMsg;
	bool midi;
};

#endif // MIDIIO_H
