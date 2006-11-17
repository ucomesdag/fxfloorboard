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

#ifndef PREFERENCESPARSER_H
#define PREFERENCESPARSER_H

#include <QString>
#include <QVector>
#include <qdom.h>
//using namespace std;

class preferencesParser
{

public:
	preferencesParser();
	~preferencesParser();

	QString getSetting(QString prefGroup, QString prefType);
	void setSetting(QString prefGroup, QString prefType, QVector<QString> prefItem, QVector<QString> prefValue);
	void loadPreferences();

private:
	QDomElement root;

	QVector<QString> prefGroup;
	QVector<QString> prefType;
	QVector<QString> prefItem;
	QVector<QString> prefValue;
	QVector< QVector <QVector <unsigned int> > > prefIndex;
};

#endif // PREFERENCESPARSER_H

 
/*

<?xml version="1.0" encoding="UTF-8"?>
<!-- This is the preferences of the GT-8 FX FloorBoard application. -->
<Preferences>
  <General>
    <Application version="0.1a" />
    <Language code="en" />
    <Files dir="G:\My GT-8 Patches" />
    <Help url="http://sourceforge.net/forum/forum.php?forum_id=585786" />
    <Webpage url="http://fxfloorboard.sourceforge.net/" />
    <Donate url="http://sourceforge.net/donate/index.php?group_id=171049" />  
  </General>
  <Window>
    <Position x="0" y="0" />
    <Collapsed bool="0" />
    <Size width="" />
  </Window>
  <Midi> 
    <MidiIn device="" />
    <MidiOut device="" />
  </Midi>
</Preferences>

*/