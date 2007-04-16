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

#include "stompbox_ns.h"

stompbox_ns::stompbox_ns(QWidget *parent)
    : stompBox(parent)
{
	/* NS */
	setImage(":/images/ns.png");
	setLSB("0E", "00");
	setKnob1("0E", "00", "01");
	setKnob2("0E", "00", "02");
	setButton("0E", "00", "00");
	setEditPages();
};

void stompbox_ns::updateSignal()
{
	updateKnob1("0E", "00", "01");
	updateKnob2("0E", "00", "02");
	updateButton("0E", "00", "00");
};

void stompbox_ns::setEditPages()
{
   editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "0E", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0E", "00", "03");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Noise Suppressor");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0E", "00", "01");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0E", "00", "02");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage();	
};
