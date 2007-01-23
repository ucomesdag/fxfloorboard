/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
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

#ifndef FLOORBOARDDISPLAY_H
#define FLOORBOARDDISPLAY_H

#include <QtGui>
#include <QWidget>
#include "customButton.h"
#include "midiIO.h"

class floorBoardDisplay : public QWidget
{
	Q_OBJECT

public:
	floorBoardDisplay(QWidget *parent = 0, 
		QPoint pos = QPoint::QPoint(0, 0));
	QPoint getPos();

public slots:
	void setValueDisplay(QString fxName, QString valueName, QString value);
	void setPatchDisplay(QString patchName);
	void setPatchNumDisplay(int bank, int patch);
	void setPos(QPoint newPos);
	void updateDisplay();
	void loadInitPatch(int index);
	void connectSignal(bool value);
	void connectionResult(QString);

signals:
	void currentIndexChanged(int index);
	void updateSignal();
	void connectedSignal();

private:
	QPoint pos;

	QFont fontDisplay;
	QTextEdit *valueDisplay;
	QTextEdit *patchDisplay;
	QTextEdit *patchNumDisplay;
	QComboBox *initPatchComboBox;

	void setInitPatchComboBox(QRect geometry);
	QList<QString> initPatches;

	customButton *connectButton;
	customButton *writeButton;
	customButton *manualButton;
	customButton *assignButton;
	customButton *masterButton;
	customButton *systemButton;

	midiIO *midi;

	bool connectButtonActive;
};

#endif // FLOORBOARDDISPLAY_H
