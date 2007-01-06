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

#include <windows.h> // Needed to acces midi and linking to winmm.lib also needed!!!
#include "midiIO.h"

midiIO::midiIO()
{
	queryMidiInDevices();
	queryMidiOutDevices();
};

/*********************** queryMidiOutDevices() ****************************
 * Retrieves all MIDI In devices installed on your system and stores them 
 * as a QList of QStrings and device id's. 
 *************************************************************************/
void midiIO::queryMidiInDevices()
{
	MIDIINCAPS mic;

	int iNumDevs = midiInGetNumDevs();
	for (int i = 0; i < iNumDevs; i++)
	{
		if (!midiInGetDevCaps(i, &mic, sizeof(MIDIINCAPS)))
		{
			/* Convert WCHAR to QString */
			this->MidiInDevices.append(QString::fromWCharArray(mic.szPname));
		};
	};
};

/*********************** queryMidiOutDevices() ****************************
 * Retrieves all MIDI Out devices installed on your system and stores them 
 * as a QList of QStrings and device id's. 
 *************************************************************************/
void midiIO::queryMidiOutDevices()
{
	MIDIOUTCAPS moc;

	int iNumDevs = midiOutGetNumDevs();
	for (int i = 0; i < iNumDevs; i++)
	{
		if (!midiOutGetDevCaps(i, &moc, sizeof(MIDIOUTCAPS)))
		{
			/* Convert WCHAR to QString */
			this->MidiOutDevices.append(QString::fromWCharArray(moc.szPname));
		};
	};
};

QList<QString> midiIO::getMidiInDevices()
{
	return this->MidiInDevices;
};

QList<QString> midiIO::getMidiOutDevices()
{
	return this->MidiOutDevices;
};

/*********************** PrintMidiOutErrorMsg() **************************
 * Retrieves and displays an error message for the passed MIDI Out error
 * number. It does this using midiOutGetErrorText().
 *************************************************************************/
void PrintMidiOutErrorMsg(unsigned long err)
{
	#define BUFFERSIZE 200
	WCHAR	errMsg[BUFFERSIZE];
	
	if (!(err = midiOutGetErrorText(err, &errMsg[0], BUFFERSIZE))) printf("%s\r\n", &errMsg[0]);
	else if (err == MMSYSERR_BADERRNUM) printf("Strange error number returned!\r\n");
	else printf("Specified pointer is invalid!\r\n");
};

/*********************** sendSysxMsg() **********************************
 * Opens, closes and sends the sysex message to the MIDI Out device that
 * is set with setMidiOut();
 *************************************************************************/
void midiIO::sendSysxMsg(QString sysxMsg, int MidiOut)
{
	HMIDIOUT    handle;
	MIDIHDR     midiHdr;
	UINT        err;

	/* Open default MIDI Out device */
	if (!(err = midiOutOpen(&handle, MidiOut, 0, 0, CALLBACK_NULL)))
	{		
		err = 0;

		/* Convert QString to char* (hex value) */
		QString hexString = sysxMsg.simplified().toUpper().remove("0X").remove(" ");	
		int msgLength = hexString.length()/2;

		char *sysEx = new char[msgLength];
		
		char *ptr;
		ptr = &sysEx[0];
		bool receive = false;
		for(int i=0;i<hexString.length();i++)
		{	
			unsigned int n;
			QString hex = "0x";
			hex.append(hexString.mid(i, 2));
			bool ok;
			n = hex.toInt(&ok, 16);
			if(i==12 && n==17) receive=true;
			*ptr = (char)n;
			i++; ptr++;
		};

		/* Store pointer in MIDIHDR */
		midiHdr.lpData = (LPSTR) &sysEx[0];

		 /* Store its size in the MIDIHDR */
		midiHdr.dwBufferLength = msgLength;//sizeof(sysEx);

		/* Flags must be set to 0 */
		midiHdr.dwFlags = 0;

		/* Prepare the buffer and MIDIHDR */
        err = midiOutPrepareHeader(handle,  &midiHdr, sizeof(MIDIHDR));
	    if (!err)
		{
			/* Output the SysEx message */
			err = midiOutLongMsg(handle, &midiHdr, sizeof(MIDIHDR));
			if (err) 
			{	
				PrintMidiOutErrorMsg(err);
			};

			/* Unprepare the buffer and MIDIHDR */
			while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(handle, &midiHdr, sizeof(MIDIHDR))) 
			{
				//* Delay to give it time to finish */
				Sleep(1000);
			};
		}
		else
		{
			PrintMidiOutErrorMsg(err);
		};

		handle = NULL; //For some reason the handle doesn't get cleared with "midiOutUnprepareHeader". 

		/* Close the MIDI device */
		midiOutClose(handle);
	}
	else
	{
		printf("Error opening default MIDI Out device!\r\n");
		PrintMidiOutErrorMsg(err);
	};
};

