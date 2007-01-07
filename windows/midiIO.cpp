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

#include <windows.h> // Needed to acces midi and linking against winmm.lib is also needed!!!

#include <QMessageBox>
#include "midiIO.h"

midiIO::midiIO() 
{
	queryMidiOutDevices();
	queryMidiInDevices();
};

midiIO::~midiIO()
{

};

/*********************** queryMidiOutDevices() *****************************
 * Retrieves all MIDI Out devices installed on your system and stores them 
 * as a QList of QStrings and device id's. 
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
			this->midiOutDevices.append(QString::fromWCharArray(moc.szPname));
		};
	};
};

QList<QString> midiIO::getMidiOutDevices()
{
	return this->midiOutDevices;
};

/*********************** queryMidiInDevices() ******************************
 * Retrieves all MIDI In devices installed on your system and stores them 
 * as a QList of QStrings and device id's. 
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
			this->midiInDevices.append(QString::fromWCharArray(mic.szPname));
		};
	};
};

QList<QString> midiIO::getMidiInDevices()
{
	return this->midiInDevices;
};

/************************* getMidiOutErrorMsg() **************************
 * Retrieves and displays an error message for the passed MIDI Out error
 * number. It does this using midiOutGetErrorText().
 *************************************************************************/
QString midiIO::getMidiOutErrorMsg(unsigned long err)
{
	#define BUFFERSIZE 200
	WCHAR	errMsg[BUFFERSIZE];
	QString errorMsg;
	
	if (!(err = midiOutGetErrorText(err, &errMsg[0], BUFFERSIZE)))
	{
		errorMsg = QString::fromWCharArray(errMsg);
	}
	else if (err == MMSYSERR_BADERRNUM)
	{
		errorMsg = tr("Strange error number returned!");
	}
	else 
	{
		errorMsg = tr("Specified pointer is invalid!");
	};

	return errorMsg;
};

/************************* getMidiInErrorMsg() ***************************
 * Retrieves and displays an error message for the passed MIDI In error
 * number. It does this using midiInGetErrorText().
 *************************************************************************/
QString midiIO::getMidiInErrorMsg(unsigned long err)
{
	#define BUFFERSIZE 200
	WCHAR	errMsg[BUFFERSIZE];
	QString errorMsg;
	
	if (!(err = midiOutGetErrorText(err, &errMsg[0], BUFFERSIZE)))
	{
		errorMsg = QString::fromWCharArray(errMsg);
	}
	else if (err == MMSYSERR_BADERRNUM)
	{
		errorMsg = tr("Strange error number returned!");
	}
	else if (err == MMSYSERR_INVALPARAM) 
	{
		errorMsg = tr("Specified pointer is invalid!");
	}
	else 
	{
		errorMsg = tr("Unable to allocate/lock memory!");
	};

	return errorMsg;
};

/*********************** sendMsg() **********************************
 * Prepares the sysx message before sending on the MIDI Out device. It 
 * converts the message from a QString to a char* and opens, sends 
 * and closes the MIDI device.
 *************************************************************************/
void midiIO::sendMsg(QString sysxMsg, int midiOut)
{
	HMIDIOUT    handle;
	MIDIHDR     midiHdr;
	UINT        err;

	/* Open default MIDI Out device */
	if (!(err = midiOutOpen(&handle, midiOut, 0, 0, CALLBACK_NULL)))
	{		
		err = 0;

		/* Convert QString to char* (hex value) */
		int msgLength = sysxMsg.length()/2;

		char *sysEx = new char[msgLength];
		
		char *ptr;
		ptr = &sysEx[0];
		for(int i=0;i<sysxMsg.length();i++)
		{	
			unsigned int n;
			QString hex = "0x";
			hex.append(sysxMsg.mid(i, 2));
			bool ok;
			n = hex.toInt(&ok, 16);
			*ptr = (char)n;
			i++; ptr++;
		};

		/* Store pointer in MIDIHDR */
		midiHdr.lpData = (LPSTR) &sysEx[0];

		 /* Store its size in the MIDIHDR */
		midiHdr.dwBufferLength = msgLength;//sizeof(sysxMsg);

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
				showErrorMsg(getMidiOutErrorMsg(err), "out");
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
			showErrorMsg(getMidiOutErrorMsg(err), "out");
		};

		handle = NULL; //For some reason the handle doesn't get cleared with "midiOutUnprepareHeader". 

		/* Close the MIDI device */
		midiOutClose(handle);

		delete[] sysEx;
	}
	else
	{
		QString errorMsg = tr("Error opening default MIDI Out device!");
		errorMsg.append("\r\n");
		errorMsg.append(getMidiOutErrorMsg(err));
		showErrorMsg(errorMsg, "out");
	};
};

