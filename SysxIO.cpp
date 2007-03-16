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

#include <QApplication>
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
	this->setConnected(false);
	this->setDevice(false);
	this->setDeviceReady(true);
	this->setSyncStatus(false);
	this->setNoError(true);
	this->setBank(0);
	this->setPatch(0);
	this->setLoadedBank(0);
	this->setLoadedPatch(0);
	this->changeCount = 0;
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
		if(offset >= checksumOffset && nexthex != "F7")
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
				errorString.append(sysxBuffer.at(sysxAddressOffset) + " ");
				errorString.append(sysxBuffer.at(sysxAddressOffset + 1) + " ");
				errorString.append(sysxBuffer.at(sysxAddressOffset + 2) + " ");
				errorString.append(sysxBuffer.at(sysxAddressOffset + 3) + " - ");
				errorString.append(tr("checksum") + " (" + checksum + ") ");
				errorString.append(tr("should have been") + " (" + getCheckSum(dataSize) + ")");
				errorString.append("\n");
				errorList.append(errorString);

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
		msgBox->setInformativeText(tr("Be aware of possible inconsistencies in this patch!"));
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
	int index = hex3.toInt(&ok, 16) + sysxDataOffset;
	QString address;
	address.append(hex1);
	address.append(hex2);
	QList<QString> sysxList = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	sysxList.replace(index, hex4);

	int dataSize = 0;
	for(int i=sysxList.size() - 3; i>=checksumOffset;i--)
	{
		dataSize += sysxList.at(i).toInt(&ok, 16);
	};
	sysxList.replace(sysxList.size() - 2, getCheckSum(dataSize));

	this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxList);

	MidiTable *midiTable = MidiTable::Instance();
	QString sysxMsg = midiTable->dataChange(hex1, hex2, hex3, hex4);

	if(this->isConnected() && this->deviceReady() && this->getSyncStatus())
	{
		this->setDeviceReady(false);

		emit setStatusSymbol(2);
		//emit setStatusProgress(0);
		emit setStatusMessage("Sending");

		QObject::connect(this, SIGNAL(sysxReply(QString)),	
			this, SLOT(resetDevice(QString)));
		
		this->sendSysx(sysxMsg);
	}
	else if(this->isConnected())
	{
		this->sendSpooler.append(sysxMsg);
	};
};

void SysxIO::setFileSource(QString hex1, QString hex2, QString hex3, QString hex4, QString hex5)
{
	bool ok;
	QString address;
	address.append(hex1);
	address.append(hex2);
	QList<QString> sysxList = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	int index = hex3.toInt(&ok, 16) + sysxDataOffset;
	sysxList.replace(index, hex4);
	sysxList.replace(index + 1, hex5);

	int dataSize = 0;
	for(int i=sysxList.size() - 3; i>=checksumOffset;i--)
	{
		dataSize += sysxList.at(i).toInt(&ok, 16);
	};
	sysxList.replace(sysxList.size() - 2, getCheckSum(dataSize));

	this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxList);

	MidiTable *midiTable = MidiTable::Instance();
	QString sysxMsg = midiTable->dataChange(hex1, hex2, hex3, hex4, hex5);

	if(this->isConnected() && this->deviceReady() && this->getSyncStatus())
	{
		this->setDeviceReady(false);

		emit setStatusSymbol(2);
		//emit setStatusProgress(0);
		emit setStatusMessage("Sending");

		QObject::connect(this, SIGNAL(sysxReply(QString)),	
			this, SLOT(resetDevice(QString)));
		
		this->sendSysx(sysxMsg);
	}
	else if(this->isConnected())
	{
		this->sendSpooler.append(sysxMsg);
	};
};

void SysxIO::setFileSource(QString hex1, QString hex2, QList<QString> hexData)
{
	QString address;
	address.append(hex1);
	address.append(hex2);

	QList<QString> sysxList = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	if(hexData.size() + sysxDataOffset + 2 == sysxList.size())
	{
		bool ok;
		for(int i=0; i<hexData.size();++i)
		{
			sysxList.replace(i + sysxDataOffset, hexData.at(i));
		};

		int dataSize = 0;
		for(int i=sysxList.size() - 3; i>=checksumOffset;i--)
		{
			dataSize += sysxList.at(i).toInt(&ok, 16);
		};
		sysxList.replace(sysxList.size() - 2, getCheckSum(dataSize));

		this->fileSource.hex.replace(this->fileSource.address.indexOf(address), sysxList);

		QString sysxMsg;
		for(int i=0;i<sysxList.size();++i)
		{
			sysxMsg.append(sysxList.at(i));
		};

		if(this->isConnected() && this->deviceReady() && this->getSyncStatus())
		{
			this->setDeviceReady(false);

			emit setStatusSymbol(2);
			//emit setStatusProgress(0);
			emit setStatusMessage("Sending");

			QObject::connect(this, SIGNAL(sysxReply(QString)),	
				this, SLOT(resetDevice(QString)));
			
			this->sendSysx(sysxMsg);
		}
		else if(this->isConnected())
		{
			this->sendSpooler.append(sysxMsg);
		};
	};
};

