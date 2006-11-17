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

#include "preferencesParser.h"	
#include <QVector>
#include <QFile>

preferencesParser::preferencesParser() 
{
	QDomDocument doc( "Application Prefferences" );
	QFile file( "preferences.xml" );
	doc.setContent( &file );                    // file is a QFile
	file.close();
	QDomElement root = doc.documentElement();   // Points to <SysX>
	this->root = root;
	loadPreferences();
};

preferencesParser::~preferencesParser()
{
	//Write back to file/create
};

QString preferencesParser::getSetting(QString prefGroup, QString prefType)
{
	return "void";
};

void preferencesParser::setSetting(QString prefGroup, QString prefType, QVector<QString> prefItem, QVector<QString> prefValue)
{
	
};

void preferencesParser::loadPreferences()
{
	this->prefGroup;
	this->prefType;
	this->prefItem;
	this->prefValue;
	this->prefIndex;
};