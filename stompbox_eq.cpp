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

#include "stompbox_eq.h"

stompbox_eq::stompbox_eq(QWidget *parent)
    : stompBox(parent)
{
	/* EQ */
	setImage(":/images/eq.png");

	/*int range1 = midiTable->getRange("Stucture", "08", "00", "03");
	int range2 = midiTable->getRange("Stucture", "08", "00", "06");
	int range3 = midiTable->getRange("Stucture", "08", "00", "09");
	int range4 = midiTable->getRange("Stucture", "08", "00", "0A");
	int range5 = midiTable->getRange("Stucture", "08", "00", "0C");

	customSlider *slider1 = new customSlider(0, 0, range1, 1, 10, QPoint::QPoint(8, 17), this, "08", "00", "03");
	customSlider *slider2 = new customSlider(0, 0, range2, 1, 10, QPoint::QPoint(24, 17), this, "08", "00", "06");
	customSlider *slider3 = new customSlider(0, 0, range3, 1, 10, QPoint::QPoint(40, 17), this, "08", "00", "09");
	customSlider *slider4 = new customSlider(0, 0, range4, 1, 10, QPoint::QPoint(56, 17), this, "08", "00", "0A");	
	customSlider *slider5 = new customSlider(0, 0, range5, 1, 10, QPoint::QPoint(79, 17), this, "08", "00", "0C");	
	customButton *button = new customButton(false, QPoint::QPoint(4, 110), this);
	customLed *led = new customLed(false, QPoint::QPoint(41, 4), this);
	QObject::connect(button, SIGNAL(valueChanged(bool)),
                         led, SLOT(changeValue(bool)));	*/
	setSlider1("08", "00", "03");
	setSlider2("08", "00", "06");
	setSlider3("08", "00", "09");
	setSlider4("08", "00", "0A");
	setSlider5("08", "00", "0C");
	setButton("08", "00", "00");
};

void stompbox_eq::updateSignal()
{
	updateSlider1("08", "00", "03");
	updateSlider2("08", "00", "06");
	updateSlider3("08", "00", "09");
	updateSlider4("08", "00", "0A");
	updateSlider5("08", "00", "0C");
	updateButton("08", "00", "00");
};
