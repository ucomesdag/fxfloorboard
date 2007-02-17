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

#include <QMessageBox>
#include "SysxIO.h"
#include "SysxIODestroyer.h"
#include "midiIO.h"
#include "Preferences.h"
#include "sysxWriter.h"
#include "MidiTable.h"
#include "globalVariables.h"

SysxIO::SysxIO() 
{
	setConnected(false);
	setDevice(false);
	setDeviceStatus(true);
	setSyncStatus(false);
	setBank(0);
	setPatch(0);
};

SysxIO* SysxIO::_instance = 0;// initialize pointer
SysxIODestroyer SysxIO::_destroyer;

SysxIO* SysxIO::Instance() 
{
	/* Multi-threading safe */
	if (!_instance /*_instance == 0*/)  // is it the first call?
	{  
		_instance = new SysxIO; // create sole instance
		_destroyer.SetSysxIO(_instance);
	};
	return _instance; // address of sole instance

	/* Single-threading */
	/*
	static SysxIO inst;
	return &inst;
	*/
};

void SysxIO::setFileSource(SysxData fileSource)
{
	this->fileSource = fileSource;
};

void SysxIO::setFileSource(QByteArray data)
{
	this->fileSource.address.clear();
	this->fileSource.hex.clear();
	
	QString errorList;
	QList<QString> sysxBuffer; 
	int dataSize = 0; int offset = 0;
	for(int i=0;i<data.size();i++)
	{
		unsigned char byte = (char)data[i];
		unsigned int n = (int)byte;
		QString hex = QString::number(n, 16).toUpper();
		if (hex.length() < 2) hex.prepend("0");
		sysxBuffer.append(hex);

		unsigned char nextbyte = (char)data[i+1];
		unsigned int nextn = (int)nextbyte;
		QString nexthex = QString::number(nextn, 16).toUpper();
		if (nexthex.length() < 2) nexthex.prepend("0");
		if(offset > 6 && nexthex != "F7")
		{		
			dataSize += n;
		};
		if(nexthex == "F7")
		{		
			QString checksum = hex;

			if(getCheckSum(dataSize) != checksum)
			{
				QString errorString;
				errorString.append(tr("Address") + ": ");
				errorString.append(sysxBuffer.at(7) + " ");
				errorString.append(sysxBuffer.at(8) + " ");
				errorString.append(sysxBuffer.at(9) + " ");
				errorString.append(sysxBuffer.at(10) + " - ");
				errorString.append(tr("checksum") + " (" + checksum + ") ");
				errorString.append(tr("should have been") + " (" + getCheckSum(dataSize) + ")");
				errorString.append("\n");
				errorList.append(errorString);

				int dataSize1 = dataSize;

				sysxBuffer = correctSysxMsg(sysxBuffer);
			};
		};
		offset++;

		if(hex == "F7") 
		{	
			this->fileSource.address.append( sysxBuffer.at(sysxAddressOffset + 2) + sysxBuffer.at(sysxAddressOffset +3) );
			this->fileSource.hex.append(sysxBuffer);
			sysxBuffer.clear();
			dataSize = 0;
			offset = 0;
		};
	};
	if(!errorList.isEmpty())
	{
		QMessageBox *msgBox = new QMessageBox();
		//msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard - Checksum Error"));
		msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
		msgBox->setIcon(QMessageBox::Warning);
		msgBox->setTextFormat(Qt::RichText);
		QString msgText;
		msgText.append("<font size='+1'><b>");
		msgText.append(tr("The file opened contains one or more incorrect checksums."));
		msgText.append("<b></font><br>");
		msgText.append(tr("The incorrect values have been corrected where possible.\n" 
			"If correction was inpossible then some settings might have been reset to zero."));
		msgBox->setText(msgText);
		msgBox->setInformativeText(tr("Please verify the patch settings for consistency."));
		msgBox->setDetailedText(errorList);
		msgBox->setStandardButtons(QMessageBox::Ok);
		msgBox->exec();
	};
};

void SysxIO::setFileSource(QString data)
{
	this->fileSource.address.clear();
	this->fileSource.hex.clear();
	
	QList<QString> sysxBuffer;
	for(int i=0;i<data.size();i++)
	{
		QString hex = data.mid(i, 2);
		sysxBuffer.append(hex);
		i++;

		if(hex == "F7") 
		{	
			this->fileSource.address.append( sysxBuffer.at(sysxAddressOffset + 2) + sysxBuffer.at(sysxAddressOffset + 3) );
			this->fileSource.hex.append(sysxBuffer);
			sysxBuffer.clear();
		};
	};
};