/************************ resetDevice() ******************************
* Reset the device after sending a sysexmesage.
* And starts to processes the spooler if the device is free.
**********************************************************************/
void SysxIO::resetDevice(QString replyMsg) 
{
	if(this->sendSpooler.size() == 0)
	{
		QObject::disconnect(this, SIGNAL(sysxReply(QString)),	
			this, SLOT(resetDevice(QString)));

		this->setDeviceReady(true);	// Free the device after finishing interaction.

		emit setStatusSymbol(1);
		emit setStatusMessage("Ready");
		emit setStatusProgress(0);
	}
	else
	{
		processSpooler();
	};
};

/************************ processSpooler() ******************************
* Send message that are in the spooler due to that the device was busy.
* And eliminates multiple messages where only the valye changes.
**********************************************************************/
void SysxIO::processSpooler() 
{
	QString sysxMsg;
	for(int i=0; i<this->sendSpooler.size(); ++i)
	{
		if(i + 1 < this->sendSpooler.size())
		{
			QString currentMsg = this->sendSpooler.at(i).left(2 * 3);
			QString nextMsg = this->sendSpooler.at(i + 1).left(2 * 3);
			if(currentMsg != nextMsg)
			{
				sysxMsg.append(this->sendSpooler.at(i));
			};
		}
		else
		{
			sysxMsg.append(this->sendSpooler.at(i));
		};		
	};
	this->sendSysx(sysxMsg);
	this->sendSpooler.clear();
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
	/*bool ok;
	QString base = "80";
	int sum = dataSize % base.toInt(&ok, 16);
	if(sum!=0) sum = base.toInt(&ok, 16) - sum;
	QString checksum = QString::number(sum, 16).toUpper();
	if(checksum.length()<2) checksum.prepend("0");
	return checksum;*/
	MidiTable *midiTable = MidiTable::Instance();
	return midiTable->getCheckSum(dataSize);
};

