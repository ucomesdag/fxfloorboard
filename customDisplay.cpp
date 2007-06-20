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

#include <QPainter>
#include "customDisplay.h"

customDisplay::customDisplay(QRect geometry, QWidget *parent)
    : QWidget(parent)
{
	this->geometry = geometry;
	this->font.setFamily("void");
	this->setGeometry(geometry);
	this->setLabelPosition();
}; 

void customDisplay::paintEvent(QPaintEvent *)
{
	/* Resize the main text if needed. */
	if(this->font.family() == "void")
	{
		this->font = this->mainLabel->font();
	}
	else
	{
		this->mainLabel->setFont(this->font);
	};
	
	while(this->mainLabel->geometry().width() < QFontMetrics(this->mainLabel->font()).width(this->mainLabel->text()))
	{
		/* Change the font size to make it fit... */
		QFont font = this->mainLabel->font();
		int fontSize = font.pixelSize();
		font.setPixelSize(fontSize - 1);
		this->mainLabel->setFont(font);
		if (fontSize <= 1)
		{
			break;
		};
	};
	
	/* This paints the background with the border. */
	QPen border;
	border.setWidth(1);
	border.setColor(QColor(150,150,150));

	QPainter painter(this);
	painter.setPen(border);
    painter.setBrush(QColor(0,1,62));
    //painter.drawRoundRect(QRectF(0.0, 0.0, geometry.width()-1, geometry.height()-1), 8, 8);
	painter.drawRect(QRect(0.0, 0.0, geometry.width()-1, geometry.height()-1));
};

void customDisplay::setLabelPosition(bool invert)
{
	int height = this->geometry.height();
	int width = this->geometry.width();

	int marginWidth = 4;
	int marginHeight = 3;

	QRect subGeometry, mainGeometry, htmlGeometry;
	if(invert)
	{
		subGeometry = QRect(marginWidth, marginHeight, width - (marginWidth * 2), (height / 2) - marginHeight);  
		mainGeometry = QRect(marginWidth, (height / 2), width - (marginWidth * 2), (height / 2) - marginHeight);
	}
	else
	{
		mainGeometry = QRect(marginWidth, marginHeight, width - (marginWidth * 2), (height / 2) - marginHeight);  
		subGeometry = QRect(marginWidth, marginHeight + (height / 2), width - (marginWidth * 2), (height / 2) - marginHeight);
	};

	this->mainLabel = new QLabel(this);
	this->mainLabel->setObjectName("displayLarge");
	this->mainLabel->setAlignment(Qt::AlignLeft); 
	this->mainLabel->setGeometry(mainGeometry);

	this->subLabelLeft = new QLabel(this);
	this->subLabelLeft->setObjectName("displaySmall");
	this->subLabelLeft->setAlignment(Qt::AlignLeft);
	this->subLabelLeft->setGeometry(subGeometry);

	this->subLabelRight = new QLabel(this);
	this->subLabelRight->setObjectName("displaySmall");
	this->subLabelRight->setAlignment(Qt::AlignRight);
	this->subLabelRight->setGeometry(subGeometry);
};


void customDisplay::setMainText(QString mainText, Qt::Alignment alignment)
{
	this->mainLabel->setText(mainText);
	this->mainLabel->setAlignment(alignment);
};

void customDisplay::setSubText(QString subTextLeft, QString subTextRight)
{
	this->subLabelLeft->setText(subTextLeft);
	this->subLabelRight->setText(subTextRight);
};

void customDisplay::clearAll()
{
	this->mainLabel->clear();
	this->subLabelLeft->clear();
	this->subLabelRight->clear();
};

void customDisplay::setMainObjectName(QString name)
{
	this->mainLabel->setObjectName(name);
};

void customDisplay::setSubObjectName(QString name)
{
	this->subLabelLeft->setObjectName(name);
	this->subLabelRight->setObjectName(name);
};
