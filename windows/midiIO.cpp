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

#include "midiIO.h"
#include "../SysxIO.h"
#include "../globalVariables.h"

unsigned char midiIO::SysXBuffer[256];
unsigned char midiIO::SysXFlag = 0;
int midiIO::bytesTotal = 0;
int midiIO::count = 0;
bool midiIO::dataReceive = false;
QString midiIO::sysxBuffer;

midiIO::midiIO()
{
	this->midi = false; // Set this to false;

	/* Connect signals */
	SysxIO *sysxIO = SysxIO::Instance();
	QObject::connect(this, SIGNAL(setStatusSymbol(int)),
                sysxIO, SIGNAL(setStatusSymbol(int)));
	QObject::connect(this, SIGNAL(setStatusProgress(int)),
                sysxIO, SIGNAL(setStatusProgress(int)));
	QObject::connect(this, SIGNAL(setStatusMessage(QString)),
                sysxIO, SIGNAL(setStatusMessage(QString)));
	QObject::connect(this, SIGNAL(errorSignal(QString, QString)),
                sysxIO, SLOT(errorSignal(QString, QString)));

	QObject::connect(this, SIGNAL(replyMsg(QString)),
		sysxIO, SLOT(receiveSysx(QString)));
	QObject::connect(this, SIGNAL(midiFinished()),	
			sysxIO, SLOT(finishedSending()));
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
	queryMidiOutDevices();
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
	queryMidiInDevices();
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
void midiIO::sendMsg(QString sysxOutMsg, int midiOut)
{
	HMIDIOUT    outHandle;
	MIDIHDR     midiHdr;
	UINT        err;

	emit setStatusProgress(100);

	/* Open default MIDI Out device */
	if (!(err = midiOutOpen(&outHandle, midiOut, 0, 0, CALLBACK_NULL)))
	{		
		err = 0;

		/* Convert QString to char* (hex value) */
		int msgLength = sysxOutMsg.length()/2;

		char *sysEx = new char[msgLength];
		
		char *ptr;
		ptr = &sysEx[0];
		for(int i=0;i<sysxOutMsg.length();i++)
		{	
			unsigned int n;
			QString hex = "0x";
			hex.append(sysxOutMsg.mid(i, 2));
			bool ok;
			n = hex.toInt(&ok, 16);
			*ptr = (char)n;
			i++; ptr++;
		};

		/* Store pointer in MIDIHDR */
		midiHdr.lpData = (LPSTR) &sysEx[0];

		 /* Store its size in the MIDIHDR */
		midiHdr.dwBufferLength = (UINT) msgLength;//sizeof(sysxOutMsg);

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
				QString errorMsg;
				errorMsg.append("<font size='+1'><b>");
				errorMsg.append(tr("Midi Output Error!"));
				errorMsg.append("<b></font><br>");
				errorMsg.append(getMidiOutErrorMsg(err));
				showErrorMsg(errorMsg, "out");
			};

			Sleep(sendTimeout); // Extra delay due to midiOutUnprepareHeader doesn't return MIDIERR_STILLPLAYING. 

			/* Unprepare the buffer and MIDIHDR */
			while (MIDIERR_STILLPLAYING == midiOutUnprepareHeader(outHandle, &midiHdr, sizeof(MIDIHDR))) 
			{
				//* Delay to give it time to finish */
				Sleep(0);
			};
		}
		else
		{
			QString errorMsg;
			errorMsg.append("<font size='+1'><b>");
			errorMsg.append(tr("Midi Output Error!"));
			errorMsg.append("<b></font><br>");
			errorMsg.append(getMidiOutErrorMsg(err));
			showErrorMsg(errorMsg, "out");
		};

		/* Close the MIDI device */
		midiOutClose(outHandle); /*For some reason this does the same as midiOutReset()???. 
								 Or are prepare and unprepareHeaders not doeing there job?*/
		delete[] sysEx;
		//emit setStatusProgress(100); Moved to the beginning of the method was too short to display.
	}
	else
	{
		QString errorMsg;
		errorMsg.append("<font size='+1'><b>");
		errorMsg.append(tr("Error opening default MIDI Out device!"));
		errorMsg.append("<b></font><br>");
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

	dwParam2; dwInstance; // not used;
	
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

					/* Indicate we've begun handling a particular System Exclusive message */
					SysXFlag |= 0x01;
				};

				/* Is this the last block (ie, the end of System Exclusive byte is here in the buffer)? */
				if (*(ptr + (lpMIDIHeader->dwBytesRecorded - 1)) == 0xF7)
				{
					/* Indicate we're done handling this particular System Exclusive message */
					SysXFlag &= (~0x01);
				};
				QString tmp; int bytesReceived = 0;
				emitProgress(bytesReceived);

				while((lpMIDIHeader->dwBytesRecorded--))
				{
					unsigned int n = (int)*ptr;
					QString hex = QString::number(n, 16).toUpper();
					if (hex.length() < 2) hex.prepend("0");
					sysxBuffer.append(hex);
					tmp.append(hex);
					tmp.append(" ");
					bytesReceived = sysxBuffer.size() / 2;
					emitProgress(bytesReceived);
					ptr++;
				};

				bytesReceived = sysxBuffer.size() / 2;
				emitProgress(bytesReceived);

				/* Was this the last block of System Exclusive bytes? */
				if (!SysXFlag)
				{
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

/**************************** run() **************************************
 * New QThread that processes the sysex or midi message and handles if yes 
 * or no it has to start receiving a reply on the MIDI In device midiIn. If 
 * so midiCallback() will handle the receive of the incomming sysex message.
 *************************************************************************/
void midiIO::run()
{
	if(midi) // Check if we are going to send syssex or midi data.
	{
		HMIDIOUT    outHandle;
		UINT        err;
		DWORD		midi;

		emit setStatusProgress(0); // Reset the progress bar.

		/* Open default MIDI Out device */
		if (!(err = midiOutOpen(&outHandle, midiOut, 0, 0, CALLBACK_NULL)))
		{
			err = 0;
			
			/* Convert the QString to DWORD (hex value) */
			bool ok;
			if(midiMsg.count("0x") > 1)
			{
				QStringList msgList = midiMsg.split("0x", QString::SkipEmptyParts);
				for(int i=0;i<msgList.size();++i)
				{
					QString midiMsg = msgList.at(i);
					midiMsg.prepend("0x");
					midi = (DWORD)midiMsg.toInt(&ok, 16);

					/* Output the midi command */
					midiOutShortMsg(outHandle, midi);

					/* Calculate the percentage and update the progress bar */
					int percentage = (100/(double)msgList.size()) * (double)(i + 1);
					emit setStatusProgress(percentage);
					
					/* Give it some time to process the midi 
					message before sending the next*/
					Sleep(midiSendTimeout);
				};
			}
			else  
			{
				if(!midiMsg.contains("0x"))
				{
					midiMsg.prepend("0x");
				}
				midi = (DWORD)midiMsg.toInt(&ok, 16);

				/* Output the midi command */
				midiOutShortMsg(outHandle, midi);
			};	

			/* Give it some time to finish else there is a change that 
			the device is closed before finishing the transmission */
			Sleep(midiTimeout);

			/* Close the MIDI device */
			midiOutClose(outHandle);
		}
		else
		{
			QString errorMsg;
			errorMsg.append("<font size='+1'><b>");
			errorMsg.append(tr("Error opening default MIDI Out device!"));
			errorMsg.append("<b></font><br>");
			errorMsg.append(getMidiOutErrorMsg(err));
			showErrorMsg(errorMsg, "out");
		};
		this->midi = false; // reset to false.

		emit setStatusProgress(100); // Finished so we can set the progress bar to 100%.
		emit midiFinished(); // We are finished so we send a signal to free the device.		
	}
	else
	{
		this->SysXFlag = 0;
		this->count = 0;
		this->dataReceive = false;
		this->sysxBuffer = "";
		this->sysxInMsg = "";

		/* Check if we are going to receive something on sending */
		bool receive;
		(this->sysxOutMsg.mid(12, 2) != "12")? receive = true: receive = false;

		if(receive==true)
		{
			
			/* Get the size of data bytes returned to calculate the progress percentage */
			bool ok;
			QString sizeChunk = sysxOutMsg.mid(sysxDataOffset * 2, 4 * 2);
			int dataLenght = sizeChunk.toInt(&ok, 16);
			bytesTotal = (sysxDataOffset + 2) + dataLenght;
			if(dataLenght == 0) // Id request>
			{
				bytesTotal += 2;
			}
			else if(sizeChunk == "00001F00") // Patch Request
			{
				bytesTotal = 1010;
			};
			
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
							dataReceive = true;
							sendMsg(sysxOutMsg, midiOut);
							count=0;
							if(multiple)
							{
								/* Make the loop wait a bit more for the rest
								due to the crapy midi api of windows */
								while(dataReceive || count < maxWait)
								{
									Sleep(receiveTimeout);
									count++;
								};
							}
							else
							{
								while(dataReceive && count < minWait) // count is in case we get stuck
								{
									Sleep(receiveTimeout);
									count++;
								};
							};

							/* We need to set a flag to tell our callback midiCallback()
							   not to do any more midiInAddBuffer(), because when we
							   call midiInReset() below, Windows will send a final
							   MIM_LONGDATA message to that callback. If we were to
							   allow midiCallback() to midiInAddBuffer() again, we'd
							   never get the driver to finish with our midiHdr
							*/
							SysXFlag |= 0x80;
							sysxInMsg = sysxBuffer.toUpper();
						};

						/* Stop recording */
						midiInReset(inHandle);
					};
				};

				/* If there was an error above, then print a message */
				if (err) 
				{
					QString errorMsg;
					errorMsg.append("<font size='+1'><b>");
					errorMsg.append(tr("Midi Input Error!"));
					errorMsg.append("<b></font><br>");
					errorMsg.append(getMidiInErrorMsg(err));
					showErrorMsg(errorMsg, "in");
				};
				
				/* Close the MIDI In device */
				while ((err = midiInClose(inHandle)) == MIDIERR_STILLPLAYING) 
				{
					Sleep(0);
				};

				if (err) 
				{
					QString errorMsg;
					errorMsg.append("<font size='+1'><b>");
					errorMsg.append(tr("Midi Input Error!"));
					errorMsg.append("<b></font><br>");
					errorMsg.append(getMidiInErrorMsg(err));
					showErrorMsg(errorMsg, "in");
				};
				
				/* Unprepare the buffer and MIDIHDR. Unpreparing a buffer that has not been prepared is ok */
				midiInUnprepareHeader(inHandle, &midiHdr, sizeof(MIDIHDR));
			}
			else
			{
				QString errorMsg;
				errorMsg.append("<font size='+1'><b>");
				errorMsg.append(tr("Error opening default MIDI In device!"));
				errorMsg.append("<b></font><br>");
				errorMsg.append(getMidiInErrorMsg(err));
				showErrorMsg(errorMsg, "in");
			};
		}
		else
		{
			sendMsg(sysxOutMsg, midiOut);
		};
		
		this->sysxInMsg = sysxInMsg;
		emit replyMsg(sysxInMsg);
	};
	this->exec();
};

