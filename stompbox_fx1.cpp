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
	editDetails()->page()->newGroupBox("GROUP A");

	editDetails()->page()->newGroupBox("Pre Amp/SP A");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "0B");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Pre Amp/SP B");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(1, 0, 1, 1);

	editDetails()->page()->newGroupBox("Pre Amp/SP C");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(2, 0, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->addPage();


	editDetails()->page()->newGroupBox("Pre Amp/SP A");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "0B");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Pre Amp/SP B");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(1, 0, 1, 1);
	editDetails()->addPage();


	editDetails()->page()->newGroupBox("GROUP A");

	editDetails()->page()->newGroupBox("Pre Amp/SP A");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "0B");

	editDetails()->page()->newGroupBox("Pre Amp/SP B");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(1, 0, 1, 5);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->addPage();


	editDetails()->page()->newGroupBox("GROUP A");

	editDetails()->page()->newGroupBox("Pre Amp/SP A");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "0B");

	editDetails()->page()->newGroupBox("Pre Amp/SP B");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(1, 0, 1, 5);

	editDetails()->page()->newGroupBox("Pre Amp/SP C");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "1C");
	editDetails()->page()->addGroupBox(2, 0, 1, 5);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->addPage();

	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(0, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(0, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(0, 4, 1, 1, "07", "00", "0B");

	editDetails()->page()->addKnob(1, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(1, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(1, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(1, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(1, 4, 1, 1, "07", "00", "1C");

	editDetails()->addPage();
};
