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

#include "stompbox_rv.h"
 
stompbox_rv::stompbox_rv(QWidget *parent)
    : stompBox(parent)
{
	/* REVERB */
	setImage(":/images/rv.png");
	setLSB("0D", "00");
	setKnob1("0D", "00", "03");
	setKnob2("0D", "00", "08");
	setComboBox("0D", "00", "02");
	setButton("0D", "00", "00");
	setEditPages();
};

void stompbox_rv::updateSignal()
{
	updateKnob1("0D", "00", "03");
	updateKnob2("0D", "00", "08");
	updateComboBox("0D", "00", "02");
	updateButton("0D", "00", "00");
};

void stompbox_rv::setEditPages()
{
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "0D", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0D", "00", "02");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Reverb");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0D", "00", "03");
	editDetails()->page()->addKnob(0, 3, 1, 1, "0D", "00", "04");
	editDetails()->page()->addKnob(0, 5, 1, 1, "0D", "00", "07");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0D", "00", "05");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0D", "00", "06");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0D", "00", "08");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0D", "00", "09");
	editDetails()->page()->addGroupBox(0, 3, 1, 1);

	editDetails()->addPage();
};

