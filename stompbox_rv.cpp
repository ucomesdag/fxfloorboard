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

#include "stompbox_rv.h"
#include "MidiTable.h"
#include "SysxIO.h"

#include "customButton.h"
#include "customLed.h"
#include "customDial.h"
 
stompbox_rv::stompbox_rv(QWidget *parent)
    : stompBox(parent)
{
	SysxIO *sysxIO = SysxIO::Instance();
	MidiTable *midiTable = MidiTable::Instance();

	/* REVERB */
	this->setImage(":/images/rv.png");

	this->setComboBox(midiTable->getMidiMap("Stucture", "0D", "00", "02"));

	int range1 = midiTable->getRange("Stucture", "0D", "00", "03");
	int range2 = midiTable->getRange("Stucture", "0D", "00", "08");

	customDial *knob1 = new customDial(0, 0, range1, 1, 10, QPoint::QPoint(6, 9), this, "0D", "00", "03");
	customDial *knob2 = new customDial(0, 0, range2, 1, 10, QPoint::QPoint(53, 9), this, "0D", "00", "08");
	customButton *button = new customButton(false, QPoint::QPoint(4, 110), this);
	customLed *led = new customLed(false, QPoint::QPoint(41, 4), this);
	QObject::connect(button, SIGNAL(valueChanged(bool)),
                         led, SLOT(setValue(bool)));	
};