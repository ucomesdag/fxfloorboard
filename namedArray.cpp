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

#include "namedArray.h"
#include <QVector>
#include <QString>

namedArray::namedArray() 
{
	this->_index_cur++;
};

namedArray::~namedArray()
{

};

unsigned int namedArray::_index_cur = 0; //counter

void namedArray::append(QString index, QString var)
{	
	this->_index.append(index); 
	this->_var.append(var);
};

QString namedArray::at(QString index)
{
	return this->_var.at(this->_index.indexOf(index));
};

QString	namedArray::indexOf(QString var)
{
	return this->_index.at(this->_var.indexOf(var));
};