void SysxIO::setFileSource(QString hex1, QString hex2, QString hex3, QString hex4)
{
	bool ok;
	int dataOffset = sysxDataOffset;
	int index = hex3.toInt(&ok, 16) + dataOffset;
	QString address;
	address.append(hex1);
	address.append(hex2);
	QList<QString> sysxMsg = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	sysxMsg.replace(index, hex4);

	int dataSize = 0;
	for(int i=sysxMsg.size() - 3; i>6;i--)
	{
		dataSize += sysxMsg.at(i).toInt(&ok, 16);
	};
	sysxMsg.replace(sysxMsg.size() - 2, getCheckSum(dataSize));

	this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxMsg);
};

void SysxIO::setFileSource(QString hex1, QString hex2, QString hex3, QString hex4, QString hex5)
{
	bool ok;
	int dataOffset = sysxDataOffset;
	QString address;
	address.append(hex1);
	address.append(hex2);
	QList<QString> sysxMsg = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	int index = hex3.toInt(&ok, 16) + dataOffset;
	sysxMsg.replace(index, hex4);
	sysxMsg.replace(index + 1, hex5);

	int dataSize = 0;
	for(int i=sysxMsg.size() - 2; i>6;i--)
	{
		dataSize += sysxMsg.at(i).toInt(&ok, 16);
	};
	sysxMsg.replace(sysxMsg.size() - 2, getCheckSum(dataSize));

	this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxMsg);
};

void SysxIO::setFileName(QString fileName)
{
	this->fileName = fileName;	
};

QString SysxIO::getFileName()
{
	return this->fileName;	
};

SysxData SysxIO::getFileSource()
{
	return this->fileSource;
};

QList<QString> SysxIO::getFileSource(QString hex1, QString hex2)
{
	QString address;
	address.append(hex1);
	address.append(hex2);
	if(this->fileSource.address.indexOf(address) == -1)
	{
		sysxWriter file;
		file.setFile(":default.syx");  // Read the default sysex file so whe don't start empty handed.
		if(file.readFile())
		{	
			setFileSource(file.getFileSource());
		};
	};
	QList<QString> sysxMsg = this->fileSource.hex.at( this->fileSource.address.indexOf(address) );
	return sysxMsg;
};

QString SysxIO::getCheckSum(int dataSize)
{
	bool ok;
	QString base = "80";
	int sum = dataSize % base.toInt(&ok, 16);
	if(sum!=0) sum = base.toInt(&ok, 16) - sum;
	QString checksum = QString::number(sum, 16).toUpper();
	if(checksum.length()<2) checksum.prepend("0");
	return checksum;
};

QList<QString> SysxIO::correctSysxMsg(QList<QString> sysxMsg)
{
	QString address1 = sysxMsg.at(sysxAddressOffset + 2);
	QString address2 = sysxMsg.at(sysxAddressOffset + 3); 

	bool ok;

	MidiTable *midiTable = MidiTable::Instance();
	for(int i=sysxDataOffset;i<sysxMsg.size() - 3;i++)
	{
		if(i==12) i++; // is reserved memmory address on the GT-8 so we skip it.
		
		QString address3 = QString::number(i - sysxDataOffset, 16).toUpper();
		if(address3.length()<2) address3.prepend("0");
		
		int range = midiTable->getRange("Stucture", address1, address2, address3);

		if(midiTable->isData("Stucture", address1, address2, address3))
		{	
			int maxRange = QString("7F").toInt(&ok, 16) + 1; // index starts at 0 -> 0-127 = 128 entry's.
			int value1 = sysxMsg.at(i).toInt(&ok, 16);
			int value2 = sysxMsg.at(i + 1).toInt(&ok, 16);
			int value = (value1 * maxRange) + value2;
			
			if(value > range)
			{
				value = 0;//(int)(range / 2);
				int dif = (int)(value/maxRange);
				QString valueHex1 = QString::number(dif, 16).toUpper();
				if(valueHex1.length() < 2) valueHex1.prepend("0");
				QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
				if(valueHex2.length() < 2) valueHex2.prepend("0");
				
				sysxMsg.replace(i, valueHex1);
				sysxMsg.replace(i + 1, valueHex2);
			};

			i++;
		}
		else
		{
			if(sysxMsg.at(i).toInt(&ok, 16) > range)
			{
				int value = 0;//(int)(range / 2);
				QString valueHex = QString::number(value, 16).toUpper();
				if(valueHex.length() < 2) valueHex.prepend("0");
				sysxMsg.replace(i, valueHex);
			};
		};
	};
	
	int dataSize = 0;
	for(int i=sysxMsg.size() - 1; i>=sysxAddressOffset;i--)
	{
		dataSize += sysxMsg.at(i).toInt(&ok, 16);
	};
	sysxMsg.replace(sysxMsg.size() - 1, getCheckSum(dataSize));

	return sysxMsg;
};

void SysxIO::setConnected(bool connected)
{
	this->connected = connected;	
};

bool SysxIO::isConnected()
{
	return this->connected;	
};

void SysxIO::setDeviceStatus(bool deviceready)
{
	this->deviceready = deviceready;	
};

bool SysxIO::getDeviceStatus()
{
	return this->deviceready;	
};

