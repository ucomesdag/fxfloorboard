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

#ifndef FLOORBOARDDISPLAY_H
#define FLOORBOARDDISPLAY_H

#include <QtGui>
#include <QWidget>
#include <QTimer>
#include "customButton.h"
#include "customDisplay.h"
#include "initPatchListMenu.h"

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
	void connectSignal(bool value);
	void writeSignal(bool value);
	void connectionResult(QString);
	void resetDevice(QString replyMsg);
	void patchSelectSignal(int bank, int patch);
	void blinkSellectedPatch(bool active = true);
	void patchLoadSignal(int bank, int patch);
	void notConnected();

	void patchChangeFailed();

	void writeToBuffer();
	void writeToMemory();

	void valueChanged(bool value, QString hex1, QString hex2, QString hex3); // Not used.

signals:
	void updateSignal();
	void connectedSignal();
	void notConnectedSignal();

	void setStatusSymbol(int value);
	void setStatusProgress(int value);
    void setStatusMessage(QString message);

private:
	QPoint pos;

	customDisplay *valueDisplay;
	customDisplay *patchDisplay;
	customDisplay *patchNumDisplay;

	customButton *connectButton;
	customButton *writeButton;
	customButton *manualButton;
	customButton *assignButton;
	customButton *masterButton;
	customButton *systemButton;

	bool connectButtonActive;
	bool patchLoadError;
	QTimer* timer;
	int blinkCount;
	bool currentSyncStatus;

	initPatchListMenu *initPatch;
};

#endif // FLOORBOARDDISPLAY_H
