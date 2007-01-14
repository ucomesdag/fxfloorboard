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

unsigned char midiIO::SysXBuffer[256];
unsigned char midiIO::SysXFlag = 0;
int midiIO::count = 0;
bool midiIO::dataReceive = false;
QString midiIO::sysxBuffer = 0;

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

	this->dataReceive = false;
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
	HMIDIOUT    outHandle;
	MIDIHDR     midiHdr;
	UINT        err;

	/* Open default MIDI Out device */
	if (!(err = midiOutOpen(&outHandle, midiOut, 0, 0, CALLBACK_NULL)))
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
		midiHdr.dwBufferLength = (UINT) msgLength;//sizeof(sysxMsg);

		/* Flags must be set to 0 */
		midiHdr.dwFlags = 0;

		/* Prepare the buffer and MIDIHDR */
        err = midiOutPrepareHeader(outHandle, &midiHdr, sizeof(MIDIHDR));
	    if (!err)
		{
			/* Output the SysEx message */
			err = midiOutLongMsg(outHandle, &midiHdr, sizeof(MIDIHDR));
			if (err) 
			{	
				showErrorMsg(getMidiOutErrorMsg(err), "out");
			};

			Sleep(10); // Extra delay due to midiOutUnprepareHeader doesn't return MIDIERR_STILLPLAYING. 

			/* Unprepare the buffer and MIDIHDR */
			while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(outHandle, &midiHdr, sizeof(MIDIHDR))) 
			{
				//* Delay to give it time to finish */
				Sleep(100);
			};
			
		}
		else
		{
			showErrorMsg(getMidiOutErrorMsg(err), "out");
		};

		/* Close the MIDI device */
		midiOutClose(outHandle); /*For some reason this does the same as midiOutReset()???. 
								 Or are prepare and unprepareHeaders not doeing there job?*/

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
void CALLBACK midiIO::midiCallback(HMIDIIN handle, UINT wMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	LPMIDIHDR		lpMIDIHeader;
	unsigned char *	ptr;

	/* Determine why Windows called me */
	switch (wMsg)
	{
		/* Received all or part of some System Exclusive message */	
		case MIM_LONGDATA:
		{
			/* If this application is ready to close down, then don't midiInAddBuffer() again */
			if (!(SysXFlag & 0x80))
			{
				/*	Assign address of MIDIHDR to a LPMIDIHDR variable. Makes it easier to access the
					field that contains the pointer to our block of MIDI events */
				lpMIDIHeader = (LPMIDIHDR)dwParam1;

				/* Get address of the MIDI event that caused this call */
				ptr = (unsigned char *)(lpMIDIHeader->lpData);

				/* Is this the first block of System Exclusive bytes? */
				if (!SysXFlag)
				{
					dataReceive = true;
					count = 0;

					/* Print out a noticeable heading as well as the timestamp of the first block.
						(But note that other, subsequent blocks will have their own time stamps). */
					//printf("*************** System Exclusive ************** 0x%08X\r\n", dwParam2);

					/* Indicate we've begun handling a particular System Exclusive message */
					SysXFlag |= 0x01;
				};

				/* Is this the last block (ie, the end of System Exclusive byte is here in the buffer)? */
				if (*(ptr + (lpMIDIHeader->dwBytesRecorded - 1)) == 0xF7)
				{
					/* Indicate we're done handling this particular System Exclusive message */
					SysXFlag &= (~0x01);
				};
				while((lpMIDIHeader->dwBytesRecorded--))
				{
					//printf("%x ", *ptr);
					unsigned int n = (int)*ptr;
					QString hex = QString::number(n, 16).toUpper();
					if (hex.length() < 2) sysxBuffer.append("0");
					sysxBuffer.append(hex);
					ptr++;
				}

				/* Was this the last block of System Exclusive bytes? */
				if (!SysXFlag)
				{
					/* Print out a noticeable ending */
					//printf("***********************************************\r\n");
					dataReceive = false;
				};

				/* Queue the MIDIHDR for more input */
				midiInAddBuffer(handle, lpMIDIHeader, sizeof(MIDIHDR));
			};
			break;
		};
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
    this->SysXFlag = 0;
    this->count = 0;
    this->dataReceive = false;
    this->sysxBuffer = "";
	QString sysxInMsg = 0;

	/* Check if we are going to receive something on sending */
	QString sysxOutMsg = sysxMsg.simplified().toUpper().remove("0X").remove(" ");	

	bool receive;
	(sysxOutMsg.mid(12, 2) != "12")? receive = true: receive = false;

	if(receive==true)
	{
		HMIDIIN			inHandle;
		MIDIHDR			midiHdr;
		unsigned long	err;

		/* Open default MIDI In device */
		if (!(err = midiInOpen(&inHandle, midiIn, (DWORD)midiCallback, 0, CALLBACK_FUNCTION)))
		{
			/* Store pointer to our input buffer for System Exclusive messages in MIDIHDR */
			midiHdr.lpData = (LPSTR)&SysXBuffer[0];

			/* Store its size in the MIDIHDR */
			midiHdr.dwBufferLength = sizeof(SysXBuffer);

			/* Flags must be set to 0 */
			midiHdr.dwFlags = 0;
			
			/* Prepare the buffer and MIDIHDR */
			err = midiInPrepareHeader(inHandle, &midiHdr, sizeof(MIDIHDR));
			if (!err)
			{
				/* Queue MIDI input buffer */
				err = midiInAddBuffer(inHandle, &midiHdr, sizeof(MIDIHDR));
				if (!err)
				{
					/* Start recording Midi */
					err = midiInStart(inHandle);
					if (!err)
					{
						//printf("Recording started...\r\n");
						this->dataReceive = true;
						sendMsg(sysxOutMsg, midiOut);
						this->count=0;
						while(this->dataReceive && this->count < 5)
						{
							//printf("Waiting for data.... \r\n");
							Sleep(150); //1000
							this->count++;
							//if (this->count > 5 && this->dataReceive) this->dataReceive = false;
						};

						/* We need to set a flag to tell our callback midiCallback()
						   not to do any more midiInAddBuffer(), because when we
						   call midiInReset() below, Windows will send a final
						   MIM_LONGDATA message to that callback. If we were to
						   allow midiCallback() to midiInAddBuffer() again, we'd
						   never get the driver to finish with our midiHdr
						*/
						this->SysXFlag |= 0x80;
						//printf("Recording stopped!\r\n\r\n");
						sysxInMsg = this->sysxBuffer.toUpper();
					};

					/* Stop recording */
					midiInReset(inHandle);
				};
			};

			/* If there was an error above, then print a message */
			if (err) 
			{
				showErrorMsg(getMidiInErrorMsg(err), "in");
			};
			
			/* Close the MIDI In device */
			while ((err = midiInClose(inHandle)) == MIDIERR_STILLPLAYING) 
			{
				Sleep(0);
			};

			if (err) 
			{
				showErrorMsg(getMidiInErrorMsg(err), "in");
			};
			
			/* Unprepare the buffer and MIDIHDR. Unpreparing a buffer that has not been prepared is ok */
			midiInUnprepareHeader(inHandle, &midiHdr, sizeof(MIDIHDR));
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