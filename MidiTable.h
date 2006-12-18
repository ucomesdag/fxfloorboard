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

#ifndef MIDITABLE_H
#define MIDITABLE_H

#include <QString>
#include <QVector>
#include <qdom.h>

/* Declaring struc types */
struct Midi
{
	QString value, name, abbr, desc;
	QVector<QString> type, id;
	QVector<Midi> level;
};

class MidiTableDestroyer;

class MidiTable
{

public:
	static MidiTable* Instance(); //Singleton patern design

	void loadMidiMap();
	Midi getMidiMap(QString root, QString hex1);
	Midi getMidiMap(QString root, QString hex1, QString hex2);
	Midi getMidiMap(QString root, QString hex1, QString hex2, QString hex3);
	Midi getMidiMap(QString root, QString hex1, QString hex2, QString hex3, QString hex4);
	Midi getMidiMap(QString root, QString hex1, QString hex2, QString hex3, QString hex4, QString hex5);
	
protected:
	MidiTable();
	friend class MidiTableDestroyer;
	virtual ~MidiTable() { };

private:
	static MidiTable* _instance;
	static MidiTableDestroyer _destroyer;

	QDomElement root;
	Midi midiMap;
};

#endif // MIDITABLE_H