/*********************** midiCallback() **********************************
 * Processes the sysex message and handles if yes or no it has to start  
 * receiving a reply on the MIDI In device midiIn. If so it will
 * receiving the received sysex message.
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
					//printf("*************** System Exclusive ************** 0x%08X\r\n", dwParam2);
					SysXFlag |= 0x01;
				}
				if (*(ptr + (lpMIDIHeader->dwBytesRecorded - 1)) == 0xF7)
				{
					SysXFlag &= (~0x01);
				}
				while((lpMIDIHeader->dwBytesRecorded--))
				{
					//printf("%x ", *ptr);
					unsigned int n = (int)*ptr;
					QString hex = QString::number(n, 16).toUpper();
					if (hex.length() < 2) sysxBuffer.append("0");
					sysxBuffer.append(hex);
					ptr++;
				}
				if (!SysXFlag)
				{
					//printf("***********************************************\r\n");
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

/*********************** sendSysxMsg() ********************************
 * Processes the sysex message and handles if yes or no it has to start 
 * receiving a reply on the MIDI In device midiIn. If so midiCallback() 
 * will handle the receive of the incomming sysex message.
 *************************************************************************/
QString midiIO::sendSysxMsg(QString sysxMsg, int midiOut, int midiIn)
{	
	QString sysxInMsg;

	/* Check if we are going to receive something on sending */
	QString sysxOutMsg = sysxMsg.simplified().toUpper().remove("0X").remove(" ");	

	bool receive;
	(sysxOutMsg.mid(12, 2) == "11")? receive = true: receive = false;

	if(receive==true)
	{
		HMIDIIN			handle;
		MIDIHDR			midiHdr;
		unsigned long	err;

		if (!(err = midiInOpen(&handle, midiIn, (DWORD)midiCallback, 0, CALLBACK_FUNCTION)))
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
						//printf("Recording started...\r\n");
						dataReceive = true;
						sendMsg(sysxOutMsg, midiOut);
						count=0;
						while(dataReceive || count < 3)
						{
							//printf("Waiting for data.... \r\n");
							Sleep(1000);
							count++;
							if (count < 3 && dataReceive) dataReceive = false;
						};
						SysXFlag |= 0x80;
						//printf("Recording stopped!\r\n\r\n");
						sysxInMsg = sysxBuffer.toUpper();
					};
					midiInReset(handle);
				};
			};

			if (err) 
			{
				showErrorMsg(getMidiInErrorMsg(err), "in");
			};
			
			while ((err = midiInClose(handle)) == MIDIERR_STILLPLAYING) 
			{
				Sleep(0);
			};

			if (err) 
			{
				showErrorMsg(getMidiInErrorMsg(err), "in");
			};
			
			midiInUnprepareHeader(handle, &midiHdr, sizeof(MIDIHDR));
		}
		else
		{
			QString errorMsg = tr("Error opening default MIDI In device!");
			errorMsg.append("\r\n");
			errorMsg.append(getMidiInErrorMsg(err));
			showErrorMsg(errorMsg, "in");
		};
	}
	else
	{
		sendMsg(sysxOutMsg, midiOut);
		return NULL;
	};
	
	return sysxInMsg;
};

/*********************** showErrorMsg() ********************************
 * Formats the error message received by the midi-in or midi-out device 
 * and outputs a warning text box to the user.
 *************************************************************************/
void midiIO::showErrorMsg(QString errorMsg, QString type)
{
	QString windowTitle;
	if(type == "out")
	{
		windowTitle = tr("GT-8 Fx FloorBoard - Midi Output Error");
	}
	else if(type == "in")
	{
		windowTitle = tr("GT-8 Fx FloorBoard - Midi Input Error");
	};

	QMessageBox *msgBox = new QMessageBox();
	msgBox->setWindowTitle(windowTitle);
	msgBox->setIcon(QMessageBox::Warning);
	msgBox->setText(errorMsg);
	msgBox->setStandardButtons(QMessageBox::Ok);
	msgBox->exec();
};