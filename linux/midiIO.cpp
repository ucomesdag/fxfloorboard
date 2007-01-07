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
	this->MidiOutDevices.push_back(QString("Midi not implemented!"));
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
	this->MidiInDevices.push_back(QString("Midi not implemented!"));
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
	QString errorMsg;
	return errorMsg;
};

/************************* getMidiInErrorMsg() ***************************
 * Retrieves and displays an error message for the passed MIDI In error
 * number. It does this using midiInGetErrorText().
 *************************************************************************/
QString midiIO::getMidiInErrorMsg(unsigned long err)
{
	QString errorMsg;
	return errorMsg;
};

/*********************** sendMsg() **********************************
 * Prepares the sysx message before sending on the MIDI Out device. It 
 * converts the message from a QString to a char* and opens, sends 
 * and closes the MIDI device.
 *************************************************************************/
void midiIO::sendMsg(QString sysxMsg, int midiOut)
{

};

/*********************** sendSysxMsg() ********************************
 * Processes the sysex message and handles if yes or no it has to start 
 * receiving a reply on the MIDI In device midiIn. If so midiCallback() 
 * will handle the receive of the incomming sysex message.
 *************************************************************************/
QString midiIO::sendSysxMsg(QString sysxMsg, int midiOut, int midiIn)
{	
	QString sysxInMsg;
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