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

#include "customControlKnob.h"
#include "MidiTable.h"
#include "SysxIO.h"

customControlKnob::customControlKnob(QWidget *parent, 
									 QString hex1, QString hex2, QString hex3, 
									 QString background, QString direction, int lenght)
	: QWidget(parent)
{
	this->display = new QLineEdit(this);
	this->label = new QLabel(this);
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	QFont labelFont;
	labelFont.setFamily("Arial");
	labelFont.setBold(true);
	labelFont.setPixelSize(9);
	labelFont.setStretch(100);

	QPalette labelPal;
	labelPal.setColor(this->label->foregroundRole(), Qt::white);

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", hex1, hex2, hex3);
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	QString labeltxt = items.customdesc;

	this->label->setPalette(labelPal);
	this->label->setFont(labelFont);
	this->label->setText(labeltxt.toUpper());
	
	QPoint labelPos, displayPos, knobPos, bgPos;
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
		int labellenght = 50;
		
		bgPos = QPoint(0, 0 + 9);
		knobPos = QPoint(bgPos.x() + 5, bgPos.y() + 6);
		labelPos = QPoint(knobPos.x()  + ((37 - labellenght) / 2), 0);
		displayPos = QPoint(knobPos.x() + ((37 - lenght) / 2), knobPos.y() + 37 + 7);

		this->label->setFixedWidth(labellenght);
		this->label->setAlignment(Qt::AlignCenter);
	};

	this->label->move(labelPos);

	QLabel *newBackGround = new QLabel(this);
	if (background == "normal")
	{
		newBackGround->setPixmap(QPixmap(":/images/knobbgn.png"));
	}
	else if (background == "turbo")
	{
		newBackGround->setPixmap(QPixmap(":/images/knobbgt.png"));
	}
	else
	{
		newBackGround->setPixmap(QPixmap(":/images/knobbg.png"));
	};
	newBackGround->move(bgPos);

	QString imagePath(":/images/knob.png");
	unsigned int imageRange = 63;
	this->knob = new customDial(0, 0, range, 1, 10, knobPos, this, hex1, hex2, hex3, imagePath, imageRange);

	QPalette displayPal;
	displayPal.setColor(QPalette::Base,QColor(0,1,62));
    displayPal.setColor(QPalette::Text,QColor(0,255,204));
	displayPal.setColor(QPalette::Highlight,QColor(0,1,62));
	displayPal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	displayPal.setColor(QPalette::Window,QColor(0,1,62));
	displayPal.setColor(QPalette::WindowText,QColor(0,255,204));	//List Border
	displayPal.setColor(QPalette::Button,QColor(0,1,62));
	displayPal.setColor(QPalette::ButtonText,QColor(0,255,204));

	displayPal.setColor(QPalette::Light,QColor(0,1,62));			//Lighter than Button color.
	displayPal.setColor(QPalette::Midlight,QColor(0,1,62));			//Between Button and Light.
	displayPal.setColor(QPalette::Dark,QColor(0,1,62));				//Darker than Button.
	displayPal.setColor(QPalette::Mid,QColor(0,1,62));				//Between Button and Dark.
	displayPal.setColor(QPalette::Shadow,QColor(0,1,62));

	this->display->setPalette(displayPal);
	this->display->setFixedWidth(lenght);
	this->display->setFixedHeight(13);
	this->display->setAlignment(Qt::AlignRight);
	this->display->setDisabled(true);
	this->display->move(displayPos);

	valueChanged(0, hex1, hex2, hex3);

	QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                this, SLOT( dialogUpdateSignal() ));
	/*QObject::connect(this, SIGNAL( updateSignal() ),
                this->parent(), SIGNAL( updateSignal() ));*/
};

void customControlKnob::paintEvent(QPaintEvent *)
{
	/*QPixmap image(":images/dragbar.png");
	
	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);*/
};

void customControlKnob::valueChanged(int value, QString hex1, QString hex2, QString hex3)
{
	QString valueHex = QString::number(value, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");

	MidiTable *midiTable = MidiTable::Instance();
	QString valueStr = midiTable->getValue("Stucture", hex1, hex2, hex3, valueHex);
	
	this->display->setText(valueStr);
};

void customControlKnob::dialogUpdateSignal()
{
	SysxIO *sysxIO = SysxIO::Instance();
	int value = sysxIO->getSourceValue(this->hex1, this->hex2, this->hex3);
	this->knob->setValue(value);
	this->valueChanged(value, this->hex1, this->hex2, this->hex3);
};
