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

#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QVector>
#include <qdom.h>
//using namespace std;

class PreferencesDestroyer;

class Preferences
{

public:
	static Preferences* Instance(); //Singleton patern design
	
	QString getPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName);
	void setPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName, QString prefValueData);
	void loadPreferences(QString fileName);
	void savePreferences();

protected :
	Preferences();
	friend class PreferencesDestroyer;
	virtual ~Preferences() { };

private:
	static Preferences* _instance;
	static PreferencesDestroyer _destroyer;

	QDomElement root;
	QVector<QString> metaSearch;
	QVector<QString> prefValues;
	QVector<QString> groupNames;
	QVector<QString> typeNames;
	QVector<QString> itemNames;
};

#endif // PREFERENCES_H