/*********************** midiCallback() **********************************
 * Processes the sysex message and handles if yes or no it has to start  
 * receiving a reply on the with MidiIn device. If so it will start the 
 * reception of the incoming sysex message.
 *************************************************************************/
unsigned char SysXBuffer[256];
unsigned char SysXFlag = 0;
unsigned int count = 0;
bool dataReceive = false;
QString sysxBuffer;
void CALLBACK midiCallback(HMIDIIN handle, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	LPMIDIHDR		lpMIDIHeader;
	unsigned char *	ptr;
	unsigned char 	bytes;

	switch (wMsg)
	{
		case MIM_LONGDATA:
		{
			if (!(SysXFlag & 0x80))
			{
				lpMIDIHeader = (LPMIDIHDR)dwParam1;
				ptr = (unsigned char *)(lpMIDIHeader->lpData);
				if (!SysXFlag)
				{
					dataReceive = true;
					count = 0;
					printf("*************** System Exclusive ************** 0x%08X\r\n", dwParam2);
					SysXFlag |= 0x01;
				}
				if (*(ptr + (lpMIDIHeader->dwBytesRecorded - 1)) == 0xF7)
				{
					SysXFlag &= (~0x01);
				}
				while((lpMIDIHeader->dwBytesRecorded--))
				{
					printf("%x ", *ptr);
					unsigned int n = (int)*ptr;
					QString hex = QString::number(n, 16).toUpper();
					if (hex.length() < 2) sysxBuffer.append("0");
					sysxBuffer.append(hex);
					ptr++;
				}
				if (!SysXFlag)
				{
					printf("***********************************************\r\n");
					dataReceive = false;
				}
				midiInAddBuffer(handle, lpMIDIHeader, sizeof(MIDIHDR));
			}
			break;
		}
		/* Process these messages if you desire 
		case MIM_DATA:
		case MIM_OPEN:
        case MIM_CLOSE:
        case MIM_ERROR:
        case MIM_LONGERROR:
        case MIM_MOREDATA:*/
  }
};

/*********************** PrintMidiInErrorMsg() ***************************
 * Retrieves and displays an error message for the passed MIDI In error
 * number. It does this using midiInGetErrorText().
 *************************************************************************/
void PrintMidiInErrorMsg(unsigned long err)
{
	WCHAR	errMsg[BUFFERSIZE];
	
	if (!(err = midiInGetErrorText(err, &errMsg[0], BUFFERSIZE))) printf("%s\r\n", &errMsg[0]);
	else if (err == MMSYSERR_BADERRNUM) printf("Strange error number returned!\r\n");
	else if (err == MMSYSERR_INVALPARAM) printf("Specified pointer is invalid!\r\n");
	else printf("Unable to allocate/lock memory!\r\n");
};

/*********************** receiveSysxMsg() ********************************
 * Processes the sysex message and handles when it has to start  
 * receiving a reply on the MidiIn device. If so midiCallback() will handle 
 * the reception of the incomming sysex message.
 *************************************************************************/
QString midiIO::receiveSysxMsg(QString sysxMsg, int MidiOut, int MidiIn)
{	
	unsigned char SysXBuffer[256];
	unsigned char SysXFlag = 0;
	unsigned int count = 0;
	bool dataReceive = false;
	QString sysxBuffer;
	
	HMIDIIN			handle;
	MIDIHDR			midiHdr;
	unsigned long	err;

	if (!(err = midiInOpen(&handle, MidiIn, (DWORD)midiCallback, 0, CALLBACK_FUNCTION)))
	{
		midiHdr.lpData = (LPSTR)&SysXBuffer[0];
		midiHdr.dwBufferLength = sizeof(SysXBuffer);
		midiHdr.dwFlags = 0;
		
		err = midiInPrepareHeader(handle, &midiHdr, sizeof(MIDIHDR));
		if (!err)
		{
			err = midiInAddBuffer(handle, &midiHdr, sizeof(MIDIHDR));
			if (!err)
			{
				err = midiInStart(handle);
				if (!err)
				{
					printf("Recording started...\r\n");
					dataReceive = true;
					sendSysxMsg(sysxMsg, MidiOut);
					count=0;
					while(dataReceive || count < 3)
					{
						printf("Waiting for data.... \r\n");
						Sleep(1000);
						count++;
						if (count < 3 && dataReceive) dataReceive = false;
					}
					SysXFlag |= 0x80;
					printf("Recording stopped!\r\n\r\n");
					QString sysxIn = sysxBuffer;
					return sysxIn;
				}
				midiInReset(handle);
			}
		}
		if (err) PrintMidiInErrorMsg(err);
		while ((err = midiInClose(handle)) == MIDIERR_STILLPLAYING) Sleep(0);
		if (err) PrintMidiInErrorMsg(err);
		midiInUnprepareHeader(handle, &midiHdr, sizeof(MIDIHDR));
	}
	else
	{
		printf("Error opening the default MIDI In Device!\r\n");
		PrintMidiInErrorMsg(err);
	};
};