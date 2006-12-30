/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
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

#include "SysxIO.h"
#include "SysxIODestroyer.h"

SysxIO::SysxIO() 
{
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
	
	QList<QString> sysxBuffer;
	for(int i=0;i<data.size();i++)
	{
		unsigned char byte = (char)data[i];
		unsigned int n = (int)byte;
		QString hex = QString::number(n, 16).toUpper();
		if (hex.length() < 2) hex.prepend("0");
		sysxBuffer.append(hex);

		if(hex == "F7") 
		{
			this->fileSource.address.append( sysxBuffer.at(9) + sysxBuffer.at(10) );
			this->fileSource.hex.append(sysxBuffer);
			sysxBuffer.clear();
		};
	};
};

void SysxIO::setFileSource(QString hex1, QString hex2, QString hex3, QString hexValue)
{
	bool ok;
	int dataOffset = 11;
	int index = hex3.toInt(&ok, 16) + dataOffset;
	QString address;
	address.append(hex1);
	address.append(hex2);
	QList<QString> sysxMsg = this->fileSource.hex.at(this->fileSource.address.indexOf(address));
	sysxMsg.replace(index, hexValue);
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
	QList<QString> sysxMsg = this->fileSource.hex.at( this->fileSource.address.indexOf(address) );
	return sysxMsg;
};

