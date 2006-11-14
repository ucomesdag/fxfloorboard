/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8: FX Floor Board".
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
	QAction *openAction = fileMenu->addAction("&Open File...");
	QAction *saveAction = fileMenu->addAction("&Save");
	QAction *saveAsAction = fileMenu->addAction("Save &As...");
	fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("E&xit");
    menuBar->addMenu(fileMenu);

	QMenu *toolsMenu = new QMenu("&Tools", this);
	QAction *settingsAction = toolsMenu->addAction("&Preferences");
    menuBar->addMenu(toolsMenu);

	QMenu *helpMenu = new QMenu("&Help", this);
	QAction *helpAction = helpMenu->addAction("FX Floor Board &Help");
	QAction *homepageAction = helpMenu->addAction("FX Floor Board &Webpage");
	helpMenu->addSeparator();
	QAction *donationAction = helpMenu->addAction("Make a &Donation");
	QAction *licenseAction = helpMenu->addAction("&License");
	helpMenu->addSeparator(); QAction *aboutAction = helpMenu->addAction("&About");
    menuBar->addMenu(helpMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	
	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
	connect(homepageAction, SIGNAL(triggered()), this, SLOT(homepage()));
	connect(donationAction, SIGNAL(triggered()), this, SLOT(donate()));
	connect(licenseAction, SIGNAL(triggered()), this, SLOT(license()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

};

void mainWindow::updateSize(QSize floorSize)
{
	this->setFixedWidth(floorSize.width());
};

/* HELP MENU */
void mainWindow::help()
{
	QUrl helpUrl("http://sourceforge.net/forum/forum.php?forum_id=585786");
	QDesktopServices::openUrl(helpUrl);
};

void mainWindow::homepage()
{
	QUrl homepageUrl("http://fxfloorboard.sourceforge.net/");
	QDesktopServices::openUrl(homepageUrl);
};

void mainWindow::donate()
{
	QUrl donateUrl("http://sourceforge.net/donate/index.php?group_id=171049");
	QDesktopServices::openUrl(donateUrl);
};

void mainWindow::license()
{
	/*
	QString licensePath = QDir::currentPath();
	licensePath.append("/license.txt");
	QUrl licenseUrl = QUrl::fromLocalFile(licensePath);
	QDesktopServices::openUrl(licenseUrl);
	*/
	QUrl licenseUrl(":license.txt");
	QDesktopServices::openUrl(licenseUrl);
};

void mainWindow::about()
{
    QFile file(":about"); 
	if(file.open(QIODevice::ReadOnly))
	{	
		QMessageBox::about(this, "About FX Floor Board", file.readAll());
	};
};

