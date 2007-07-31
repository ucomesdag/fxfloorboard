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

#include "stompbox_fx2.h"

stompbox_fx2::stompbox_fx2(QWidget *parent)
    : stompBox(parent)
{
	/* FX2 */
	setImage(":/images/fx2.png");
	setLSB("09", "00");
	setComboBox("09", "00", "02", QRect(8, 31, 79, 13));
	setButton("09", "00", "00");
	setEditPages();
};

void stompbox_fx2::updateSignal()
{
	updateComboBox("09", "00", "02");
	updateButton("09", "00", "00");
};

void stompbox_fx2::setEditPages()
{
    /* ACS */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "03");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Advanced Compressor");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "04");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "05");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "06");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "07");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "00");

	/* LM */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "08");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Limitter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "09");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "0A");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "0B");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "0C");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "0D");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "01");

	/* TW */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(0, 1, 1, 1, "09", "00", "0F", "bottom", Qt::AlignRight);
	editDetails()->page()->addComboBox(1, 0, 1, 2, "09", "00", "0E");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Touch Wah");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "10");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "11");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "12");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "14");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "13");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "02");

	/* AW */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "15");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Auto Wah");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "18", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "16");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "17");
	editDetails()->page()->addKnob(0, 4, 1, 1, "09", "00", "19");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "1B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "1A");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "03");

	/* TM */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "1C");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Tone Modify");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "1D");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "1E");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "1F");;
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "20");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "04");

	/* GS */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "21");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Guitar Simulator");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "22");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "23");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "24");;
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "25");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "05");

	/* TR */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Tremelo");
	editDetails()->page()->newGroupBox("Wave");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "26");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "27", "normal","right", 105);
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "28");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "06");

	/* PH */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "29");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Phaser");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "2A", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "09", "00", "2E", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "2B");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "2C");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "2D");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "2F");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "30");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "07");

	/* FL */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Flanger");
	editDetails()->page()->addKnob(0, 0, 1, 3, "09", "00", "31", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "09", "00", "32");
	editDetails()->page()->addKnob(1, 1, 1, 1, "09", "00", "33");
	editDetails()->page()->addKnob(1, 2, 1, 1, "09", "00", "34");
	editDetails()->page()->addKnob(1, 3, 1, 1, "09", "00", "35");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "36");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "37");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "38");
	editDetails()->page()->addGroupBox(0, 3, 1, 1);
	editDetails()->addPage("09", "00", "02", "08");

	/* PN */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Pan");
	editDetails()->page()->newGroupBox("Wave");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "39");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "3A", "normal","right", 105);
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "3B");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "09");

	/* VB */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Vibrato");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "3C", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "3D");

	editDetails()->page()->newGroupBox("Trigger");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "3E", "middle", Qt::AlignCenter);
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "3F");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "0A");

	/* UV */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Uni-V");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "40", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "41");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "42");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "0B");

	/* RM */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "43");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Ring Modulator");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "44");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "45");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "46");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "0C");

	/* SG */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Slow Gear");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "47");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "48");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "0D");

	/* DF */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Defretter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "49");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "4A");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "4B");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "4C");
	editDetails()->page()->addKnob(0, 4, 1, 1, "09", "00", "4D");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "4E");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "4F");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "0E");

	/* STR */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Sitar");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "50");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "51");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "52");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "53");
	editDetails()->page()->addKnob(0, 4, 1, 1, "09", "00", "54");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "55");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "56");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "0F");

	/* FB */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->newStackControl(0);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "57");
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->insertStackField(0, 0, 1, 2, 1);
	/* OSC */
	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Feedbacker");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "58");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "5A");

	editDetails()->page()->newGroupBox("Hi Oct");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "59");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "5B");
	editDetails()->page()->addGroupBox(1, 0, 1, 2);

	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "5C", "normal","right", 105);
	editDetails()->page()->addKnob(1, 2, 1, 1, "09", "00", "5D");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	/* NATURAL */
	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Feedbacker");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "5A");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->addPage("09", "00", "02", "10");

	/* AFB */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Anti-Feedback");

	editDetails()->page()->newGroupBox("Frequency 1");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "5E");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "5F");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Frequency 2");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "60");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "61");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Frequency 3");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "62");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "63");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "11");

	/* HU */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "64");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Humanizer");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "09", "00", "65");
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "66");
	editDetails()->page()->addKnob(0, 1, 1, 3, "09", "00", "68", "normal","right", 105);
	editDetails()->page()->addKnob(1, 1, 1, 1, "09", "00", "67");
	editDetails()->page()->addKnob(1, 2, 1, 1, "09", "00", "69");
	editDetails()->page()->addKnob(1, 3, 1, 1, "09", "00", "6A");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "6B");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "12");

	/* SL */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Slicer");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "6C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "6D", "normal","right", 105);
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "6E");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "13");

	/* WSY */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 1, "09", "00", "6F");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Wave Synth");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "70");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "71");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "72");
	editDetails()->page()->addKnob(0, 3, 1, 1, "09", "00", "73");
	editDetails()->page()->addKnob(0, 4, 1, 1, "09", "00", "74");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "75");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "76");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "14");

	/* SEQ */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Equalizer");
	editDetails()->page()->newGroupBox("Low");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "77");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "78");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Low-Middle");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "79");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "7A");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "7B");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("High-Middle");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "7C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "09", "00", "7D");
	editDetails()->page()->addKnob(0, 2, 1, 1, "09", "00", "7E");
	editDetails()->page()->addGroupBox(1, 1, 1, 1);

	editDetails()->page()->newGroupBox("High");
	editDetails()->page()->addKnob(0, 0, 1, 1, "09", "00", "7F");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "00");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "01");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "15");

	/* FX 2 ONLY */

	/* HR */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->newStackControl(0);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0A", "00", "02");	
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->insertStackField(0, 0, 1, 3, 1);

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Harmonist");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "03", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "04", "normal","right", 105);
	editDetails()->page()->addGroupBox(0, 0, 2, 1);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0C", "normal","right", 50);
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "06");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "07");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0D");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Harmonist");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "03", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "04", "normal","right", 105);
	editDetails()->page()->addGroupBox(0, 0, 2, 1);
	editDetails()->page()->newGroupBox("Voice 2");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "08", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "09", "normal","right", 105);
	editDetails()->page()->addGroupBox(3, 0, 1, 1);

	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0C", "normal","right", 50);
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "06");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "07");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "0B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0D");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Harmonist");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "03", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "04", "normal","right", 105);
	editDetails()->page()->addGroupBox(0, 0, 2, 1);
	editDetails()->page()->newGroupBox("Voice 2");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "08", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "09", "normal","right", 105);
	editDetails()->page()->addGroupBox(3, 0, 1, 1);

	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0C", "normal","right", 50);
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "06");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "07");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "0B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "0D");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();
	editDetails()->addPage("09", "00", "02", "16");

	/* PS */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->newStackControl(0);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0A", "00", "0E");	
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->insertStackField(0, 0, 1, 3, 1);

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Pitch Shifter");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "0F");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "12", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "10");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "11");
	editDetails()->page()->addGroupBox(0, 0, 3, 1);
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "14");
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "15");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "1C");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Pitch Shifter");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "0F");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "19", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "17");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "18");
	editDetails()->page()->addGroupBox(0, 0, 3, 1);
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "14");

	editDetails()->page()->newGroupBox("Voice 2");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "16");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "12", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "10");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "11");
	editDetails()->page()->addGroupBox(4, 0, 1, 1);
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "15");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "1B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "1C");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Pitch Shifter");
	editDetails()->page()->newGroupBox("Voice 1");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "0F");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "19", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "17");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "18");
	editDetails()->page()->addGroupBox(0, 0, 3, 1);

	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "14");

	editDetails()->page()->newGroupBox("Voice 2");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "16");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "12", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "10");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "11");
	editDetails()->page()->addGroupBox(4, 0, 1, 1);
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "15");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "1B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "1C");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->page()->addStackField();
	editDetails()->addPage("09", "00", "02", "17");

	/* PB */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Pedal Bend");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "1D");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "1E");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "1F");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "20");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "21");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "18");

	/* OC */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Octave", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addComboBox(0, 0, 1, 3, "0A", "00", "22");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "23");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "24");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "19");

	/* RT */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 3, "0A", "00", "25");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Rotary");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "26", "normal","right", 105);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "27", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "28");
	editDetails()->page()->addKnob(0, 2, 2, 1, "0A", "00", "29");
	editDetails()->page()->addKnob(0, 3, 2, 1, "0A", "00", "2A");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);
	editDetails()->addPage("09", "00", "02", "1A");

	/* 2CE */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("2x2 Chorus");
	editDetails()->page()->newGroupBox("Low");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "2C", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "2D");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "2E");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("High");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "30", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "31");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "32");
	editDetails()->page()->addGroupBox(1, 0, 1, 1);
	editDetails()->page()->addKnob(0, 1, 2, 1, "0A", "00", "2B", "normal","right", 63);
	editDetails()->page()->addGroupBox(0, 1, 3, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "2F");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "33");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);

	editDetails()->addPage("09", "00", "02", "1B");

	/* AR */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Auto Riff");

	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "34", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "37");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "39");
	editDetails()->page()->addKnob(0, 3, 1, 1, "0A", "00", "38", "normal","right", 50);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "36", "normal","right", 105);
	editDetails()->page()->addSwitch(1, 2, 1, 1, "0A", "00", "35", "middle", Qt::AlignCenter);
	editDetails()->page()->addSwitch(1, 3, 1, 1, "0A", "00", "3A", "middle", Qt::AlignCenter);
	editDetails()->page()->addGroupBox(0, 1, 2, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "3B");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "3C");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "1C");

	/* SYN */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->newStackControl(0);
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0A", "00", "3E");
	editDetails()->page()->addStackControl();
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->insertStackField(0, 0, 1, 2, 1);

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Guitar Synth");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "40");
	editDetails()->page()->addSwitch(0, 1, 1, 1, "0A", "00", "3F", "middle", Qt::AlignCenter);
	editDetails()->page()->addSwitch(0, 2, 1, 1, "0A", "00", "4B", "middle", Qt::AlignCenter);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "3D");
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "43");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0A", "00", "44");
	editDetails()->page()->addKnob(2, 0, 1, 1, "0A", "00", "48");
	editDetails()->page()->addKnob(2, 1, 1, 1, "0A", "00", "49");
	editDetails()->page()->addKnob(2, 2, 1, 1, "0A", "00", "4A");
	editDetails()->page()->addGroupBox(0, 0, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "45");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "46");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "47");
	editDetails()->page()->addGroupBox(0, 1, 1, 2);

	editDetails()->page()->newGroupBox("PWM");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "41");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "42");
	editDetails()->page()->addGroupBox(1, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "4C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "4D");
	editDetails()->page()->addGroupBox(0, 4, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Guitar Synth");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0A", "00", "40");
	editDetails()->page()->addSwitch(0, 1, 1, 1, "0A", "00", "3F", "middle", Qt::AlignCenter);
	editDetails()->page()->addSwitch(0, 2, 1, 1, "0A", "00", "4B", "middle", Qt::AlignCenter);
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "3D");
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "43");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0A", "00", "44");
	editDetails()->page()->addKnob(2, 0, 1, 1, "0A", "00", "48");
	editDetails()->page()->addKnob(2, 1, 1, 1, "0A", "00", "49");
	editDetails()->page()->addKnob(2, 2, 1, 1, "0A", "00", "4A");
	editDetails()->page()->addGroupBox(0, 0, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "45");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "46");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "47");
	editDetails()->page()->addGroupBox(0, 1, 1, 2);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "4C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "4D");
	editDetails()->page()->addGroupBox(0, 4, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Guitar Synth");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "3D");
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "43");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0A", "00", "44");
	editDetails()->page()->addKnob(2, 0, 1, 1, "0A", "00", "48");
	editDetails()->page()->addKnob(2, 1, 1, 1, "0A", "00", "49");
	editDetails()->page()->addKnob(2, 2, 1, 1, "0A", "00", "4A");
	editDetails()->page()->addGroupBox(0, 0, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "45");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "46");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "47");
	editDetails()->page()->addGroupBox(0, 1, 1, 2);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "4C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "4D");
	editDetails()->page()->addGroupBox(0, 4, 1, 1);
	editDetails()->page()->addStackField();

	editDetails()->page()->newStackField(0);
	editDetails()->page()->newGroupBox("Guitar Synth");
	editDetails()->page()->addKnob(1, 0, 1, 1, "0A", "00", "3D");
	editDetails()->page()->addKnob(1, 1, 1, 1, "0A", "00", "43");
	editDetails()->page()->addKnob(1, 2, 1, 1, "0A", "00", "44");
	editDetails()->page()->addKnob(2, 0, 1, 1, "0A", "00", "48");
	editDetails()->page()->addKnob(2, 1, 1, 1, "0A", "00", "49");
	editDetails()->page()->addKnob(2, 2, 1, 1, "0A", "00", "4A");
	editDetails()->page()->addGroupBox(0, 0, 2, 1);

	editDetails()->page()->newGroupBox("Filter");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "45");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "46");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "47");
	editDetails()->page()->addGroupBox(0, 1, 1, 2);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "4C");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "4D");
	editDetails()->page()->addGroupBox(0, 4, 1, 1);
	editDetails()->page()->addStackField();
	editDetails()->addPage("09", "00", "02", "1D");
	
	/* AC */
	editDetails()->page()->newGroupBox("Effect");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addComboBox(1, 0, 1, 1, "0A", "00", "4E");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Acoustic Processor");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "4F");
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "50");
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "51");
	editDetails()->page()->addKnob(0, 3, 1, 1, "0A", "00", "52");
	editDetails()->page()->addKnob(0, 4, 1, 1, "0A", "00", "53");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "54");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "1E");

	/* SH */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Sound Hold");
	editDetails()->page()->addSwitch(0, 0, 1, 1, "0A", "00", "55", "middle", Qt::AlignTop | Qt::AlignHCenter);	
	editDetails()->page()->addKnob(0, 2, 1, 1, "0A", "00", "56");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "57");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "1F");

	/* SDD */
	editDetails()->page()->newGroupBox("Effect", Qt::AlignTop | Qt::AlignHCenter);
	editDetails()->page()->addSwitch(0, 0, 1, 1, "09", "00", "00");	
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->page()->newGroupBox("Sub Delay");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "58", "normal","right", 105);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0A", "00", "5A");
	editDetails()->page()->addGroupBox(0, 1, 1, 1);

	editDetails()->page()->newGroupBox("Level");
	editDetails()->page()->addKnob(0, 0, 1, 1, "0A", "00", "5B");
	editDetails()->page()->addGroupBox(0, 2, 1, 1);
	editDetails()->addPage("09", "00", "02", "20");
};
