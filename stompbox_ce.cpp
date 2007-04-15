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

#include "stompbox_ce.h"

stompbox_ce::stompbox_ce(QWidget *parent)
    : stompBox(parent)
{
	/* Chorus */
	setImage(":/images/ce.png");
	setLSB("0C", "00");
	setKnob1("0C", "00", "03");
	setKnob2("0C", "00", "04");
	setComboBox("0C", "00", "02");
	setButton("0C", "00", "00");
	setEditPages();
};

void stompbox_ce::updateSignal()
{
	updateKnob1("0C", "00", "03");
	updateKnob2("0C", "00", "04");
	updateComboBox("0C", "00", "02");
	updateButton("0C", "00", "00");
};

void stompbox_ce::setEditPages()
{
    editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "0C", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0C", "00", "02");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Chorus");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0C", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0C", "00", "04");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0C", "00", "05");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0C", "00", "06");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0C", "00", "07");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0C", "00", "08");
	editDetails()->page()->addGroupBox(0, 3, 1, 1);

	editDetails()->addPage();	
};
