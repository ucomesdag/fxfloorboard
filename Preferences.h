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
#include <QFile>

//using namespace std;

class Preferences
{

public:
	static Preferences* Instance(); //Singleton patern design
	
	QString getPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName);
	void setPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName, QString prefValueData);
	void loadPreferences();

protected :
	Preferences();
	~Preferences();

private:
	static Preferences* _instance;

	QDomElement root;
	QVector<QString> metaSearch;
	QVector<QString> prefValues;
	QVector<QString> groupNames;
	QVector<QString> typeNames;
	QVector<QString> itemNames;
};

Preferences::Preferences() 
{
	loadPreferences();
};

Preferences::~Preferences()
{
	//Write back to file/create
};

Preferences* Preferences::_instance = 0;// initialize pointer
Preferences* Preferences::Instance() 
  {
    if (_instance == 0)  // is it the first call?
    {  
      _instance = new Preferences; // create sole instance
    }
    return _instance; // address of sole instance
  }

QString Preferences::getPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName)
{
	/* Look op and return of the value coresponding to the group->type->item */
	QString setting = this->prefValues
		.at( this->metaSearch.indexOf(QString(prefGroupName + ":" + prefTypeName + ":" + prefItemName)) );
	return setting;
};

void Preferences::setPreferences(QString prefGroupName, QString prefTypeName, QString prefItemName, QString prefValueData)
{
	
};

void Preferences::loadPreferences()
{	
	/* Loads the xml document and creates the QDomElement root */
	QDomDocument doc( "Application Preferences" );
	QFile file( "preferences.xml" );
	doc.setContent( &file );                    // file is a QFile
	file.close();
	QDomElement root = doc.documentElement();   // Points to <Preferences>
	this->root = root;

	/* Iterate trough xml tree and load in to the assigned vector */
	QDomNode node = root.firstChild(); // Points to prefBuffer group
	while ( !node.isNull() ) 
	{
		QString _groupName = node.nodeName();
		
		QDomNode childNode = node.firstChild(); // Points to prefBuffer type
		while ( !childNode.isNull() ) 
		{
			QString _typeName = childNode.nodeName();

			for (unsigned int itemNum=0;itemNum<childNode.attributes().count();itemNum++ ) 
			{   // Iterates trough all the atributes
				QString _itemName = childNode.attributes().item(itemNum).nodeName();
				QString _value = childNode.attributes().namedItem(_itemName).nodeValue();
				
				this->metaSearch.append(_groupName + ":" + _typeName + ":" + _itemName);
				this->prefValues.append(_value);
				this->groupNames.append(_groupName);
				this->typeNames.append(_typeName);
				this->itemNames.append(_itemName);
			};

			childNode = childNode.nextSibling();
		};
		
		node = node.nextSibling();
	};
	
};

#endif // PREFERENCES_H