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

#ifndef FLOORBOARD_H
#define FLOORBOARD_H

#include <QWidget>
#include <QPixmap>
#include "stompBox.h"
#include "editWindow.h"

class floorBoard : public QWidget
{
    Q_OBJECT

public:
    floorBoard(
		QWidget *parent = 0,
		QString imagePathFloor = ":/images/floor.png",
		QString imagePathStompBG = ":/images/stompbg.png",
		QString imagePathInfoBar = ":/images/infobar.png",
		unsigned int marginStompBoxesTop = 72,
		unsigned int marginStompBoxesBottom = 72,
		unsigned int marginStompBoxesWidth = 25,
		unsigned int panelBarOffset = 10,
		unsigned int borderWidth = 3,
		QPoint pos = QPoint::QPoint(0, 0));
	~floorBoard();
	QPoint getStompPos(int id);

public slots:
	void setWidth(int dist);
	void setCollapse();
	void updateStompBoxes();
	void setEditDialog(editWindow* editDialog);

signals:
	void valueChanged(QString fxName, QString valueName, QString value);
	void setDisplayPos(QPoint newpos);
	void setFloorPanelBarPos(QPoint newpos);
	void updateStompOffset(signed int offsetDif);
	void sizeChanged(QSize newsize, QSize oldSize);
	void setCollapseState(bool state);
	void resizeSignal(QRect newrect);
	void showDragBar(QPoint newpos);
	void hideDragBar();
	void updateSignal();
	
protected:
	void paintEvent(QPaintEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
	void initSize(QSize floorSize);
	void setSize(QSize newSize);
	void setFloorBoard();
	void initStomps();
	void setStomps(QList<QString> stompOrder);
	void setStompPos(QString name, int order);
	void setStompPos(int index, int order);
	void centerEditDialog();

	QString imagePathFloor;
	QString imagePathStompBG;
	QString imagePathInfoBar;

	unsigned int offset;
	unsigned int infoBarWidth;
	unsigned int panelBarOffset;
	unsigned int borderWidth;
	unsigned int floorHeight;
	QSize minSize;
	QSize maxSize;
	QSize l_floorSize;
	QSize floorSize;

	unsigned int marginStompBoxesTop;
	unsigned int marginStompBoxesBottom;
	unsigned int marginStompBoxesWidth;

	QSize stompSize;
	QPixmap baseImage;
	QPixmap image;
	
	QPoint pos;
	QPoint displayPos;
	QPoint liberainPos;
	QPoint panelBarPos;

	QList<QPoint> fxPos;
	QList<int> fx;
	bool colapseState;

	QList<stompBox*> stompBoxes;
	QList<QString> stompNames;
	editWindow* editDialog;
};

#endif // FLOORBOARD_H
