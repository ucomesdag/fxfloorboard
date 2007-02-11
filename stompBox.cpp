/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
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

#include "stompBox.h"
#include "MidiTable.h"
#include "SysxIO.h"

stompBox::stompBox(QWidget *parent, unsigned int id, QString imagePath, QPoint stompPos)
    : QWidget(parent)
{
    this->pal.setColor(QPalette::Base,QColor(0,1,62));
    this->pal.setColor(QPalette::Text,QColor(0,255,204));
	this->pal.setColor(QPalette::Highlight,QColor(0,1,62));
	this->pal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	this->pal.setColor(QPalette::Window,QColor(0,1,62));
	this->pal.setColor(QPalette::WindowText,QColor(0,255,204));		//List Border
	this->pal.setColor(QPalette::Button,QColor(0,1,62));
	this->pal.setColor(QPalette::ButtonText,QColor(0,255,204));

	this->pal.setColor(QPalette::Light,QColor(0,1,62));				//Lighter than Button color.
	this->pal.setColor(QPalette::Midlight,QColor(0,1,62));			//Between Button and Light.
	this->pal.setColor(QPalette::Dark,QColor(0,1,62));				//Darker than Button.
	this->pal.setColor(QPalette::Mid,QColor(0,1,62));					//Between Button and Dark.
	this->pal.setColor(QPalette::Shadow,QColor(0,1,62));

	QFont font;
	this->font.setFamily("Arial");
	this->font.setBold(true);
	this->font.setPixelSize(10);
	this->font.setStretch(110);
	
	this->id = id;
	this->imagePath = imagePath;
	this->stompSize = QPixmap(imagePath).size();
	this->stompPos = stompPos;	

	this->setFixedSize(stompSize);

	QObject::connect(this, SIGNAL( valueChanged(QString, QString, QString) ),
                this->parent(), SIGNAL( valueChanged(QString, QString, QString) ));
	
	QObject::connect(this->parent(), SIGNAL( updateStompOffset(signed int) ),
                this, SLOT( updatePos(signed int) ));

	QObject::connect(this->parent(), SIGNAL( updateSignal() ),
                this, SLOT( updateSignal() ));
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

void stompBox::mousePressEvent(QMouseEvent *event) 
{ 
	if (event->button() == Qt::LeftButton) this->dragStartPosition = event->pos(); 
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

QPalette stompBox::getPal()
{
	return this->pal;
};

QFont stompBox::getFont()
{
	return this->font;
};

void stompBox::setComboBox(QString hex1, QString hex2, QString hex3, QRect geometry)
{
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;

	int maxLenght = 0;
	int itemsCount;

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", hex1, hex2, hex3);

	comboBox = new QComboBox(this);
	
	for(itemsCount=0;itemsCount<items.level.size();itemsCount++ )
	{
		QString item;
		QString desc = items.level.at(itemsCount).desc;
		QString longdesc = items.level.at(itemsCount).longdesc;
		if(!longdesc.isEmpty())
		{
			item = longdesc;
		}
		else
		{
			item = desc;
		};
		comboBox->addItem(item);
		int pixelWidth = QFontMetrics(this->getFont()).width(item);
		if(maxLenght < pixelWidth) maxLenght = pixelWidth;
	};
	comboBox->setGeometry(geometry);
	comboBox->setEditable(false);
	comboBox->setFont(this->getFont());
	comboBox->setPalette(this->getPal());
	comboBox->setFrame(false);
	comboBox->setMaxVisibleItems(itemsCount);
	comboBox->view()->setMinimumWidth( maxLenght + 10 );
	
	QObject::connect(comboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(valueChanged(int)));
};

void stompBox::setComboBoxCurrentIndex(int index)
{
	comboBox->setCurrentIndex(index);
};

void stompBox::setKnob1(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	knob1 = new customDial(0, 0, range, 1, 10, QPoint::QPoint(6, 9), this, hex1, hex2, hex3);
};

void stompBox::setKnob2(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	knob2 = new customDial(0, 0, range, 1, 10, QPoint::QPoint(53, 9), this, hex1, hex2, hex3);
};

void stompBox::setSlider1(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	slider1 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(8, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider2(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	slider2 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(24, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider3(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	slider3 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(40, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider4(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
	slider4 = new customSlider(0, 0, range, 1, 10, QPoint::QPoint(56, 17), this, hex1, hex2, hex3);
};

void stompBox::setSlider5(QString hex1, QString hex2, QString hex3)
{	
	MidiTable *midiTable = MidiTable::Instance();
	int range = midiTable->getRange("Stucture", hex1, hex2, hex3);
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
	switchbutton = new customSwitch(false, QPoint::QPoint(5, 41), this, hex1, hex2, hex3);	
};

QList<QString> stompBox::getSourceItems(QString hex1, QString hex2)
{
	SysxIO *sysxIO = SysxIO::Instance();
	QList<QString> items = sysxIO->getFileSource(hex1, hex2);	
	return items; 
};

void stompBox::updateComboBox(QString hex1, QString hex2, QString hex3)
{	
	setComboBoxCurrentIndex(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateKnob1(QString hex1, QString hex2, QString hex3)
{	
	knob1->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateKnob2(QString hex1, QString hex2, QString hex3)
{	
	knob2->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider1(QString hex1, QString hex2, QString hex3)
{	
	slider1->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider2(QString hex1, QString hex2, QString hex3)
{	
	slider2->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider3(QString hex1, QString hex2, QString hex3)
{	
	slider3->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider4(QString hex1, QString hex2, QString hex3)
{	
	slider4->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateSlider5(QString hex1, QString hex2, QString hex3)
{	
	slider5->setValue(getSourceValue(hex1, hex2, hex3));
};

void stompBox::updateButton(QString hex1, QString hex2, QString hex3)
{
	int value = getSourceValue(hex1, hex2, hex3);
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
	int value = getSourceValue(hex1, hex2, hex3);
	switchbutton->setValue((value==1)?true:false);
};

void stompBox::valueChanged(int value, QString hex1, QString hex2, QString hex3)
{
	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", hex1, hex2, hex3);
	QString fxName, valueName;
	if(hex1 == "0E") // NoiseSuppressor is part of MASTER -> correcting the name for consistency.
	{
		fxName = "Noise Suppressor";
		valueName = items.desc.remove("NS :");
	}
	else
	{
		fxName = midiTable->getMidiMap("Stucture", hex1).name;
		valueName = items.desc;
	};
	QString valueHex = QString::number(value, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");
	QString valueStr = midiTable->getValue("Stucture", hex1, hex2, hex3, valueHex);
	emit valueChanged(fxName, valueName, valueStr);

	SysxIO *sysxIO = SysxIO::Instance(); bool ok;
	if(midiTable->isData("Stucture", hex1, hex2, hex3))
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
};

void stompBox::valueChanged(bool value, QString hex1, QString hex2, QString hex3)
{
	int valueInt;
	(value)? valueInt=1: valueInt=0;
	QString valueHex = QString::number(valueInt, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");
	
	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(hex1, hex2, hex3, valueHex);
};

void stompBox::valueChanged(int index)
{
	QString valueHex = QString::number(index, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");
	
	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(this->hex1, this->hex2, this->hex3, valueHex);

	MidiTable *midiTable = MidiTable::Instance();
	Midi items = midiTable->getMidiMap("Stucture", this->hex1, this->hex2, this->hex3);

	QString desc = items.level.at(index).desc;
	QString longdesc = items.level.at(index).longdesc;
	if(longdesc.isEmpty())
	{
		longdesc = desc;
	};

	this->comboBox->setCurrentIndex(index);
	this->comboBox->setEditText(desc);
};

int stompBox::getSourceValue(QString hex1, QString hex2, QString hex3)
{
	MidiTable *midiTable = MidiTable::Instance();

	bool ok;
	int value; int dataOffset = 11;
	QList<QString> items = getSourceItems(hex1, hex2);
	if(midiTable->isData("Stucture", hex1, hex2, hex3))
	{
		int maxRange = QString("7F").toInt(&ok, 16) + 1;
		int listindex = dataOffset + QString(hex3).toInt(&ok, 16);
		int valueData1 = items.at(listindex).toInt(&ok, 16);
		int valueData2 = items.at(listindex + 1).toInt(&ok, 16);
		value = (valueData1 * maxRange) + valueData2;
	}
	else
	{
		value = items.at(dataOffset + QString(hex3).toInt(&ok, 16)).toInt(&ok, 16);
	};
	return value;
};
