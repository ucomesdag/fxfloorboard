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

#include "customControlSwitch.h"
#include "MidiTable.h"
#include "SysxIO.h"

customControlSwitch::customControlSwitch(QWidget *parent, 
									 QString hex1, QString hex2, QString hex3, 
									 QString direction)
	: QWidget(parent)
{
	this->label = new customControlLabel(this);
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Structure", hex1, hex2, hex3);
	QString labeltxt = items.customdesc;
	
	this->label->setUpperCase(true);
	this->label->setText(labeltxt);
	
	QString imagePath(":/images/switch.png");
	this->switchbutton = new customSwitch(false, this, hex1, hex2, hex3, imagePath);	

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
		this->label->setAlignment(Qt::AlignLeft);
		
		QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->setMargin(0);
		mainLayout->setSpacing(0);
		mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
		mainLayout->addWidget(this->switchbutton, 0, Qt::AlignLeft);
		mainLayout->addStretch(0);

		this->setLayout(mainLayout);
		this->setFixedHeight(12 + 15);
	}
	else if(direction == "middle")
	{
		this->label->setAlignment(Qt::AlignLeft);

		QVBoxLayout *mainLayout = new QVBoxLayout;
		mainLayout->setMargin(0);
		mainLayout->setSpacing(0);
		mainLayout->addStretch(0);
		mainLayout->addWidget(this->label, 0, Qt::AlignLeft);
		mainLayout->addWidget(this->switchbutton, 0, Qt::AlignCenter);

		this->setLayout(mainLayout);
		this->setFixedHeight(12 + 15);
	};

	QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                this, SLOT( dialogUpdateSignal() ));

	QObject::connect(this, SIGNAL( updateSignal() ),
                this->parent(), SIGNAL( updateSignal() ));
};

void customControlSwitch::paintEvent(QPaintEvent *)
{
	/*DRAWS RED BACKGROUND FOR DEBUGGING PURPOSE */
	/*QPixmap image(":images/dragbar.png");
	
	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);*/
};

void customControlSwitch::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
	QString valueHex;
	if(value)
	{
		valueHex = "01";
	}
	else
	{
		valueHex = "00";
	};

	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(hex1, hex2, hex3, valueHex);

	//emit updateDisplay(valueStr);
	emit updateSignal();
};

void customControlSwitch::dialogUpdateSignal()
{
	SysxIO *sysxIO = SysxIO::Instance();
	int value = sysxIO->getSourceValue(this->hex1, this->hex2, this->hex3);
	if(value == 0)
	{
		this->switchbutton->setValue(false);
	}
	else
	{
		this->switchbutton->setValue(true);
	};
	//this->valueChanged(value, this->hex1, this->hex2, this->hex3);
};