void SysxIO::setDevice(bool isdevice)
{
	this->isdevice = isdevice;		
};

bool SysxIO::isDevice()
{
	return this->isdevice;	
};

void SysxIO::setSyncStatus(bool syncStatus)
{
	this->syncStatus = syncStatus;		
};

bool SysxIO::getSyncStatus()
{
	return this->syncStatus;	
};

void SysxIO::setBank(int bank)
{
	this->bank = bank;	
};

void SysxIO::setPatch(int patch)
{
	this->patch = patch;	
};

int SysxIO::getBank(){
	return this->bank;	
};

int SysxIO::getPatch(){
	return this->patch;	
};

/*********************** getRequestName() ***********************************
* Set the name for check of the patch that we are going to load.
***************************************************************************/
void SysxIO::setRequestName(QString requestName)
{
	this->requestName = requestName;	
};

/*********************** getRequestName() ***********************************
* Return the name for check of the patch that should have been loaded.
***************************************************************************/
QString SysxIO::getRequestName(){
	return this->requestName;	
};

/*********************** getPatchChangeMsg() *****************************
* Formats the midi message for bank changing (temp buffer) and returns it.
*************************************************************************/
QString SysxIO::getPatchChangeMsg(int bank, int patch)
{
	int bankOffset = ((bank - 1) * patchPerBank) + (patch - 1);
	int bankSize = 100;
	int bankMsbNum = (int)(bankOffset / bankSize);
	int programChangeNum = bankOffset - (bankSize * bankMsbNum);
	QString bankMsb = QString::number(bankMsbNum, 16);
	QString programChange = QString::number(programChangeNum, 16);
	
	if (bankMsb.length() < 2) bankMsb.prepend("0");
	if (programChange.length() < 2) programChange.prepend("0");

	QString midiMsg;
	midiMsg.append("0x00"+bankMsb+"00B0");			// MSB		
	midiMsg.append("0x000010B0");					// LSB -> not used!
	midiMsg.append("0x0000"+programChange+"C0");	// Program Controle

	return midiMsg;
};

/***************************** sendMidi() ***********************************
* Sends a midi message over the midiOut device sellected in the preferences.
****************************************************************************/
void SysxIO::sendMidi(QString midiMsg)
{
	Preferences *preferences = Preferences::Instance(); bool ok;
	int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);	// Get midi out device from preferences.
	
	midiIO *midi = new midiIO();

	QObject::connect(midi, SIGNAL(replyMsg(QString)),	// Connect the result of the request
		this, SLOT(receiveSysex(QString)));				// to finishedSending function.

	midi->sendMidi(midiMsg, midiOut);
};

/***************************** finishedSending() *************************************
* Signals that we are finished sending a midi message, so we can go one with our life.
*************************************************************************************/
void SysxIO::finishedSending()
{
	emit isFinished();
};

/***************************** sendSysex() ***********************************
* Sends a sysex message over the midiOut device sellected in the preferences.
****************************************************************************/
void SysxIO::sendSysx(QString sysxMsg)
{
	if(getDeviceStatus() == true && isConnected())
	{
		setDeviceStatus(false);

		Preferences *preferences = Preferences::Instance();  bool ok;
		int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);	// Get midi out device from preferences.
		int midiIn = preferences->getPreferences("Midi", "MidiIn", "device").toInt(&ok, 10);	// Get midi in device from preferences.
		
		midiIO *midi = new midiIO();

		QObject::connect(midi, SIGNAL(replyMsg(QString)),	// Connect the result of the request
			this, SLOT(receiveSysex(QString)));				// to receiveSysex function.

		midi->sendSysxMsg(sysxMsg, midiOut, midiIn);
	};
};

/***************************** receiveSysex() *******************************
* Receives possible replied sysex message on sendSysex.
****************************************************************************/
void SysxIO::receiveSysex(QString sysxMsg)
{
	setDeviceStatus(true);
	emit sysxReply(sysxMsg);
};

/***************************** requestPatchName() ***************************
* Send a patch name request. Result will be send with the returnPatchName 
* function. 
****************************************************************************/
void SysxIO::requestPatchName(int bank, int patch)
{
	//emit sysxReply(sysxMsg);
};

/***************************** returnPatchName() ***************************
* Emits a signal with the retrieved patch name.
****************************************************************************/
void SysxIO::returnPatchName(int bank, int patch)
{
	//emit sysexReply(sysxMsg);
};

/***************************** requestPatchChange() *************************
* Send a patch change request. Result will be send with the checked with 
* confirmPatchChange function.
****************************************************************************/
void SysxIO::requestPatchChange(int bank, int patch)
{
	//emit sysexReply(sysxMsg);
};

/***************************** confirmPatchChange() ***************************
* Emits a signal if the patch change was confirmed alese it will retry until
* the maximum retry's has been reached.
****************************************************************************/
void SysxIO::confirmPatchChange(int bank, int patch)
{
	//emit sysexReply(sysxMsg);
};
