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

#include "stompbox_wah.h"

stompbox_wah::stompbox_wah(QWidget *parent)
    : stompBox(parent)
{
	/* WAH */
	setImage(":/images/wah.png");
	setLSB("03", "00");
	setComboBox("03", "00", "02");
	setButton("03", "00", "00", QPoint::QPoint(0, 109), ":/images/pedal.png");
	setEditPages();
};

void stompbox_wah::updateSignal()
{
	updateComboBox("03", "00", "02");
	updateButton("03", "00", "00");
};

void stompbox_wah::setEditPages()
{
    editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "03", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "03", "00", "02");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Wah");
	editDetails()->page()->addKnob(0, 0, 1, 1, "03", "00", "03");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "03", "00", "04");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->addPage();	
};
