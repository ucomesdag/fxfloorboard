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

#include "stompbox_fv.h"
#include "MidiTable.h"
#include "SysxIO.h"

stompbox_fv::stompbox_fv(QWidget *parent)
    : stompBox(parent)
{
	SysxIO *sysxIO = SysxIO::Instance();
	MidiTable *midiTable = MidiTable::Instance();

	/* VOLUME */
	this->setImage(":/images/fv.png");

	customButton *button = new customButton(false, QPoint::QPoint(0, 109), this, ":/images/pedal.png", QSize::QSize(85, 59));
	customLed *led = new customLed(false, QPoint::QPoint(41, 4), this);
	QObject::connect(button, SIGNAL(valueChanged(bool)),
                         led, SLOT(setValue(bool)));	
};