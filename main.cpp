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
#include "mainWindow.h"
#include "Preferences.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	
	Preferences *preferences = Preferences::Instance(); // Load preferences

	mainWindow mainWindow;
	mainWindow.setWindowFlags( Qt::WindowTitleHint |  Qt::WindowMinimizeButtonHint );
	mainWindow.setWindowIcon(QIcon::QIcon(":/images/windowicon.png"));

	bool ok;
	QString x_str = preferences->getPreferences("Window", "Position", "x");
	QString y_str = preferences->getPreferences("Window", "Position", "y");
	mainWindow.setGeometry(x_str.toInt(&ok, 10), y_str.toInt(&ok, 10), mainWindow.width(), mainWindow.height());

	mainWindow.show();

    return app.exec();
};