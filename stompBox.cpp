/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8: FX FloorBoard".
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

stompBox::stompBox(QWidget *parent, unsigned int id, QString imagePath, QPoint stompPos, QSize stompSize)
    : QWidget(parent)
{
	this->id = id;
	this->imagePath = imagePath;
	this->stompSize = stompSize;
	this->stompPos = stompPos;	

	this->setFixedSize(stompSize);

	QObject::connect(this, SIGNAL( knobValue(int) ),
                this->parent(), SIGNAL( knobValue(int) ));
	
	QObject::connect(this->parent(), SIGNAL( updateStompOffset(signed int) ),
                this, SLOT( updatePos(signed int) ));
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
	return id;
};