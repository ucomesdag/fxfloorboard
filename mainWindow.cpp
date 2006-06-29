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

#include <QLayout>
#include "mainWindow.h"
#include "floorBoard.h"

mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)
{
	createMenu();

	floorBoard *fxsBoard = new floorBoard(this);

	QObject::connect(fxsBoard, SIGNAL( sizeChanged(QSize) ),
                this, SLOT( updateSize(QSize) ) );

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMenuBar(menuBar);
	mainLayout->addWidget(fxsBoard);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(mainLayout);
};

void mainWindow::createMenu()
{
    menuBar = new QMenuBar;

    QMenu *fileMenu = new QMenu("&File", this);
	QAction *openAction = fileMenu->addAction("&Open");
	QAction *saveAction = fileMenu->addAction("&Save");
	QAction *saveAsAction = fileMenu->addAction("S&ave as");
	fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("E&xit");
    menuBar->addMenu(fileMenu);

	QMenu *toolsMenu = new QMenu("&Tools", this);
	QAction *settingsAction = toolsMenu->addAction("&Preferences");
    menuBar->addMenu(toolsMenu);

	QMenu *helpMenu = new QMenu("&Help", this);
	QAction *licenseAction = helpMenu->addAction("&License");
	helpMenu->addSeparator();
	QAction *homepageAction = helpMenu->addAction("H&omepage");
	QAction *donationAction = helpMenu->addAction("Make a &Donation");
	helpMenu->addSeparator();
    QAction *aboutAction = helpMenu->addAction("A&bout");
    menuBar->addMenu(helpMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
};

void mainWindow::updateSize(QSize floorSize)
{
	this->setFixedWidth(floorSize.width());
};
