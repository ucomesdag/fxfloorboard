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
	this->label = new customControlLabel(this);
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Structure", hex1, hex2, hex3);
	QString labeltxt = items.customdesc;
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	
	this->label->setPixelSize(9);
	this->label->setStretch(100);
	this->label->setUpperCase(true);
	this->label->setText(labeltxt);
	
	QPoint labelPos, displayPos, knobPos, bgPos;
	if(direction == "left")
	{
		
	}
	else if(direction == "right")
	{
		//this->label->setFixedWidth(this->width());
		this->label->setAlignment(Qt::AlignLeft);
		
		/*bgPos = QPoint(1, 0 + 9);
		knobPos = QPoint(bgPos.x() + 5, bgPos.y() + 6);
		labelPos = QPoint(knobPos.x()  + ((37 - this->label->width()) / 2), 0);
		displayPos = QPoint(50, knobPos.y() + ((37 - 13) / 2));

		this->setFixedSize(50 + lenght, bgPos.y() + 50);*/

		bgPos = QPoint(1, 0);
		knobPos = QPoint(bgPos.x() + 5, bgPos.y() + 6);
		labelPos = QPoint(50, knobPos.y());
		displayPos = QPoint(50, knobPos.y() + ((37 - 13) / 2));

		this->setFixedSize(50 + lenght, 50);
	}
	else if(direction == "top")
	{
		
	}
	else if(direction == "bottom")
	{
		this->label->setFixedWidth(this->width());
		this->label->setAlignment(Qt::AlignCenter);
		
		bgPos = QPoint(1, 0 + 9);
		knobPos = QPoint(bgPos.x() + 5, bgPos.y() + 6);
		labelPos = QPoint(knobPos.x()  + ((37 - this->label->width()) / 2), 0);
		displayPos = QPoint(knobPos.x() + ((37 - lenght) / 2), knobPos.y() + 37 + 7);

		this->setFixedSize(50, displayPos.y() + 13 + 2);
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

	QFont displayFont;
	displayFont.setFamily("Arial");
	displayFont.setBold(false);
	displayFont.setPixelSize(10);
	displayFont.setStretch(110);	
	
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

	this->display->setFont(displayFont);
	this->display->setPalette(displayPal);
	this->display->setFixedWidth(lenght);
	this->display->setFixedHeight(13);
	this->display->setAlignment(Qt::AlignRight);
	this->display->setDisabled(true);
	this->display->move(displayPos);

	QObject::connect(this->parent(), SIGNAL( dialogUpdateSignal() ),
                this, SLOT( dialogUpdateSignal() ));

	QObject::connect(this, SIGNAL( updateSignal() ),
                this->parent(), SIGNAL( updateSignal() ));

	QObject::connect(this, SIGNAL( updateDisplay(QString) ),
                this->display, SLOT( setText(QString) ));
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
	MidiTable *midiTable = MidiTable::Instance();
	
	QString valueHex = QString::number(value, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");

	SysxIO *sysxIO = SysxIO::Instance(); bool ok;
	if(midiTable->isData("Structure", hex1, hex2, hex3))
	{	
		int maxRange = QString("7F").toInt(&ok, 16) + 1;
		int value = valueHex.toInt(&ok, 16);
		int dif = value/maxRange;
		QString valueHex1 = QString::number(dif, 16).toUpper();
		if(valueHex1.length() < 2) valueHex1.prepend("0");
		QString valueHex2 = QString::number(value - (dif * maxRange), 16).toUpper();
		if(valueHex2.length() < 2) valueHex2.prepend("0");
		
		sysxIO->setFileSource(hex1, hex2, hex3, valueHex1, valueHex2);		
	}
	else
	{
		sysxIO->setFileSource(hex1, hex2, hex3, valueHex);
	};

	QString valueStr = midiTable->getValue("Structure", hex1, hex2, hex3, valueHex);
	
	//this->display->setText(valueStr);
	emit updateDisplay(valueStr);
	emit updateSignal();
};

void customControlKnob::dialogUpdateSignal()
{
	SysxIO *sysxIO = SysxIO::Instance();
	int value = sysxIO->getSourceValue(this->hex1, this->hex2, this->hex3);
	this->knob->setValue(value);

	QString valueHex = QString::number(value, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");

	MidiTable *midiTable = MidiTable::Instance();
	QString valueStr = midiTable->getValue("Structure", hex1, hex2, hex3, valueHex);
	
	//this->display->setText(valueStr);
	emit updateDisplay(valueStr);
	//this->valueChanged(value, this->hex1, this->hex2, this->hex3);
};
