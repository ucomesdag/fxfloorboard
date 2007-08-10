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

#include "stompBox.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "globalVariables.h"

stompBox::stompBox(QWidget *parent, unsigned int id, QString imagePath, QPoint stompPos)
    : QWidget(parent)
{
	this->id = id;
	this->imagePath = imagePath;
	this->stompSize = QPixmap(imagePath).size();
	this->stompPos = stompPos;	

	this->setFixedSize(stompSize);

	this->editDialog = new editWindow();

	QObject::connect(this, SIGNAL( valueChanged(QString, QString, QString) ),
                this->parent(), SIGNAL( valueChanged(QString, QString, QString) ));
	
	QObject::connect(this->parent(), SIGNAL( updateStompOffset(signed int) ),
                this, SLOT( updatePos(signed int) ));

	QObject::connect(this->parent(), SIGNAL( updateSignal() ),
                this, SLOT( updateSignal() ));

	QObject::connect(this->editDialog, SIGNAL( updateSignal() ),
                this, SLOT( updateSignal() ));

	QObject::connect(this, SIGNAL( dialogUpdateSignal() ),
                this->editDialog, SIGNAL( dialogUpdateSignal() ));	

	QObject::connect(this->parent(), SIGNAL( updateSignal() ),
                this->editDialog, SIGNAL( dialogUpdateSignal() ));

	QObject::connect(this->editDialog, SIGNAL( updateSignal() ),
                this, SLOT( setDisplayToFxName() ));

	QObject::connect(this, SIGNAL( setEditDialog(editWindow*) ),
                this->parent(), SLOT( setEditDialog(editWindow*) ));
};

void stompBox::paintEvent(QPaintEvent *)
{
	QRectF target(0.0, 0.0, stompSize.width(), stompSize.height());
	QRectF source(0.0, 0.0, stompSize.width(), stompSize.height());
	QPixmap image(imagePath);

	this->image = image;

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
};

editWindow* stompBox::editDetails()
{
	return this->editDialog;
};

void stompBox::mousePressEvent(QMouseEvent *event) 
{ 
	emitValueChanged(this->hex1, this->hex2, "00", "void");

	if (event->button() == Qt::LeftButton) 
	{
		this->dragStartPosition = event->pos(); 
	}
	else if (event->button() == Qt::RightButton)
	{
		this->editDialog->setWindow(this->fxName);
		emit setEditDialog(this->editDialog);
	};
};

void stompBox::mouseDoubleClickEvent(QMouseEvent *event)
{
	event;
	this->editDialog->setWindow(this->fxName);
	emit setEditDialog(this->editDialog);
};

void stompBox::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton)
	{
        QPoint mousePoint = event->pos();
		QRect stompRect = this->rect();

		if ( /*(event->pos() - dragStartPosition).manhattanLength() > QApplication::startDragDistance() &&*/
			stompRect.contains(mousePoint) )
		{
			
			QByteArray itemData;
			QDataStream dataStream(&itemData, QIODevice::WriteOnly);
			dataStream << id << stompPos << stompSize << event->pos() - rect().topLeft();
			
			QMimeData *mimeData = new QMimeData;
			mimeData->setData("application/x-stompbox", itemData);
			mimeData->setText(this->objectName());

			QDrag *drag = new QDrag(this);
			drag->setMimeData(mimeData);
			drag->setHotSpot(event->pos() - rect().topLeft());
			
			QPixmap screen = QPixmap::grabWidget(this);
			screen.setMask(image.mask()); 

			QRectF source(4, 4, stompSize.width()-8, stompSize.height()-8);
			QRectF target(4, 4, stompSize.width()-8, stompSize.height()-8);
			
			QPixmap buffer = image;
			QPainter painter(&buffer);
			painter.drawPixmap(target, screen, source);
			painter.end();

			drag->setPixmap(buffer);
			
			hide();
			
			if (drag->start(Qt::MoveAction) != Qt::MoveAction)
			{
				event->ignore();
				show();
			};

			if(drag->source() != drag->target())
			{
				event->ignore();
				show();
			};
		};
	};
};

