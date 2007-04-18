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

#include "stompbox_fv.h"

stompbox_fv::stompbox_fv(QWidget *parent)
    : stompBox(parent)
{
	/* VOLUME */
	setImage(":/images/fv.png");
	setLSB("15", "00");
	//setButton("15", "00", "00", QPoint::QPoint(0, 109), ":/images/pedal.png");
	setEditPages();
};

void stompbox_fv::updateSignal()
{
	//updateButton("15", "00", "00");
};

void stompbox_fv::setEditPages()
{
    editDetails()->page()->newGroupBox("Foot Volume");
	editDetails()->page()->addComboBox(0, 0, 1, 1, "0E", "00", "08", "bottom", Qt::AlignTop);
	editDetails()->page()->addKnob(0, 1, 1, 1, "0E", "00", "07");
	editDetails()->page()->addGroupBox(0, 0, 1, 1);

	editDetails()->addPage();
};
