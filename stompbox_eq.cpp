/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8 FX FloorBoard".
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
#include "MidiTable.h"
#include "SysxIO.h"

#include "customButton.h"
#include "customLed.h"
#include "customSlider.h"

stompbox_eq::stompbox_eq(QWidget *parent)
    : stompBox(parent)
{
	SysxIO *sysxIO = SysxIO::Instance();
	MidiTable *midiTable = MidiTable::Instance();

	/* EQ */
	this->setImage(":/images/eq.png");

	customSlider *slider1 = new customSlider(50, 0, 100, 1, 10, QPoint::QPoint(8, 17), this);
	customSlider *slider2 = new customSlider(50, 0, 100, 1, 10, QPoint::QPoint(24, 17), this);
	customSlider *slider3 = new customSlider(50, 0, 100, 1, 10, QPoint::QPoint(40, 17), this);
	customSlider *slider4 = new customSlider(50, 0, 100, 1, 10, QPoint::QPoint(56, 17), this);	
	customSlider *slider5 = new customSlider(50, 0, 100, 1, 10, QPoint::QPoint(79, 17), this);	
	customButton *button = new customButton(false, QPoint::QPoint(4, 110), this);
	customLed *led = new customLed(false, QPoint::QPoint(41, 4), this);
	QObject::connect(button, SIGNAL(valueChanged(bool)),
                         led, SLOT(setValue(bool)));	
};