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

#ifndef STOMPBOX_H
#define STOMPBOX_H

#include <QtGui>
#include <QWidget>

class stompBox : public QWidget
{
    Q_OBJECT

public:
    stompBox(
		QWidget *parent = 0,
		unsigned int id = 0,
		QString imagePath = ":/images/od.png",
		QPoint pos = QPoint::QPoint(0, 0),
		QSize stompSize = QSize::QSize(94, 168));
	void setPos(QPoint newPos);
	void setImage(QString imagePath);
	void setSize(QSize newSize);
	void setId(unsigned int id);
	unsigned int getId();

public slots:
	void updatePos(signed int offsetDif);

signals:
	void knobValue(int);


protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QString imagePath;
	QSize stompSize;
	QPoint stompPos; 
	unsigned int id;

	QPoint dragStartPosition;
	QPixmap image;
};

#endif // STOMPBOX_H
