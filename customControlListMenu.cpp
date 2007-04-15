/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8 Fx FloorBoard".
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

#include "customControlListMenu.h"
#include "MidiTable.h"
#include "SysxIO.h"

customControlListMenu::customControlListMenu(QWidget *parent, 
									 QString hex1, QString hex2, QString hex3, 
									 QString direction)
	: QWidget(parent)
{
	this->label = new customControlLabel(this);
	this->combobox = new QComboBox(this);
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", hex1, hex2, hex3);
	QString labeltxt = items.customdesc;
	
	this->label->setPixelSize(9);
	this->label->setStretch(100);
	this->label->setUpperCase(true);
	this->label->setText(labeltxt);

	this->comboPalette.setColor(QPalette::Base,QColor(0,1,62));
    this->comboPalette.setColor(QPalette::Text,QColor(0,255,204));
	this->comboPalette.setColor(QPalette::Highlight,QColor(0,1,62));
	this->comboPalette.setColor(QPalette::HighlightedText,QColor(0,255,204));

	this->comboPalette.setColor(QPalette::Window,QColor(0,1,62));
	this->comboPalette.setColor(QPalette::WindowText,QColor(0,255,204));	//List Border
	this->comboPalette.setColor(QPalette::Button,QColor(0,1,62));
	this->comboPalette.setColor(QPalette::ButtonText,QColor(0,255,204));

	this->comboPalette.setColor(QPalette::Light,QColor(0,1,62));			//Lighter than Button color.
	this->comboPalette.setColor(QPalette::Midlight,QColor(0,1,62));		//Between Button and Light.
	this->comboPalette.setColor(QPalette::Dark,QColor(0,1,62));			//Darker than Button.
	this->comboPalette.setColor(QPalette::Mid,QColor(0,1,62));			//Between Button and Dark.
	this->comboPalette.setColor(QPalette::Shadow,QColor(0,1,62));

	this->comboFont.setFamily("Arial");
	this->comboFont.setBold(true);
	this->comboFont.setPixelSize(10);
	this->comboFont.setStretch(110);

	setComboBox();
	
	QPoint labelPos, comboboxPos;
	if(direction == "left")
	{
		
	}
	else if(direction == "right")
	{
		
	}
	else if(direction == "top")
	{
		
	}
	else if(direction == "bottom")
	{
		//this->label->setFixedWidth(this->combobox->width());
		this->label->setAlignment(Qt::AlignLeft);
		
		labelPos = QPoint(0, 0);
		comboboxPos = QPoint(0, 12);

		this->setFixedSize(this->comboWidth + 15, 12 + 17);
	};

	this->label->move(labelPos);
	this->combobox->move(comboboxPos);
	

	QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                this, SLOT( dialogUpdateSignal() ));

	QObject::connect(this, SIGNAL( updateSignal() ),
                this->parent(), SIGNAL( updateSignal() ));

	QObject::connect(this->combobox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(valueChanged(int)));

	QObject::connect(this->combobox, SIGNAL(currentIndexChanged(int)),
                this, SIGNAL(currentIndexChanged(int)));
};

void customControlListMenu::paintEvent(QPaintEvent *)
{
	QPixmap image(":images/dragbar.png");
	
	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
};

void customControlListMenu::setComboBox()
{
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	int maxLenght = 0;
	int itemsCount;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", hex1, hex2, hex3);

	for(itemsCount=0;itemsCount<items.level.size();itemsCount++ )
	{
		QString item;
		QString desc = items.level.at(itemsCount).desc;
		QString customdesc = items.level.at(itemsCount).customdesc;
		if(!customdesc.isEmpty())
		{
			item = customdesc;
		}
		else
		{
			item = desc;
		};
		this->combobox->addItem(item);
		int pixelWidth = QFontMetrics(this->comboFont).width(item);
		if(maxLenght < pixelWidth) maxLenght = pixelWidth;
	};

	this->comboWidth = maxLenght + 10;

	//this->combobox->setGeometry(geometry);
	this->combobox->setEditable(false);
	this->combobox->setFont(this->comboFont);
	this->combobox->setPalette(this->comboPalette);
	this->combobox->setFrame(false);
	this->combobox->setMaxVisibleItems(itemsCount);
	this->combobox->view()->setMinimumWidth( this->comboWidth );
};

void customControlListMenu::valueChanged(int index)
{
	QString valueHex = QString::number(index, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");

	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(hex1, hex2, hex3, valueHex);

	//emit updateDisplay(valueStr);
	emit updateSignal();
};

void customControlListMenu::dialogUpdateSignal()
{
	SysxIO *sysxIO = SysxIO::Instance();
	int index = sysxIO->getSourceValue(this->hex1, this->hex2, this->hex3);
	this->combobox->setCurrentIndex(index);
	//this->valueChanged(index);
};
