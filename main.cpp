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

#include <QtGui/QApplication>
#include <QtGui>
#include <QDesktopWidget>
#include <QFile>
#include "mainWindow.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "customSplashScreen.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	/* Loading translation */
	QTranslator translator;
	translator.load("translation");
	app.installTranslator(&translator);
	
	/* Splash Screen setup uses subclassed QSplashScreen for message position controle. */
	QPixmap splashImage(":images/splash.png");

	customSplashScreen *splash = new customSplashScreen(splashImage);
	splash->setMessageRect(QRect::QRect(148, 340, 332, 14)); // Setting the message position.

	QFont splashFont;
	splashFont.setFamily("Arial");
	//splashFont.setBold(true);
	splashFont.setPixelSize(9);
	splashFont.setStretch(125);

	splash->setFont(splashFont);
	splash->setMask(splashImage.mask());
	splash->setWindowOpacity(0.95);
	splash->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen);
	
	Preferences *preferences = Preferences::Instance(); // Load the preferences.
	if(preferences->getPreferences("Window", "Splash", "bool")=="true")
	{
		splash->show();
	};
	/* To intercept mousclick to hide spalsh screen. Since the 
	splash screen is typically displayed before the event loop 
	has started running, it is necessary to periodically call. */
	app.processEvents(); 

	splash->showStatusMessage(QObject::tr("Starting..."));
	mainWindow window;// = new mainWindow;

	app.processEvents(); 

	splash->showStatusMessage(QObject::tr("Checking license file..."));
	if(!QFile("license.txt").exists())
	{
		splash->showStatusMessage(QObject::tr("Loading license file..."));
		QFile file(":license.txt" );
		file.copy("license.txt");
		file.close();
	};

	app.processEvents(); 

	splash->showStatusMessage(QObject::tr("Loading midi mapping..."));
	MidiTable *midiTable = MidiTable::Instance();

	app.processEvents(); 

	splash->showStatusMessage(QObject::tr("Initializing main window..."));
	window.setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint );
	window.setWindowIcon(QIcon::QIcon(":/images/windowicon.png"));

	app.processEvents(); 

	bool ok;
	QString x_str = preferences->getPreferences("Window", "Position", "x");
	QString y_str = preferences->getPreferences("Window", "Position", "y");
	if(preferences->getPreferences("Window", "Restore", "window")=="true" && !x_str.isEmpty())
	{
		splash->showStatusMessage(QObject::tr("Restoring window position..."));
		window.setGeometry(x_str.toInt(&ok, 10), y_str.toInt(&ok, 10), window.width(), window.height());
	}
	else
	{
		splash->showStatusMessage(QObject::tr("Centering main window..."));
		QDesktopWidget *desktop = new QDesktopWidget;
		//QRect screen = desktop->screenGeometry(desktop->primaryScreen());
		QRect screen = desktop->availableGeometry(desktop->primaryScreen()); 
		int screenWidth = screen.width();                    // returns screen width
		int screenHeight = screen.height();                  // returns screen height
		int windowWidth = window.getWindowSize().width();                  
		int windowHeight = window.getWindowSize().height();

		int x = (screenWidth - windowWidth) / 2;
		int y = (screenHeight - windowHeight) / 2;
		window.setGeometry(x, y, window.width(), window.height());
	};

	app.processEvents(); 

	splash->showStatusMessage(QObject::tr("Finished Initializing..."));

	window.show();
	splash->finish(&window);
	return app.exec();
};