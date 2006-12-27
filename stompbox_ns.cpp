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

#include "stompbox_ns.h"

stompbox_ns::stompbox_ns(QWidget *parent)
    : stompBox(parent)
{
	/* NS */
	setImage(":/images/ns.png");
	setKnob1("0E", "00", "01");
	setKnob2("0E", "00", "02");
	setButton();
};

void stompbox_ns::updateSignal()
{
	updateKnob1("0E", "00", "01");
	updateKnob2("0E", "00", "02");
	updateButton("0E", "00", "00");
};