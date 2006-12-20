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

#include <QtGui>

#include "customSlider.h"

customSlider::customSlider(double value, double min, double max, double single, double page, 
						QPoint sliderPos, QWidget *parent, 
						QString slideImagePath, QString sliderButtonImagePath, 
						QSize slideSize, QSize sliderButtonSize)
    : QWidget(parent)
{
	this->value = value;
	this->min = min;
	this->max = max;
	this->single = single;
	this->page = page;
	this->slideImagePath = slideImagePath;
	this->sliderButtonImagePath = sliderButtonImagePath;
	this->slideSize = slideSize;
	this->sliderButtonSize = sliderButtonSize;
	this->sliderPos = sliderPos;

	setOffset(value);
    setGeometry(sliderPos.x(), sliderPos.y(), slideSize.width(), slideSize.height());

	QObject::connect(this, SIGNAL( valueChanged(int) ),
                this->parent(), SIGNAL( sliderValue(int) ));
};

void customSlider::paintEvent(QPaintEvent *)
{
	QPixmap slide(slideImagePath);
	QPixmap sliderButton(sliderButtonImagePath);

	QRectF target(0.0 , yOffset, sliderButtonSize.width(), sliderButtonSize.height());
	QRectF source(0.0 , 0.0, sliderButtonSize.width(), sliderButtonSize.height());
	QRectF screen(0.0 , 0.0, slideSize.width(), slideSize.height());

	QPixmap buffer = slide;
	QPainter painterBuffer(&buffer);
	painterBuffer.drawPixmap(target, sliderButton, source);
	painterBuffer.end();

	QPainter painter(this);
	painter.drawPixmap(screen, buffer, screen);
};

void customSlider::setOffset(double _newValue)
{
	double dataRange = max - min;
	double range = slideSize.height()  - sliderButtonSize.height();
	//double result = (0 - min) + (_newValue / (dataRange / range));
	double result = (_newValue - min) * (range / dataRange);
	
	this->value = _newValue;	
	this->yOffset = result;
	this->update();
	setValue(_newValue);
};

void customSlider::setOffset(QPoint pos)
{
	this->yOffset = pos.y();
	this->update();
	setValue(this->value);
};

void customSlider::mouseTrigger(QPoint mousePos)
{
	this->_lastValue = value;

	double dataRange = max - min;
	double range = slideSize.height() - sliderButtonSize.height();

	double _newValue = (double)(mousePos.y() - (sliderButtonSize.height() * 2) * (range / dataRange)) *  (dataRange / range);

	QPoint buttonCenter = QPoint(0, (sliderButtonSize.height()/2));
	QPoint relativePos = mousePos - buttonCenter;
	int minY = 0;
	int maxY = slideSize.height() - sliderButtonSize.height();
	if( relativePos.y() > minY && relativePos.y() < maxY )
	{
		setOffset(relativePos);
		this->value = _newValue;
	} 
	else if(relativePos.y() <= minY)
	{
		setOffset(QPoint(0, minY));
		this->value = min;
	}
	else if(relativePos.y() >= maxY)
	{
		setOffset(QPoint(0, maxY));
		this->value = max;
	};
};

void customSlider::mousePressEvent(QMouseEvent *event)
{
	if ( event->button() == Qt::LeftButton )
	{	
		setFocus();
		mouseTrigger(event->pos());
	};
};

void customSlider::mouseMoveEvent(QMouseEvent *event)
{
	mouseTrigger(event->pos());
};

void customSlider::wheelEvent(QWheelEvent *event)
{
    double numDegrees = (double)(event->delta() / 8);
    double numSteps = numDegrees / 15;

    if (event->orientation() == Qt::Vertical) 
	{
		this->_lastValue = value;
		
		double _newValue = _lastValue - (numSteps * single);

		if(_newValue < min)
		{
			_newValue = min;
		}
		else if(_newValue > max)
		{
			_newValue = max;
		};
		setOffset(_newValue);	
    };
};

void customSlider::keyPressEvent(QKeyEvent *event)
{
	double numSteps = 0;
	this->_lastValue = value;
	
	switch(event->key())
	{
		case Qt::Key_Up: numSteps = single;break;
		case Qt::Key_Down: numSteps = -single;break;
		case Qt::Key_Plus: numSteps = single;break;
		case Qt::Key_Minus: numSteps = -single;break;

		case Qt::Key_PageUp: numSteps = page;break;
		case Qt::Key_PageDown: numSteps = -page;break;

		case Qt::Key_Right: numSteps = -(max-min);break;
		case Qt::Key_Left: numSteps = max-min;break;
	};

	if (numSteps!=0) 
	{
		double _newValue = _lastValue - numSteps;
		if(_newValue < min)
		{
			_newValue = min;
			this->_lastValue = value;
		}
		else if(_newValue > max)
		{
			_newValue = max;
			this->_lastValue = value;
		};
		setOffset(_newValue);
	};
};

void customSlider::setValue(double value)
{
    if (value != m_value) {
        this->m_value = value;
		emit valueChanged((int)(value));
    };
};