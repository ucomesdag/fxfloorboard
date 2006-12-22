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

#include <QtGui>
#include "mainWindow.h"
#include "Preferences.h"
#include "preferencesDialog.h"
#include "SysxIO.h"

/*mainWindow::mainWindow(QWidget *parent)
    : QWidget(parent)*/
mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	floorBoard *fxsBoard = new floorBoard(this);
	this->fxFloorBoard = fxsBoard;
	
	this->setWindowTitle("GT-8 FX FloorBoard");
	this->setCentralWidget(fxsBoard);
	
	createActions();
	createMenus();
	createStatusBar();

	QObject::connect(fxsBoard, SIGNAL( sizeChanged(QSize, QSize) ),
                this, SLOT( updateSize(QSize, QSize) ) );

	/*QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setMenuBar(menuBar);
	mainLayout->addWidget(fxsBoard);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	mainLayout->setSizeConstraint(QLayout::SetFixedSize);
	setLayout(mainLayout);*/
	this->wSize = fxsBoard->getSize();
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
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

void mainWindow::updateSize(QSize floorSize, QSize oldFloorSize)
{
	this->setFixedWidth(floorSize.width());
	int x = this->geometry().x() - ((floorSize.width() - oldFloorSize.width()) / 2);
	int y = this->geometry().y();	
	this->setGeometry(x, y, floorSize.width(), this->geometry().height());
};

void mainWindow::createActions()
{
	openAct = new QAction(QIcon(":/images/open.png"), tr("&Open File..."), this);
	openAct->setShortcut(tr("Ctrl+O"));
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
	saveAct->setShortcut(tr("Ctrl+S"));
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(tr("Ctrl+Q"));
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	settingsAct = new QAction(QIcon(":/images/cut.png"), tr("&Preferences"), this);
	settingsAct->setShortcut(tr("Ctrl+X"));
	settingsAct->setStatusTip(tr("...."));
	connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

	helpAct = new QAction(QIcon(":/images/copy.png"), tr("GT-8 FX FloorBoard &Help"), this);
	helpAct->setShortcut(tr("Ctrl+C"));
	helpAct->setStatusTip(tr("....."));
	connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

	homepageAct = new QAction(QIcon(":/images/paste.png"), tr("GT-8 FX FloorBoard &Webpage"), this);
	homepageAct->setShortcut(tr("Ctrl+V"));
	homepageAct->setStatusTip(tr("........"));
	connect(homepageAct, SIGNAL(triggered()), this, SLOT(homepage()));

	donationAct = new QAction(QIcon(":/images/paste.png"), tr("Make a &Donation"), this);
	donationAct->setShortcut(tr("Ctrl+V"));
	donationAct->setStatusTip(tr("........"));
	connect(donationAct, SIGNAL(triggered()), this, SLOT(donate()));

	licenseAct = new QAction(QIcon(":/images/paste.png"), tr("&License"), this);
	licenseAct->setShortcut(tr("Ctrl+V"));
	licenseAct->setStatusTip(tr("........"));
	connect(licenseAct, SIGNAL(triggered()), this, SLOT(license()));

	aboutAct = new QAction(tr("&About"), this);
	aboutAct->setStatusTip(tr("Show the application's About box"));
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
};

void mainWindow::createMenus()
{
    /*menuBar = new QMenuBar;*/

    QMenu *fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addAction(saveAct);
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    menuBar()->addMenu(fileMenu);

	QMenu *toolsMenu = new QMenu(tr("&Tools"), this);
	toolsMenu->addAction(settingsAct);
    menuBar()->addMenu(toolsMenu);

	QMenu *helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(helpAct);
	helpMenu->addAction(homepageAct);
	helpMenu->addSeparator();
	helpMenu->addAction(donationAct);
	helpMenu->addAction(licenseAct);
	helpMenu->addSeparator(); 
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
    menuBar()->addMenu(helpMenu);
};

void mainWindow::createStatusBar()
{
	statusBar()->setSizeGripEnabled(false);
	statusBar()->showMessage(tr("Ready"));
};

/* FILE MENU */
void mainWindow::open()
{
	Preferences *preferences = Preferences::Instance();
	QString dir = preferences->getPreferences("General", "Files", "dir");

	QString fileName = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                dir,
                "System Exclusive (*.syx)");
	if (!fileName.isEmpty())	
	{
		file.setFile(fileName);  
		if(file.readFile())
		{	
			// DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
			SysxIO *sysxIO = SysxIO::Instance();
			sysxIO->setFileSource(file.getFileSource());

			emit updateSignal();
		};
	};
};

void mainWindow::save()
{
	Preferences *preferences = Preferences::Instance();
	QString dir = preferences->getPreferences("General", "Files", "dir");

	if(file.getFileName().isEmpty())
	{
		QString fileName = QFileDialog::getSaveFileName(
						this,
						"Save As",
						dir,
						"System Exclusive (*.syx)");
		if (!fileName.isEmpty())	
		{
			if(!fileName.contains(".syx"))
			{
				fileName.append(".syx");
			};
			file.writeFile(fileName);
		};
	}
	else
	{
		file.writeFile(file.getFileName());
	};
};

void mainWindow::saveAs()
{
	Preferences *preferences = Preferences::Instance();
	QString dir = preferences->getPreferences("General", "Files", "dir");

	QString fileName = QFileDialog::getSaveFileName(
                    this,
                    "Save As",
                    dir,
                    "System Exclusive (*.syx)");
	if (!fileName.isEmpty())	
	{
		if(!fileName.contains(".syx"))
		{
			fileName.append(".syx");
		};
		file.writeFile(fileName);
	};
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

		if(midiIn=="-1") { midiIn = ""; };
		if(midiOut=="-1") {	midiOut = ""; };

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
	QDesktopServices::openUrl(QUrl(":license.txt"));
};

void mainWindow::about()
{
    Preferences *preferences = Preferences::Instance();
	QString version = preferences->getPreferences("General", "Application", "version");
	
	QFile file(":about"); 
	if(file.open(QIODevice::ReadOnly))
	{	
		QMessageBox::about(this, tr("GT-8 Fx FloorBoard - About"), 
			"GT-8 FX FloorBoard, " + tr("version") + " " + version + "\n" + file.readAll());
	};
};

QSize mainWindow::getWindowSize()
{
	return this->wSize;
};

void mainWindow::closeEvent(QCloseEvent* ce)
{
	Preferences *preferences = Preferences::Instance();
	preferences->savePreferences();
	ce->accept();
};