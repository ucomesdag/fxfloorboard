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

#include "stompbox_pre.h"

stompbox_pre::stompbox_pre(QWidget *parent)
    : stompBox(parent)
{
	/* PRE */
	setImage(":/images/pre.png");
	setLSB("07", "00");
	setSwitch("07", "00", "00");
	setEditPages();
};

void stompbox_pre::updateSignal()
{
	updateSwitch("07", "00", "00");
};

void stompbox_pre::setEditPages()
{
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "00");

	editDetails()->page()->newStackControl(0);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "07", "00", "03");
	editDetails()->page()->addStackControl();

	editDetails()->page()->newStackControl(1);
	editDetails()->page()->addComboBox(2, 0, 1, 1, "07", "00", "02");
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	/* TYPE SETTINGS */
	editDetails()->page()->insertStackField(1, 1, 0, 1, 1);

	editDetails()->page()->newStackField(1);
	editDetails()->page()->addStackField();
	
	editDetails()->page()->newStackField(1, Qt::AlignCenter);
	editDetails()->page()->newGroupBox("Ch.Delay");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "05");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(1, Qt::AlignCenter);
	editDetails()->page()->newGroupBox("Ch.Delay");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "05");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(1, Qt::AlignCenter);
	editDetails()->page()->newGroupBox("Dynamic");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "04");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	/* CHANNEL A */
	editDetails()->page()->insertStackField(0, 0, 1, 2, 1);
	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Channel A");

	editDetails()->page()->newGroupBox("Pre Amp");
	editDetails()->page()->newStackControl(2);
	editDetails()->page()->addComboBox(0, 0, 1, 5, "07", "00", "06");
	editDetails()->page()->addStackControl();
	editDetails()->page()->addComboBox(0, 5, 1, 1, "07", "00", "0E", "bottom", Qt::AlignHCenter);
	
	editDetails()->page()->addKnob(2, 0, 1, 1, "07", "00", "07", "turbo");
	editDetails()->page()->addKnob(2, 1, 1, 1, "07", "00", "08");
	editDetails()->page()->addKnob(2, 2, 1, 1, "07", "00", "09");
	editDetails()->page()->addKnob(2, 3, 1, 1, "07", "00", "0A");
	editDetails()->page()->addKnob(2, 4, 1, 1, "07", "00", "0B");

	editDetails()->page()->insertStackField(2, 2, 5, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "0C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "16");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Speaker");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "07", "00", "11");
	editDetails()->page()->addComboBox(0, 1, 1, 1, "07", "00", "13", "bottom", Qt::AlignRight);
	editDetails()->page()->addComboBox(1, 0, 1, 2, "07", "00", "12");
	editDetails()->page()->addKnob(0, 2, 2, 1, "07", "00", "14");
	editDetails()->page()->addKnob(0, 3, 2, 1, "07", "00", "15");
	editDetails()->page()->addGroupBox(1, 0, 1, 1);

	editDetails()->page()->newGroupBox("Solo");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0F", "middle", Qt::AlignLeft | Qt::AlignTop);
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "10");
	editDetails()->page()->addGroupBox(1, 1, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	/* CHANNEL B */
	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Channel B");

	editDetails()->page()->newGroupBox("Pre Amp");
	editDetails()->page()->newStackControl(3);
	editDetails()->page()->addComboBox(0, 0, 1, 5, "07", "00", "17");
	editDetails()->page()->addStackControl();

	editDetails()->page()->addComboBox(0, 5, 1, 1, "07", "00", "1F", "bottom", Qt::AlignHCenter);
	
	editDetails()->page()->addKnob(2, 0, 1, 1, "07", "00", "18", "turbo");
	editDetails()->page()->addKnob(2, 1, 1, 1, "07", "00", "19");
	editDetails()->page()->addKnob(2, 2, 1, 1, "07", "00", "1A");
	editDetails()->page()->addKnob(2, 3, 1, 1, "07", "00", "1B");
	editDetails()->page()->addKnob(2, 4, 1, 1, "07", "00", "1C");

	editDetails()->page()->insertStackField(3, 2, 5, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "07", "00", "1D");
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "27");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Speaker");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "07", "00", "22");
	editDetails()->page()->addComboBox(0, 1, 1, 1, "07", "00", "24", "bottom", Qt::AlignRight);
	editDetails()->page()->addComboBox(1, 0, 1, 2, "07", "00", "23");
	editDetails()->page()->addKnob(0, 2, 2, 1, "07", "00", "25");
	editDetails()->page()->addKnob(0, 3, 2, 1, "07", "00", "26");
	editDetails()->page()->addGroupBox(1, 0, 1, 1);

	editDetails()->page()->newGroupBox("Solo");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "20", "middle", Qt::AlignLeft | Qt::AlignTop);
	editDetails()->page()->addKnob(0, 1, 1, 1, "07", "00", "21");
	editDetails()->page()->addGroupBox(1, 1, 1, 1);

	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(2);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "0D", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();
	editDetails()->page()->newStackField(3);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "07", "00", "1E", "middle", Qt::AlignCenter);
	editDetails()->page()->addStackField();

	editDetails()->addPage();	
};
