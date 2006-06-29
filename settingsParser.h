/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "FX Floor Board".
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

#ifndef SETTINGSPARSER_H
#define SETTINGSPARSER_H

#include <QString>
#include <QVector>
#include <qdom.h>
//using namespace std;

class settingsParser
{

public:
	settingsParser();
	~settingsParser();
	QVector<QVector<QVector<QString>>> getSettings(QVector<QString> fileSource);

	
private:
	QDomElement root;
	QVector<QVector<QVector<QString>>> settings;
		
};

#endif // SETTINGSPARSER_H