QList<QString> SysxIO::correctSysxMsg(QList<QString> sysxMsg)
{
	QString address1 = sysxMsg.at(sysxAddressOffset + 2);
	QString address2 = sysxMsg.at(sysxAddressOffset + 3); 

	bool ok;

	MidiTable *midiTable = MidiTable::Instance();
	for(int i=sysxDataOffset;i<sysxMsg.size() - 3;i++)
	{
		if(i==sysxDataOffset + 1) i++; // is reserved memmory address on the GT-8 so we skip it.
		
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
	for(int i=sysxMsg.size() - 1; i>=checksumOffset;i--)
	{
		dataSize += sysxMsg.at(i).toInt(&ok, 16);
	};
	sysxMsg.replace(sysxMsg.size() - 1, getCheckSum(dataSize));

	return sysxMsg;
};

/***************************** isConnected() ******************************
* Connection status that's globaly accesible.
****************************************************************************/
bool SysxIO::isConnected()
{
	return this->connected;	
};

void SysxIO::setConnected(bool connected)
{
	this->connected = connected;	
};

/***************************** deviceReady() ******************************
* Midi busy or READY status that's globaly accesible. 
* To prevent multiple messages sent at once.
****************************************************************************/
bool SysxIO::deviceReady()
{
	return this->status;	
};

void SysxIO::setDeviceReady(bool status)
{
	this->status = status;	
};

/***************************** isDevice() **********************************
* Flag that hold if the current sysex data we are editing is from file or DEVICE.
****************************************************************************/
bool SysxIO::isDevice()
{
	return this->isdevice;	
};

void SysxIO::setDevice(bool isdevice)
{
	this->isdevice = isdevice;		
};

/***************************** getSyncStatus() **********************************
* Flag that hold if the sysex data we are editing is synchronized with what's 
* on the device.
****************************************************************************/
bool SysxIO::getSyncStatus()
{
	return this->syncStatus;	
};

void SysxIO::setSyncStatus(bool syncStatus)
{
	this->syncStatus = syncStatus;		
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

void SysxIO::setLoadedBank(int bank)
{
	this->loadedBank = bank;	
};

void SysxIO::setLoadedPatch(int patch)
{
	this->loadedPatch = patch;	
};

int SysxIO::getLoadedBank(){
	return this->loadedBank;	
};

int SysxIO::getLoadedPatch(){
	return this->loadedPatch;	
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
QString SysxIO::getRequestName()
{
	return this->requestName;	
};

/*********************** getPatchChangeMsg() *****************************
* Formats the midi message for bank changing (temp buffer) and returns it.
*************************************************************************/
QString SysxIO::getPatchChangeMsg(int bank, int patch)
{
	int bankOffset = ((bank - 1) * patchPerBank) + (patch - 1);
	int bankSize = 100; // Size of items that go in a bank ( != patch address wich equals 127 ).
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
	if(isConnected())
	{
		Preferences *preferences = Preferences::Instance(); bool ok;
		int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);	// Get midi out device from preferences.
		
		midiIO *midi = new midiIO();

		midi->sendMidi(midiMsg, midiOut);
	};
};

/***************************** finishedSending() *************************************
* Signals that we are finished sending a midi message, so we can go one with our life.
*************************************************************************************/
void SysxIO::finishedSending()
{
	emit isFinished();
	/*emit setStatusSymbol(1);
	emit setStatusProgress(0);
	emit setStatusMessage(tr("Ready"));*/

	//this->namePatchChange();
};

/***************************** requestPatchChange() *************************
* Send a patch change request. Result will be checked with checkPatchChange.
****************************************************************************/
void SysxIO::requestPatchChange(int bank, int patch)
{
	/*emit setStatusSymbol(2);
	emit setStatusProgress(0);
	emit setStatusMessage(tr("Sending"));*/
	
	this->bankChange = bank;
	this->patchChange = patch;

	QObject::connect(this, SIGNAL(isFinished()),	// Connect the result of the request
		this, SLOT(namePatchChange()));				// to returnPatchName function.
	
	QString midiMsg = getPatchChangeMsg(bank, patch);
	this->sendMidi(midiMsg);
};
  
/***************************** namePatchChange() *************************
* Get the name of the patch we are switching to and check it with the 
* one requested.
****************************************************************************/
void SysxIO::namePatchChange()
{	
	QObject::disconnect(this, SIGNAL(isFinished()),	
		this, SLOT(namePatchChange()));
	QObject::disconnect(SIGNAL(patchName(QString)));
	
	QObject::connect(this, SIGNAL(patchName(QString)),
		this, SLOT(checkPatchChange(QString)));		
	
	this->requestPatchName(0, 0);
};

/***************************** checkPatchChange() *************************
* Emits a signal if the patch change was confirmed else it will retry until
* the maximum retry's has been reached.
****************************************************************************/
void SysxIO::checkPatchChange(QString name)
{	
	QObject::disconnect(this, SIGNAL(patchName(QString)),
		this, SLOT(checkPatchChange(QString)));

	if(this->requestName  == name)
	{
		emit isChanged();
		this->changeCount = 0;
	}
	else
	{
		if(changeCount < maxRetry)
		{
			this->changeCount++;
			this->requestPatchChange(bankChange, patchChange);

			emit setStatusSymbol(2);
			//emit setStatusProgress(0);
			emit setStatusMessage("Sending");
		}
		else
		{
			QObject::disconnect(this, SIGNAL(isChanged()));
			
			this->changeCount = 0;
			this->setDeviceReady(true); // Free the device after finishing interaction.
			
			emit setStatusSymbol(1);
			emit setStatusMessage(tr("Ready"));	
			emit setStatusProgress(0);

			emit patchChangeFailed();

			QApplication::beep();

			/*QMessageBox *msgBox = new QMessageBox();
			msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setTextFormat(Qt::RichText);
			QString msgText;
			msgText.append("<font size='+1'><b>");
			msgText.append(tr("Error while changing banks."));
			msgText.append("<b></font><br>");
			msgText.append(tr("An incorrect patch has been loaded. Please try to load the patch again."));
			msgBox->setText(msgText);
			msgBox->setInformativeText(tr("This is a known bug, it occures when changing the bank 'LSB'.\n"
				"For an unkown reason it didn't change."));
			msgBox->setStandardButtons(QMessageBox::Ok);
			msgBox->exec();*/
		};
	};
};

/***************************** sendSysx() ***********************************
* Sends a sysex message over the midiOut device sellected in the preferences.
*****************************************************************************/
void SysxIO::sendSysx(QString sysxMsg)
{
	Preferences *preferences = Preferences::Instance();  bool ok;
	int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);	// Get midi out device from preferences.
	int midiIn = preferences->getPreferences("Midi", "MidiIn", "device").toInt(&ok, 10);	// Get midi in device from preferences.
	
	midiIO *midi = new midiIO();

	midi->sendSysxMsg(sysxMsg, midiOut, midiIn);
};

/***************************** receiveSysx() *******************************
* Receives possible replied sysex message on sendSysex.
****************************************************************************/
void SysxIO::receiveSysx(QString sysxMsg)
{
	emit sysxReply(sysxMsg);
};

/***************************** requestPatchName() ***************************
* Send a patch name request. Result will be send with the returnPatchName 
* function. 
****************************************************************************/
void SysxIO::requestPatchName(int bank, int patch)
{
	QObject::disconnect(this, SIGNAL(sysxReply(QString)),	
			this, SLOT(returnPatchName(QString)));
	
	QObject::connect(this, SIGNAL(sysxReply(QString)),	// Connect the result of the request
		this, SLOT(returnPatchName(QString)));			// to returnPatchName function.
	
	/* Patch name request. */
	MidiTable *midiTable = MidiTable::Instance();
	QString sysxMsg = midiTable->nameRequest(bank, patch);
	sendSysx(sysxMsg);
};

/***************************** returnPatchName() ***************************
* Emits a signal with the retrieved patch name.
****************************************************************************/
void SysxIO::returnPatchName(QString sysxMsg)
{
	QObject::disconnect(this, SIGNAL(sysxReply(QString)),	
			this, SLOT(returnPatchName(QString)));
	
	QString name; 
	if(sysxMsg != "")
	{		
		MidiTable *midiTable = MidiTable::Instance();
		 
		int count = 0;
		int dataStartOffset = sysxDataOffset;
		QString hex1, hex2, hex3, hex4;
		for(int i=dataStartOffset*2; i<sysxMsg.size()-(2*2);++i)
		{
			/*hex1 = sysxMsg.mid((sysxAddressOffset + 2)*2, 2);
			hex2 = sysxMsg.mid((sysxAddressOffset + 3)*2, 2);
			hex3 = QString::number(count, 16).toUpper();
			if (hex3.length() < 2) hex3.prepend("0");
			hex4 = sysxMsg.mid(i, 2);
			name.append( midiTable->getValue("Stucture", hex1, hex2, hex3, hex4) );*/

			QString hexStr = sysxMsg.mid(i, 2);
			bool ok;
			if(hexStr == "7E")
			{
				name.append((QChar)(0x2192));
			}
			else if (hexStr == "7F")
			{
				name.append((QChar)(0x2190));
			}
			else
			{
				bool ok;
				name.append( (char)(hexStr.toInt(&ok, 16)) );
			};

			i++;
		};
	};
	emit patchName(name.trimmed());
	/*emit setStatusSymbol(3);
	emit setStatusProgress(0);
	emit setStatusMessage("Receiving");*/
};

/***************************** requestPatch() ******************************
* Send a patch request. Result will be send directly with receiveSysx signal
****************************************************************************/
void SysxIO::requestPatch(int bank, int patch)
{
	/* Patch request. */
	MidiTable *midiTable = MidiTable::Instance();
	QString sysxMsg = midiTable->patchRequest(bank, patch);
	sendSysx(sysxMsg);
};

/***************************** errorSignal() ******************************
* Displays all midi related error messages.
****************************************************************************/
void SysxIO::errorSignal(QString windowTitle, QString errorMsg)
{
	if(noError())
	{
		setNoError(false);

		emit notConnectedSignal();

		QMessageBox *msgBox = new QMessageBox();
		msgBox->setWindowTitle(windowTitle);
		msgBox->setIcon(QMessageBox::Warning);
		msgBox->setTextFormat(Qt::RichText);
		msgBox->setText(errorMsg);
		msgBox->setStandardButtons(QMessageBox::Ok);
		msgBox->exec();
	};
};

/***************************** noError() ******************************
* Error flag set on midi error to prevent (double) connexion faillure 
* messages and a midi messages.
****************************************************************************/
bool SysxIO::noError()
{
	return this->noerror;	
};

void SysxIO::setNoError(bool status)
{
	this->noerror = status;
};


void SysxIO::setCurrentPatchName(QString patchName)
{
	this->currentName = patchName;	
};

QString SysxIO::getCurrentPatchName()
{
	return this->currentName;	
};

void SysxIO::emitStatusSymbol(int value)
{
	emit setStatusSymbol(value);
};

void SysxIO::emitStatusProgress(int value)
{
	emit setStatusProgress(value);
};

void SysxIO::emitStatusMessage(QString message)
{
	emit setStatusMessage(message);
};