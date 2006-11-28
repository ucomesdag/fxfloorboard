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

#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include "sysxWriter.h"	

sysxWriter::sysxWriter() 
{
	
};

sysxWriter::~sysxWriter()
{

};

void sysxWriter::setFile(QString fileName)
{
	this->fileName = fileName;
	this->fileSource.clear();
};

bool sysxWriter::readFile()
{	
	QFile file(fileName);
    if (file.open(QIODevice::ReadOnly))
	{
		QByteArray in = file.readAll();
		QVector<QString> sysxBuffer;
		for(int i=0;i<in.size();i++)
		{
			unsigned char byte = (char)in[i];
			unsigned int n = (int)byte;
			QString hex = QString::number(n, 16).toUpper();
			if (hex.length() < 2) hex.prepend("0");
			sysxBuffer.append(hex);

			if(hex == "F7") 
			{
				this->fileSource.append(sysxBuffer);
				sysxBuffer.clear();
			};
		}
		return true;
	}
	else
	{
		return false;
	};
};

void sysxWriter::writeFile(QString fileName)
{	
	QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
	{
		QByteArray out;
		unsigned int count=0;
		for (QVector< QVector<QString> >::iterator dev = fileSource.begin(); dev != fileSource.end(); ++dev)
		{
			QVector<QString> msg(*dev);
			for (QVector<QString>::iterator code = msg.begin(); code != msg.end(); ++code)
			{
				QString str(*code);
				bool ok;
				unsigned int n = str.toInt(&ok, 16);
				out[count] = (char)n;
				count++;
			};
		};
		file.write(out);
	};

};

QVector< QVector<QString> > sysxWriter::getFileSource()
{
	return fileSource;	
};

QString sysxWriter::getFileName()
{
	return fileName;	
};