void stompBox::setPos(QPoint newPos)
{
	this->move(newPos);
	this->stompPos = newPos;
};

void stompBox::updatePos(signed int offsetDif)
{
	this->stompPos = this->pos();
	QPoint newPos = stompPos + QPoint::QPoint(offsetDif, 0);
	this->move(newPos);
	this->stompPos = newPos;
};
	
void stompBox::setImage(QString imagePath)
{
	this->imagePath = imagePath;
	this->update();
};

void stompBox::setSize(QSize newSize)
{
	this->stompSize = newSize;
	this->setFixedSize(stompSize);
};

void stompBox::setId(unsigned int id)
{
	this->id = id;
};

unsigned int stompBox::getId()
{
	return this->id;
};

void stompBox::setLSB(QString hex1, QString hex2)
{
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->editDialog->setLSB(hex1, hex2);
};

void stompBox::setComboBox(QString hex1, QString hex2, QString hex3, QRect geometry)
{
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Structure", hex1, hex2, hex3);

	this->stompComboBox = new customComboBox(this);
	this->stompComboBox->setObjectName("smallcombo");
	
	int itemcount = 0; 
	for(itemcount=0;itemcount<items.level.size();itemcount++ )
	{
		QString item;
		QString desc = items.level.at(itemcount).desc;
		QString customdesc = items.level.at(itemcount).customdesc;
		if(!customdesc.isEmpty())
		{
			item = customdesc;
		}
		else
		{
			item = desc;
		};
		this->stompComboBox->addItem(item);
	};

	this->stompComboBox->setGeometry(geometry);
	this->stompComboBox->setEditable(false);
	this->stompComboBox->setFrame(false);
	this->stompComboBox->setMaxVisibleItems(itemcount);

	QObject::connect(this->stompComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(valueChanged(int)));
};

void stompBox::setComboBoxCurrentIndex(int index)
{
	this->stompComboBox->setCurrentIndex(index);
};

void stompBox::setKnob1(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	knob1 = new customDial(0, 0, range, 1, 10, QPoint::QPoint(6, 9), this, hex1, hex2, hex3);
};

void stompBox::setKnob2(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	knob2 = new customDial(0, 0, range, 1, 10, QPoint::QPoint(53, 9), this, hex1, hex2, hex3);
};

