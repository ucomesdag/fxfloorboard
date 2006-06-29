/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "FX Floor Board".
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMenuBar>

class mainWindow : public QWidget
{
    Q_OBJECT

public:
    mainWindow(QWidget *parent = 0);
	void createMenu();

public slots:
	void updateSize(QSize floorSize);

private:
	QMenuBar *menuBar;
	QMenu *fileMenu;
	QAction *openAction;
	QAction *saveAction;
	QAction *saveAsAction;
	QAction *exitAction;
};

#endif // MAINWINDOW_H
