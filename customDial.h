/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8: FX FloorBoard".
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

#ifndef CUSTOMDIAL_H
#define CUSTOMDIAL_H

#include <QWidget>

class customDial : public QWidget
{
    Q_OBJECT

public:
    customDial(
		double value = 50, 
		double min = 0, 
		double max = 100, 
		double single = 1, 
		double page = 10,
		QPoint dialPos = QPoint::QPoint(0, 0), 
		QWidget *parent = 0,
		QString imagePath = ":/images/knob.png",
		QSize dialSize = QSize::QSize(35, 35),
		unsigned int imageRange = 62);
	void setOffset(double value);
	void setValue(double value);

signals:
	void valueChanged(int newValue);

protected:
   void paintEvent(QPaintEvent *event);
   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void wheelEvent(QWheelEvent *event);
   void keyPressEvent(QKeyEvent *event);

private:
	double value; 
	double min; 
	double max; 
	double single; 
	double page;
	QString imagePath;
	QSize dialSize;
	unsigned int imageRange;
	QPoint dialPos; 

	signed int xOffset;
	double _lastValue;
	QPoint _startdialPos;
	QPoint _lastdialPos;

	
	double m_value;
};

#endif // CUSTOMDIAL_H
