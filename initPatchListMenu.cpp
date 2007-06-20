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

#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include "SysxIO.h"
#include "sysxWriter.h"
#include "Preferences.h"
#include "initPatchListMenu.h"

initPatchListMenu::initPatchListMenu(QRect geometry, QWidget *parent)
    : QWidget(parent)
{
	setInitPatchComboBox(geometry);
};

void initPatchListMenu::setInitPatchComboBox(QRect geometry)
{
	Preferences *preferences = Preferences::Instance();
	QDir initPatchesDir = QDir("Init Patches");
	if(QFileInfo( initPatchesDir.path().append(".lnk") ).exists())
	{
		initPatchesDir.setPath(QFileInfo( initPatchesDir.path().append(".lnk") ).symLinkTarget());
	};

	QDir defaultPatchesDir = QDir(preferences->getPreferences("General", "Files", "dir").remove(QRegExp("$(/)")));
	QDir defaultInitPatchesDir = QDir(defaultPatchesDir.path().append("/").append("Init Patches"));
	if(QFileInfo( defaultInitPatchesDir.path().append(".lnk") ).exists())
	{
		defaultInitPatchesDir.setPath(QFileInfo( defaultInitPatchesDir.path().append(".lnk") ).symLinkTarget());
	};

	// Create a shortcut in the default patch directory.
	if(defaultPatchesDir.exists() && !defaultInitPatchesDir.exists())
	{
		#ifdef Q_OS_UNIX
			QFile::link(initPatchesDir.absolutePath(), defaultInitPatchesDir.absolutePath());
		#endif

		#ifdef Q_OS_WIN
			QFile::link(initPatchesDir.absolutePath(), defaultInitPatchesDir.absolutePath().append(".lnk"));
		#endif
	};

	QString snork = defaultInitPatchesDir.path();
	if(defaultInitPatchesDir.exists())
	{
		initPatchesDir = defaultInitPatchesDir;
	};

	if(initPatchesDir.exists())
	{		
		this->initPatchComboBox = new QComboBox(this);
		this->initPatchComboBox->setObjectName("smallcombo");
		initPatchComboBox->addItem(tr("[ INIT Patches ]")); 

		
		QStringList filters;
		filters << "*.syx" << "*.syx2";
		QStringList initPatchesList = initPatchesDir.entryList(filters);
		
		int itemsCount;
		int maxLenght = 0;
		for(itemsCount=0; itemsCount<initPatchesList.size(); itemsCount++)
		{
			QString path = initPatchesDir.absolutePath().append("/").append(initPatchesList.at(itemsCount));
			this->initPatches.append(path);
			QString item = initPatchesList.at(itemsCount);
			item.remove(QRegExp("^[0-9_]+"));
			item.remove(QRegExp(".{1}(syx|syx2)"));
			if(!item.contains("INIT_"))
			{
				item.prepend(tr("(My INIT) "));
			};
			item.remove("INIT_");
			item.replace("_", " ");
			item.replace("-!-", "/");
			initPatchComboBox->addItem(item);
			int pixelWidth = QFontMetrics(initPatchComboBox->font()).width(item);
			if(maxLenght < pixelWidth) maxLenght = pixelWidth;
		};	

		initPatchComboBox->setMaxVisibleItems(itemsCount + 1);
		initPatchComboBox->view()->setMinimumWidth( maxLenght + 35 );	// Used to be 25 (scrollbar correction).	

		QObject::connect(initPatchComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(loadInitPatch(int)));
		QObject::connect(this, SIGNAL(updateSignal()),
			this->parent()->parent(), SIGNAL(updateSignal()));
	
		initPatchComboBox->setGeometry(geometry);
		initPatchComboBox->setEditable(false);
		initPatchComboBox->setFrame(false);
	};
};


void initPatchListMenu::setIndex(int index)
{
	this->initPatchComboBox->setCurrentIndex(index);
};

void initPatchListMenu::loadInitPatch(int index)
{
	if(index > 0)
	{
		QString fileName = this->initPatches.at(index - 1 );
		if (!fileName.isEmpty())	
		{
			sysxWriter file;
			file.setFile(fileName);  
			if(file.readFile())
			{	
				// DO SOMETHING AFTER READING THE FILE (UPDATE THE GUI)
				SysxIO *sysxIO = SysxIO::Instance();
				sysxIO->setFileSource(file.getFileSource());
				sysxIO->setFileName(tr("init patch"));
				sysxIO->setSyncStatus(false);
				sysxIO->setDevice(false);
				emit updateSignal();
			};
		};
	};
};
