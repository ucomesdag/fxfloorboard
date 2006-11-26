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

#include <QLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QMessageBox>
#include "mainWindow.h"
#include "floorBoard.h"
#include "Preferences.h"
#include "preferencesDialog.h"

mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)
{
	createMenu();

	floorBoard *fxsBoard = new floorBoard(this);

	QObject::connect(fxsBoard, SIGNAL( sizeChanged(QSize, QSize) ),
                this, SLOT( updateSize(QSize, QSize) ) );

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMenuBar(menuBar);
	mainLayout->addWidget(fxsBoard);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(mainLayout);

	this->wSize = fxsBoard->getSize();
	
	setWindowTitle("GT-8 FX FloorBoard");
};

mainWindow::~mainWindow()
{
	Preferences *preferences = Preferences::Instance();
	if(preferences->getPreferences("Window", "Restore", "window")=="true")
	{
		preferences->setPreferences("Window", "Position", "x", QString::number(this->x(), 10));
		preferences->setPreferences("Window", "Position", "y", QString::number(this->y(), 10));
	}
	else
	{
		preferences->setPreferences("Window", "Position", "x", "");
		preferences->setPreferences("Window", "Position", "y", "");
	};
};

void mainWindow::createMenu()
{
    menuBar = new QMenuBar;

    QMenu *fileMenu = new QMenu(tr("&File"), this);
	QAction *openAction = fileMenu->addAction(tr("&Open File..."));
	QAction *saveAction = fileMenu->addAction(tr("&Save"));
	QAction *saveAsAction = fileMenu->addAction(tr("Save &As..."));
	fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(tr("E&xit"));
    menuBar->addMenu(fileMenu);

	QMenu *toolsMenu = new QMenu(tr("&Tools"), this);
	QAction *settingsAction = toolsMenu->addAction(tr("&Preferences"));
    menuBar->addMenu(toolsMenu);

	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	QAction *helpAction = helpMenu->addAction(tr("GT-8 FX FloorBoard &Help"));
	QAction *homepageAction = helpMenu->addAction(tr("GT-8 FX FloorBoard &Webpage"));
	helpMenu->addSeparator();
	QAction *donationAction = helpMenu->addAction(tr("Make a &Donation"));
	QAction *licenseAction = helpMenu->addAction(tr("&License"));
	helpMenu->addSeparator(); QAction *aboutAction = helpMenu->addAction(tr("&About"));
    menuBar->addMenu(helpMenu);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
	
	connect(settingsAction, SIGNAL(triggered()), this, SLOT(settings()));

	connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
	connect(homepageAction, SIGNAL(triggered()), this, SLOT(homepage()));
	connect(donationAction, SIGNAL(triggered()), this, SLOT(donate()));
	connect(licenseAction, SIGNAL(triggered()), this, SLOT(license()));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

};

void mainWindow::updateSize(QSize floorSize, QSize oldFloorSize)
{
	int x = this->geometry().x() - ((floorSize.width() - oldFloorSize.width()) / 2);
	int y = this->geometry().y();
	this->setFixedWidth(floorSize.width());
	this->setGeometry(x, y, floorSize.width(), this->height());
};

/* TOOLS MENU */
void mainWindow::settings()
{
	preferencesDialog *dialog = new preferencesDialog();
	if (dialog->exec())
	{
		Preferences *preferences = Preferences::Instance();

		QString dir = dialog->generalSettings->dirEdit->text();
		QString sidepanel = (dialog->windowSettings->sidepanelCheckBox->checkState())?QString("true"):QString("false");
		QString window = (dialog->windowSettings->windowCheckBox->checkState())?QString("true"):QString("false");
		QString splash = (dialog->windowSettings->splashCheckBox->checkState())?QString("true"):QString("false");
		QString midiIn = QString::number(dialog->midiSettings->midiInCombo->currentIndex() - 1, 10); // -1 because there is a default entry at index 0
		QString midiOut = QString::number(dialog->midiSettings->midiOutCombo->currentIndex() - 1, 10);

		if(midiIn=="-1")
		{
			midiIn = "";
		};
		if(midiOut=="-1")
		{
			midiOut = "";
		};

		preferences->setPreferences("General", "Files", "dir", dir);
		preferences->setPreferences("Midi", "MidiIn", "device", midiIn);
		preferences->setPreferences("Midi", "MidiOut", "device", midiOut);
		preferences->setPreferences("Window", "Restore", "sidepanel", sidepanel);
		preferences->setPreferences("Window", "Restore", "window", window);
		preferences->setPreferences("Window", "Splash", "bool", splash);
	};
};

/* HELP MENU */
void mainWindow::help()
{
	Preferences *preferences = Preferences::Instance();
	QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Help", "url") ));
};

void mainWindow::homepage()
{
	Preferences *preferences = Preferences::Instance();
	QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Webpage", "url") ));
};

void mainWindow::donate()
{
	Preferences *preferences = Preferences::Instance();
	QDesktopServices::openUrl(QUrl( preferences->getPreferences("General", "Donate", "url") ));
};

void mainWindow::license()
{
	/*
	QString licensePath = QDir::currentPath();
	licensePath.append("/license.txt");
	QUrl licenseUrl = QUrl::fromLocalFile(licensePath);
	QDesktopServices::openUrl(licenseUrl);
	*/
	QDesktopServices::openUrl(QUrl(":license.txt"));
};

void mainWindow::about()
{
    Preferences *preferences = Preferences::Instance();
	QString version = preferences->getPreferences("General", "Application", "version");
	
	QFile file(":about"); 
	if(file.open(QIODevice::ReadOnly))
	{	
		QMessageBox::about(this, tr("GT-8 Fx FloorBoard - About"), "GT-8 FX FloorBoard, " + tr("version") + " " + version + "\n" + file.readAll());
	};
};

QSize mainWindow::getWindowSize()
{
	return this->wSize;
};
