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

#ifndef SYSXIO_H
#define SYSXIO_H

#include <QString>
#include <QList>
#include <qdom.h>

class SysxIODestroyer;

struct SysxData
{
	QList<QString> adress;
	QList< QList<QString> > hex;
};

class SysxIO
{

public:
	static SysxIO* Instance(); //Singleton patern design

	void setFileSource(SysxData fileSource);
	void setFileSource(QByteArray data);
	void setFileName(QString fileName);
	QString getFileName();
	SysxData getFileSource();
	QList<QString> getFileSource(QString hex1, QString hex2);

protected :
	SysxIO();
	friend class SysxIODestroyer;
	virtual ~SysxIO() { };

private:
	static SysxIO* _instance;
	static SysxIODestroyer _destroyer;

	SysxData fileSource;
	QString fileName;

};

#endif // SYSXIO_H