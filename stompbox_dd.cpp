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

#include "stompbox_dd.h"
#include "MidiTable.h"
#include "SysxIO.h"

#include "customButton.h"
#include "customLed.h"
#include "customDial.h"

stompbox_dd::stompbox_dd(QWidget *parent)
    : stompBox(parent)
{
	/* DELAY */
	setImage(":/images/dd.png");
	setLSB("0B", "00");
	setKnob1("0B", "00", "03");
	setKnob2("0B", "00", "06");
	setComboBox("0B", "00", "02");
	setButton("0B", "00", "00");
	setEditPages();
};

void stompbox_dd::updateSignal()
{
	updateKnob1("0B", "00", "03");
	updateKnob2("0B", "00", "06");
	updateComboBox("0B", "00", "02");
	updateButton("0B", "00", "00");
};

void stompbox_dd::setEditPages()
{
    editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "0B", "00", "00");
	editDetails()->page()->newStackControl(0, 0, 1, 2, 1);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0B", "00", "02");
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("0");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("1");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("2");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("3");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("4");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("5");
	editDetails()->page()->addKnob(0, 0, 1, 3, "0B", "00", "03", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0B", "00", "05");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0B", "00", "06");
	editDetails()->page()->addGroupBox(0, 0, 0, 1);
	editDetails()->page()->addStackField();

	/*editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0B", "00", "06");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0B", "00", "07");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0B", "00", "08");
	editDetails()->page()->addGroupBox(0, 3, 1, 1);*/

	editDetails()->addPage();	
};