/*********************** sendSysxMsg() ***********************************
 * Starts a new thread that handles the send and receive of sysex messages.
 *************************************************************************/
void midiIO::sendSysxMsg(QString sysxOutMsg, int midiOut, int midiIn)
{	
	this->sysxOutMsg = sysxOutMsg.simplified().toUpper().remove("0X").remove(" ");
	this->multiple = false;
	if(sysxOutMsg.size() == 34 && sysxOutMsg.mid(sysxOutMsg.size()-8, 2) != "00")
	{
		this->multiple = true;
	};
	this->midiOut = midiOut;
	this->midiIn = midiIn;

	start();
};

/*********************** showErrorMsg() **********************************
 * Formats the error message received by the midi-in or midi-out device 
 * and outputs a warning text box to the user.
 *************************************************************************/
void midiIO::showErrorMsg(QString errorMsg, QString type)
{
	QString windowTitle;
	/*if(type == "out")
	{
		windowTitle = tr("GT-8 Fx FloorBoard - Midi Output Error");
	}
	else if(type == "in")
	{
		windowTitle = tr("GT-8 Fx FloorBoard - Midi Input Error");
	};*/
	windowTitle = tr("GT-8 Fx FloorBoard");

	emit errorSignal(windowTitle, errorMsg);
};

/*********************** sendMidi() **********************************
 * Starts a new thread that handles the sending of the midi messages.
 **********************************************************************/
void midiIO::sendMidi(QString midiMsg, int midiOut)
{	
	this->midiOut = midiOut;
	this->midiMsg = midiMsg;
	this->midi = true;

	start();
};

/*********************** emitProgress() **********************************
 * This is a static function to make it it possible to update the progress 
 * bar from the CALLBACK function which is a static member.
 * It calculates the percenage and emits the signal.
 **********************************************************************/
void midiIO::emitProgress(int bytesReceived)
{
	if(bytesReceived != 0) // This is to prevent flickering of the progress bar.
	{
		int percentage;
		percentage = (100 / (double)bytesTotal) * (double)bytesReceived;

		SysxIO *sysxIO = SysxIO::Instance();
		sysxIO->emitStatusProgress(percentage);
	};
};