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
#include "customSplashScreen.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

	QPixmap splashImage(":images/splash.png");

	customSplashScreen *splash = new customSplashScreen(splashImage);
	splash->setMessageRect(QRect::QRect(170, 400, 245, 14));
	splash->setMask(splashImage.mask());
	splash->setWindowOpacity(1.0);
	splash->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SplashScreen);
	splash->show();
	app.processEvents();

	splash->showStatusMessage("Starting...");

	mainWindow *window = new mainWindow;
	
	splash->showStatusMessage("Loading preferences...");
	Preferences *preferences = Preferences::Instance(); // Load preferences

	splash->showStatusMessage("Checking license file...");
	if(!QFile("license.txt").exists())
	{
		splash->showStatusMessage("Loading license file...");
		QFile file(":license.txt" );
		file.copy("license.txt");
		file.close();
	};

	splash->showStatusMessage("Initializing main window...");

	window->setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint );
	window->setWindowIcon(QIcon::QIcon(":/images/windowicon.png"));

	bool ok;
	QString x_str = preferences->getPreferences("Window", "Position", "x");
	QString y_str = preferences->getPreferences("Window", "Position", "y");
	if(preferences->getPreferences("Window", "Restore", "window")=="true" && !x_str.isEmpty())
	{
		splash->showStatusMessage("Restoring window position...");
		window->setGeometry(x_str.toInt(&ok, 10), y_str.toInt(&ok, 10), window->width(), window->height());
	}
	else
	{
		splash->showStatusMessage("Centering main window...");
		QDesktopWidget *desktop = new QDesktopWidget;
		//QRect screen = desktop->screenGeometry(desktop->primaryScreen());
		QRect screen = desktop->availableGeometry(desktop->primaryScreen()); 
		int screenWidth = screen.width();                    // returns screen width
		int screenHeight = screen.height();                  // returns screen height
		int windowWidth = window->getWindowSize().width();                  
		int windowHeight = window->getWindowSize().height();

		int x = (screenWidth - windowWidth) / 2;
		int y = (screenHeight - windowHeight) / 2;
		window->setGeometry(x, y, window->width(), window->height());
	};
	splash->showStatusMessage("Finished Initializing...");

	window->show();
	splash->finish(window);
	return app.exec();
};