void stompBox::setSlider1(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	slider1 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(8, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider2(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	slider2 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(24, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider3(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	slider3 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(40, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider4(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	slider4 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(56, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider5(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Structure", hex1, hex2, hex3);
	slider5 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(79, 17), this, hex1, hex2, hex3);
};

void stompBox::setButton(QString hex1, QString hex2, QString hex3)
{
	button = new customButton(false, QPoint::QPoint(4, 110), this, hex1, hex2, hex3);
	led = new customLed(false, QPoint::QPoint(41, 4), this);

	QObject::connect(button, SIGNAL(valueChanged(bool, QString, QString, QString)),
				led, SLOT(changeValue(bool)));	
};

void stompBox::setButton(QString hex1, QString hex2, QString hex3, QPoint pos, QString imagePath)
{
	button = new customButton(false, pos, this, hex1, hex2, hex3, imagePath);
	led = new customLed(false, QPoint::QPoint(41, 4), this);

	QObject::connect(button, SIGNAL(valueChanged(bool, QString, QString, QString)),
				led, SLOT(changeValue(bool)));	
};

void stompBox::setSwitch(QString hex1, QString hex2, QString hex3)
{
	switchbutton = new customSwitch(false, this, hex1, hex2, hex3);	
	switchbutton->move(QPoint::QPoint(5, 41));
};

void stompBox::updateComboBox(QString hex1, QString hex2, QString hex3)
{	
	QObject::disconnect(this->stompComboBox, SIGNAL(currentIndexChanged(int)), // To prevent sending a signal when loading a patch.
                this, SLOT(valueChanged(int)));
	
	SysxIO *sysxIO = SysxIO::Instance();
	setComboBoxCurrentIndex(sysxIO->getSourceValue(hex1, hex2, hex3));

	QObject::connect(this->stompComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(valueChanged(int)));
};

void stompBox::updateKnob1(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	knob1->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateKnob2(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	knob2->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider1(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	slider1->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider2(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	slider2->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider3(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	slider3->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider4(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	slider4->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider5(QString hex1, QString hex2, QString hex3)
{	
	SysxIO *sysxIO = SysxIO::Instance();
	slider5->setValue(sysxIO->getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateButton(QString hex1, QString hex2, QString hex3)
{
	SysxIO *sysxIO = SysxIO::Instance();
	int value = sysxIO->getSourceValue(hex1, hex2, hex3);
	if(hex1 == "15")
	{
		//Exception for the Foot Volume -> it's on when Expresion switch is off.
		(value==1)?value=0:value=1;
	};
	led->setValue((value==1)?true:false);
	button->setValue((value==1)?true:false);
};

void stompBox::updateSwitch(QString hex1, QString hex2, QString hex3)
{
	SysxIO *sysxIO = SysxIO::Instance();
	int value = sysxIO->getSourceValue(hex1, hex2, hex3);
	switchbutton->setValue((value==1)?true:false);
};

void stompBox::valueChanged(int value, QString hex1, QString hex2, QString hex3)
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

	emitValueChanged(hex1, hex2, hex3, valueHex);
};

void stompBox::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
	int valueInt;
	(value)? valueInt=1: valueInt=0;
	QString valueHex = QString::number(valueInt, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");
	
	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(hex1, hex2, hex3, valueHex);

	emitValueChanged(hex1, hex2, hex3, valueHex);
};

void stompBox::valueChanged(int index)
{
	QString valueHex = QString::number(index, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");
	
	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(this->hex1, this->hex2, this->hex3, valueHex);

	emitValueChanged(this->hex1, this->hex2, this->hex3, valueHex);

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Structure", this->hex1, this->hex2, this->hex3);

	QString desc = items.level.at(index).desc;
	QString customdesc = items.level.at(index).customdesc;
	if(customdesc.isEmpty())
	{
		customdesc = desc;
	};

	this->stompComboBox->setCurrentIndex(index);
	this->stompComboBox->setEditText(desc);
};

void stompBox::emitValueChanged(QString hex1, QString hex2, QString hex3, QString valueHex)
{
	QString valueName, valueStr;
	if(hex1 != "void" && hex2 != "void")
	{
		MidiTable *midiTable = MidiTable::Instance();
		if(valueHex != "void")
		{
			Midi items = midiTable->getMidiMap("Structure", hex1, hex2, hex3);
			valueName = items.desc;
			if(hex1 == "0E") // NoiseSuppressor is part of MASTER -> correcting the name for consistency.
			{
				this->fxName = "Noise Suppressor";
				if(items.desc == "NS :Effect")
				{
					valueName = "On/Off";
				}
				else
				{
					valueName = items.desc.remove("NS :");
				};
			}
			else
			{
				this->fxName = midiTable->getMidiMap("Structure", hex1).name;
				if(items.desc.contains(":"))
				{
					valueName = items.desc.section(":", 1, 1);
				}
				else
				{
					valueName = items.desc;
				};
			};
			valueStr = midiTable->getValue("Structure", hex1, hex2, hex3, valueHex);
			emit dialogUpdateSignal();
		}
		else
		{
			if(hex1 == "0E") // NoiseSuppressor is part of MASTER -> correcting the name for consistency.
			{
				this->fxName = "Noise Suppressor";
			}
			else if(this->hex1 == "15") // Expression Pedal -> correcting the name for consistency.
			{
				this->fxName = "Foot Volume";
			}
			else
			{
				this->fxName = midiTable->getMidiMap("Structure", hex1).name;
			};
		};
	}
	else
	{
		this->fxName = "Digital Out";
	};

	emit valueChanged(this->fxName, valueName, valueStr);
};

void stompBox::setDisplayToFxName()
{
	emit valueChanged(this->fxName, "", "");
};
