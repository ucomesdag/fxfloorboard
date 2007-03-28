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

#include "stompbox_fx1.h"

stompbox_fx1::stompbox_fx1(QWidget *parent)
    : stompBox(parent)
{
	/* FX1 */
	setImage(":/images/fx1.png");
	setLSB("00", "00");
	setComboBox("00", "00", "02", QRect(8, 31, 79, 13));
	setButton("00", "00", "00");
	setEditPages();
};

void stompbox_fx1::updateSignal()
{
	updateComboBox("00", "00", "02");
	updateButton("00", "00", "00");
};

void stompbox_fx1::setEditPages()
{
	editDetails()->page()->addLabel("Pre Amp/SP A", QPoint(0,0));
	editDetails()->page()->addKnob(QPoint(0,20));
	editDetails()->page()->addKnob(QPoint(45,20));
	editDetails()->page()->addKnob(QPoint(90,20));
	editDetails()->page()->addKnob(QPoint(135,20));
	editDetails()->page()->addKnob(QPoint(180,20));
	editDetails()->page()->addSwitch(QPoint(230,25));
	editDetails()->addPage();
	editDetails()->page()->addLabel("PAGE 2", QPoint(0,0));
	editDetails()->addPage();
	editDetails()->page()->addLabel("PAGE 3", QPoint(0,0));
	editDetails()->addPage();
	editDetails()->page()->addLabel("PAGE 4", QPoint(0,0));
	editDetails()->addPage();
};
