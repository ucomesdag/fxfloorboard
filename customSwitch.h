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

#ifndef CUSTOMSWITCH_H
#define CUSTOMSWITCH_H

#include <QWidget>

class customSwitch : public QWidget
{
    Q_OBJECT

public:
    customSwitch(
		bool active = false,
		QPoint switchPos = QPoint::QPoint(0, 0),
		QWidget *parent = 0,
		QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
		QString imagePath = ":/images/ampswitch.png");
	void setValue(bool value);

signals:
	void valueChanged(bool newValue, QString hex1, QString hex2, QString hex3);

protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	void setOffset(signed int imageNr);
	void emitValue(bool value);

	QString hex1;
	QString hex2;
	QString hex3;

	bool active;
	bool m_value;
	QString imagePath;
	QSize switchSize;
	unsigned int imageRange;
	QPoint switchPos; 
	QPoint dragStartPosition;
	signed int yOffset;
};

#endif // CUSTOMSWITCH_H
