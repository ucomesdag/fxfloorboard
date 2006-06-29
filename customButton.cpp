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

#include <QtGui>

#include "customButton.h"

customButton::customButton(bool active, QPoint buttonPos, QWidget *parent,
						   QString imagePath, QSize buttonSize, unsigned int imageRange)
    : QWidget(parent)
{
	this->active = active;
	this->imagePath = imagePath;
	this->buttonSize = buttonSize;
	this->imageRange = imageRange;
	this->buttonPos = buttonPos;
	setOffset(0);
    setGeometry(buttonPos.x(), buttonPos.y(), buttonSize.width(), buttonSize.height());
};

void customButton::paintEvent(QPaintEvent *)
{
	QRectF target(0.0 , 0.0, buttonSize.width(), buttonSize.height());
	QRectF source(0.0, yOffset, buttonSize.width(), buttonSize.height());
	QPixmap image(imagePath);

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
};

void customButton::setOffset(signed int imageNr)
{
	this->yOffset = imageNr*buttonSize.height();
	this->update();
};

void customButton::mousePressEvent(QMouseEvent *event)
{
	if ( event->button() == Qt::LeftButton )
	{	
		this->dragStartPosition = event->pos();
		if(active)
		{
			setOffset(3);
		}
		else
		{
			setOffset(1);
		};
		setFocus();
	};
};

void customButton::mouseReleaseEvent(QMouseEvent *event)
{
	if ( event->button() == Qt::LeftButton )
	{	
		if(active)
		{
			this->active = false;
		}
		else
		{
			this->active = true;
		};
		if(active)
		{
			setOffset(2);
		}
		else
		{
			setOffset(0);
		};
		clearFocus();
		setValue(active);
	};
};

void customButton::setValue(bool value)
{
    if (value != m_value) {
        this->m_value = value;
        emit valueChanged((int)value);
    };
};

void customButton::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() == Qt::LeftButton) && (event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
	{
        return;
	};
    
	
	/*if(active)
	{
		setOffset(2);
	}
	else
	{
		setOffset(0);
	};*/
};