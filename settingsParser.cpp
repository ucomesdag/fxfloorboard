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

#include "settingsParser.h"	
#include <QFile>

settingsParser::settingsParser() 
{
	QDomDocument doc( "MIDI Transalation" );
	QFile file( "midi_table.xml" );
	doc.setContent( &file );                    // file is a QFile
	file.close();
	QDomElement root = doc.documentElement();   // Points to <SysX>
	this->root = root;
};
settingsParser::~settingsParser()
{

};
QString rangeValue(QString data, QString dataMin, QString dataMax, QString rangeMin, QString rangeMax)
{	
	bool ok;
	signed int dataRange = dataMax.toInt(&ok, 16) - dataMin.toInt(&ok, 16);
	double range = rangeMax.toDouble(&ok) - rangeMin.toDouble(&ok);
	double result = (rangeMin.toDouble(&ok) - dataMin.toInt(&ok, 16)) + (data.toInt(&ok, 16) / (dataRange / range));
	if(rangeMin.contains(".") || rangeMax.contains(".")) return QString::number(result,'f', 1);
	else return QString::number(result,'f', 0);
}
QVector<QVector<QVector<QString>>> settingsParser::getSettings(QVector<QString> fileSource)
{
	
	QVector<QVector<QVector<QString>>> settings;
	for (QVector<QString>::iterator dev = fileSource.begin(); dev != fileSource.end(); ++dev)
	{
		QString str(*dev);

		QVector<QVector<QString>> block;
		unsigned int offset=0;
		unsigned int dataSize = 0;
		bool ok;
		bool nextLSB = false;
		QString name, value, abbr, desc;

		QDomNode node = root.firstChild(); // Points to <Header>

		QString tag = node.nodeName();

		for(int i=0;i<str.length();i++)
		{
			QVector<QString> row;
			QString data = str.mid(i, 2);
			if (offset==0)
			{
				node = node.firstChild();
				tag = node.nodeName();
			}
			else if (offset==7)
			{
				node = node.parentNode();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.firstChild();
				tag = node.nodeName();
			}
			else if (offset==9) 
			{
				node = node.parentNode();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.firstChild();
				tag = node.nodeName();
			}
			else if (data=="F7")
			{
				node = root.firstChild();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.firstChild();				
				tag = node.nodeName();
			}
			else if (offset!=11 && str.mid(i+2, 2)!="F7" && !nextLSB) 
			{
				if(!node.nextSibling().isNull()) node = node.nextSibling();
			} 
			else if (nextLSB) 
			{
				node = node.parentNode();
				tag = node.nodeName();
				node = node.parentNode();
				tag = node.nodeName();
				node = node.nextSibling();
				tag = node.nodeName();
				node = node.firstChild();
				tag = node.nodeName();
				node = node.firstChild();
				tag = node.nodeName();
				nextLSB = false;
			} ;
			
			
			if(node.attributes().namedItem( "value" ).nodeValue()=="7F") 
			{
				nextLSB = true;
			}
			signed int depth=0;
			bool isrange;

			if(str.mid(i+2, 2)=="F7")
			{
				bool ok;
				QString base = "80";
				unsigned int checksum = dataSize % base.toInt(&ok, 16);
				if(checksum!=0) checksum = base.toInt(&ok, 16) - checksum;
				QString result = QString::number(checksum, 16).toUpper();
				if(result.length()<2) result = "0" + result;
				QString msg = "Checksum ";
				if(data==result) msg += "correct"; else msg += "error!";
				msg += " (" + result + ")";
				
				row.append("");
				row.append(msg);
				row.append("");
				row.append("");
				block.append(row);
			}
			else if(offset==9)
			{
				if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
				while(data!=node.attributes().namedItem( "value" ).nodeValue() && !node.isNull()
					&& node.attributes().namedItem( "value" ).nodeValue()!="range"
					&& node.attributes().namedItem( "value" ).nodeValue()!="assign")
				{
					node = node.nextSibling();
				};
				name = node.parentNode().attributes().namedItem( "desc" ).nodeValue(); row.append(name);
				value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
				desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
				abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
				block.append(row);
				row.clear();

				node = node.firstChild();
				tag = node.nodeName();
				i = i+2; data = str.mid(i, 2); offset++;

				if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
				while(data!=node.attributes().namedItem( "value" ).nodeValue() && !node.isNull()
					&& node.attributes().namedItem( "value" ).nodeValue()!="range"
					&& node.attributes().namedItem( "value" ).nodeValue()!="assign")
				{
					node = node.nextSibling();
				};
				name = ""; row.append(name);//node.parentNode().attributes().namedItem( "desc" ).nodeValue(); 
				value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
				desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
				abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
				block.append(row);
				row.clear();

				node = node.firstChild();
				tag = node.nodeName();
			}
			else
			{
				if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
				if (node.childNodes().size()>0) 
				{
					node = node.firstChild();
					while(data!=node.attributes().namedItem( "value" ).nodeValue() && !node.isNull())
					{
						if (node.attributes().namedItem( "value" ).nodeValue()=="range")
						{
							QString range = node.attributes().namedItem( "name" ).nodeValue();
							QString dataMin = range.section("/",0,0);
							QString dataMax = range.section("/",1,1);
							QString rangeMin = range.section("/",2,2);
							QString rangeMax = range.section("/",3,3);
							bool ok;
							if( dataMin.toInt(&ok, 16)<=data.toInt(&ok, 16) && data.toInt(&ok, 16)<=dataMax.toInt(&ok, 16) )
							{
								value = rangeValue(data, dataMin, dataMax, rangeMin, rangeMax);
								isrange = true; break;
							};
						}
						else if (node.attributes().namedItem( "value" ).nodeValue()=="assign")
						{
							i = i+2; data = str.mid(i, 2); offset++;
							if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
							name = node.parentNode().attributes().namedItem( "desc" ).nodeValue(); row.append(name);
							value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
							desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
							abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
							block.append(row);
							row.clear(); break;
						};
						node = node.nextSibling();
					};
					depth++;
				};
				name = node.parentNode().attributes().namedItem( "desc" ).nodeValue(); row.append(name);
				if (!isrange) value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
				desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
				abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
				block.append(row);
				row.clear();
				isrange = false;

				tag = node.nodeName();

				while (node.childNodes().size()>0) 
				{
					node = node.firstChild();
					tag = node.nodeName();
					i = i+2; data = str.mid(i, 2); offset++;

					if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
					while(data!=node.attributes().namedItem( "value" ).nodeValue() && !node.isNull())
					{
						if (node.attributes().namedItem( "value" ).nodeValue()=="range")
						{
							QString range = node.attributes().namedItem( "name" ).nodeValue();
							QString dataMin = range.section("/",0,0);
							QString dataMax = range.section("/",1,1);
							QString rangeMin = range.section("/",2,2);
							QString rangeMax = range.section("/",3,3);
							bool ok;
							if( dataMin.toInt(&ok, 16)<=data.toInt(&ok, 16) && data.toInt(&ok, 16)<=dataMax.toInt(&ok, 16) )
							{
								value = rangeValue(data, dataMin, dataMax, rangeMin, rangeMax);
								isrange = true; break;
							};
						} 
						else if (node.attributes().namedItem( "value" ).nodeValue()=="assign")
						{
							i = i+2; data = str.mid(i, 2); offset++;
							if(offset>6) dataSize = dataSize + data.toInt(&ok, 16);
							name = node.parentNode().attributes().namedItem( "desc" ).nodeValue(); row.append(name);
							value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
							desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
							abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
							block.append(row);
							row.clear(); break;
						};
						node = node.nextSibling();
					};
					depth++;
					name = ""; row.append(name);//node.parentNode().attributes().namedItem( "desc" ).nodeValue();
					if (!isrange) value = node.attributes().namedItem( "name" ).nodeValue(); row.append(value);
					desc = node.attributes().namedItem( "desc" ).nodeValue(); row.append(desc);
					abbr = node.attributes().namedItem( "abbr" ).nodeValue(); row.append(abbr);
					block.append(row);
					row.clear();
					isrange = false;
				};

				tag = node.nodeName();

			};			
			for(int level=0;level<depth;level++) node = node.parentNode();
			i++; offset++;

			tag = node.nodeName();			
		};
		settings.append(block);
		node = node.parentNode();
		node = node.firstChild();
	};
	this->settings = settings;
	return settings;
};