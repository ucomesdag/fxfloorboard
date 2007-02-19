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

#include "statusBarWidget.h"
#include "globalVariables.h"

statusBarWidget::statusBarWidget(QWidget *parent)
    : QWidget(parent)
{
	const int spacing = 4;	
	int xOffset = 0;
	
	this->progressBar = new QProgressBar(this);
	this->progressBar->setTextVisible(false);
	this->progressBar->setFixedSize(80, 13);
	this->progressBar->setRange(0, 100);
	this->progressBar->setValue(0);
	this->progressBar->move(xOffset, 0);

	xOffset += this->progressBar->width() + spacing;

	this->symbol = new statusBarSymbol(this);
	this->symbol->setValue(0);
	this->symbol->move(xOffset, 0);

	xOffset += this->symbol->width() + spacing;

	this->label = new QLabel(this);
	this->label->setFixedWidth(100);
	this->label->setText(tr("Not connected"));
	this->label->move(xOffset, 0);

	xOffset += this->label->width() + spacing;

	this->setFixedWidth(xOffset);
};

void statusBarWidget::setStatusMessage(QString message)
{
	this->label->setText(message);
};

void statusBarWidget::setStatusProgress(int value)
{
	this->progressBar->setValue(value);
};

void statusBarWidget::setStatusSymbol(int value)
{
	this->symbol->setValue(value);
};