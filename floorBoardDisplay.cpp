/****************************************************************************
**
** Copyright (C) 2005, 2006, 2007 Uco Mesdag. All rights reserved.
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

#include <QFile>
#include <QDir>
#include <QRegExp>
#include "floorBoardDisplay.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "midiIO.h"
#include "sysxWriter.h"

floorBoardDisplay::floorBoardDisplay(QWidget *parent, QPoint pos)
    : QWidget(parent)
{
	Preferences *preferences = Preferences::Instance();
	QString version = preferences->getPreferences("General", "Application", "version");

	QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SLOT(updateDisplay()));
	
	this->pos = pos;

	QPalette pal;
    pal.setColor(QPalette::Base,QColor(0,1,62));
    pal.setColor(QPalette::Text,QColor(0,255,204));
	pal.setColor(QPalette::Highlight,QColor(0,1,62));
	pal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	QFont fontDisplay;
	fontDisplay.setFamily("Arial");
	fontDisplay.setBold(true);
	fontDisplay.setPixelSize(11);
	fontDisplay.setStretch(140);

	QTextEdit *patchNumDisplay = new QTextEdit(this);
	patchNumDisplay->setGeometry(25, 5, 50, 34);
	patchNumDisplay->setFont(fontDisplay);
	patchNumDisplay->setPalette(pal);
	patchNumDisplay->setReadOnly(true);
	patchNumDisplay->setAcceptRichText(true);
	patchNumDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchNumDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchNumDisplay->setLineWrapMode(QTextEdit::NoWrap);
	patchNumDisplay->setAlignment(Qt::AlignJustify);
	patchNumDisplay->setDisabled(true);
	patchNumDisplay->setLineWidth(0);
	patchNumDisplay->setContentsMargins(0, 0, 0, 0);
	this->patchNumDisplay = patchNumDisplay;

	QTextEdit *patchDisplay = new QTextEdit(this);
	patchDisplay->setGeometry(85, 5, 150, 34);
	patchDisplay->setFont(fontDisplay);
	patchDisplay->setPalette(pal);
	patchDisplay->setReadOnly(true);
	patchDisplay->setAcceptRichText(true);
	patchDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchDisplay->setLineWrapMode(QTextEdit::NoWrap);
	patchDisplay->setAlignment(Qt::AlignJustify);
	patchDisplay->setDisabled(true);
	patchDisplay->setLineWidth(0);
	patchDisplay->setContentsMargins(0, 0, 0, 0);
	this->patchDisplay = patchDisplay;

	QString str;
	str.append("<html><body>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td colspan='2' align='left'>");
	str.append("GT-8 FX FloorBoard");
	str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
	str.append(tr("version"));
	str.append("</font></td><td align='right' valign='top'><font size='-1'>");

	str.append(version);
	str.append("</font></td></tr></table>");
	str.append("</body></html>");
	patchDisplay->setHtml(str);

	QTextEdit *valueDisplay = new QTextEdit(this);
	valueDisplay->setGeometry(245, 5, 150, 34);
	valueDisplay->setFont(fontDisplay);
	valueDisplay->setPalette(pal);
	valueDisplay->setReadOnly(true);
	valueDisplay->setAcceptRichText(true);
	valueDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	valueDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	valueDisplay->setLineWrapMode(QTextEdit::NoWrap);
	valueDisplay->setAlignment(Qt::AlignJustify);
	valueDisplay->setDisabled(true);
	valueDisplay->setLineWidth(0);
	valueDisplay->setContentsMargins(0, 0, 0, 0);
	this->valueDisplay = valueDisplay;

	this->connectButton = new customButton(tr("Connect"), false, QPoint(405, 5), this, ":/images/ledbutton.png");
	this->writeButton = new customButton(tr("Write/Sync"), false, QPoint(494, 5), this, ":/images/ledbutton.png");
	this->manualButton = new customButton(tr("Manual"), false, QPoint(583, 5), this, ":/images/ledbutton.png");
	this->assignButton = new customButton(tr("Assign"), false, QPoint(583, 24), this, ":/images/pushbutton.png");
	this->masterButton = new customButton(tr("Master"), false, QPoint(672, 5), this, ":/images/pushbutton.png");
	this->systemButton = new customButton(tr("System"), false, QPoint(672, 24), this, ":/images/pushbutton.png");

	QObject::connect(this->connectButton, SIGNAL(valueChanged(bool)), this, SLOT(connectSignal(bool)));

	setInitPatchComboBox(QRect(405, 24, 168, 15));
};

QPoint floorBoardDisplay::getPos()
{
	return this->pos;
};

void floorBoardDisplay::setPos(QPoint newPos)
{
	this->move(newPos);
	this->pos = newPos;
};

void floorBoardDisplay::setValueDisplay(QString fxName, QString valueName, QString value)
{
	QString str;
	str.append("<html><body>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td colspan='2' align='left'>");
	str.append(fxName);
	str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
	str.append(valueName);
	str.append("</font></td><td align='right' valign='top'><font size='-1'>");
	str.append(value);
	str.append("</font></td></tr></table>");
	str.append("</body></html>");
	valueDisplay->setHtml(str);
};

void floorBoardDisplay::setPatchDisplay(QString patchName)
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(sysxIO->getFileName() != ":default.syx") // Keep the initial version display if the default.syx is loaded at startup.
	{
		QString fileName = sysxIO->getFileName();
		
		QString str;
		str.append("<html><body>");
		str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td align='left'>");
		str.append(patchName);
		str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
		str.append(fileName.section('/', -1, -1));
		str.append("</font></td></tr></table>");
		str.append("</body></html>");
		patchDisplay->setHtml(str);
	};
	if(sysxIO->getFileName() == tr("init patch"))
	{
		sysxIO->setFileName("");
	}
	else
	{
		this->initPatchComboBox->setCurrentIndex(0);
	};	
};

void floorBoardDisplay::setPatchNumDisplay(int patchNumber)
{
	QString str;
	str.append("<html><body>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td align='left' valign='middle'>");
	str.append("35");
	str.append("</td><td align='left' valign='middle'>");
	str.append("4");
	str.append("</td></tr></table>");
	str.append("</body></html>");
	patchNumDisplay->setHtml(str);
};

void floorBoardDisplay::updateDisplay()
{
	SysxIO *sysxIO = SysxIO::Instance();
	QList<QString> nameArray = sysxIO->getFileSource("12", "00");

	MidiTable *midiTable = MidiTable::Instance();
	QString patchName;
	for(int i=11;i<nameArray.size() - 2;i++ )
	{
		patchName.append( midiTable->getMidiMap("Stucture", "12", "00", "00", nameArray.at(i)).name);
	};	
	setPatchDisplay(patchName);
	valueDisplay->clear();
};

void floorBoardDisplay::setInitPatchComboBox(QRect geometry)
{
	Preferences *preferences = Preferences::Instance();
	QDir initPatchesDir = "Init Patches";
	QDir defaultPatchesDir = preferences->getPreferences("General", "Files", "dir").remove(QRegExp("$(/)"));
	QDir defaultInitPatchesDir = defaultPatchesDir.path().append("/").append(initPatchesDir.path());
	
	// Create a shortcut in the default patch directory.
	if(defaultPatchesDir.exists() && !defaultInitPatchesDir.exists())
	{
		/*Doesn't work on windows so disabled for the time being.
		QFile::link(initPatchesDir.absolutePath(), deafultInitPatchesDir.absolutePath());*/
	};

	if(defaultInitPatchesDir.exists())
	{
		initPatchesDir = defaultInitPatchesDir;
	};

	QPalette pal;
	pal.setColor(QPalette::Base,QColor(0,1,62));
	pal.setColor(QPalette::Text,QColor(0,255,204));
	pal.setColor(QPalette::Highlight,QColor(0,1,62));
	pal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	pal.setColor(QPalette::Window,QColor(0,1,62));
	pal.setColor(QPalette::WindowText,QColor(0,255,204));	//List Border
	pal.setColor(QPalette::Button,QColor(0,1,62));
	pal.setColor(QPalette::ButtonText,QColor(0,255,204));

	pal.setColor(QPalette::Light,QColor(0,1,62));			//Lighter than Button color.
	pal.setColor(QPalette::Midlight,QColor(0,1,62));		//Between Button and Light.
	pal.setColor(QPalette::Dark,QColor(0,1,62));			//Darker than Button.
	pal.setColor(QPalette::Mid,QColor(0,1,62));				//Between Button and Dark.
	pal.setColor(QPalette::Shadow,QColor(0,1,62));
	
	QFont font;
	font.setFamily("Arial");
	font.setBold(true);
	font.setPixelSize(10);
	font.setStretch(110);

	this->initPatchComboBox = new QComboBox(this);
	initPatchComboBox->addItem(tr("[ INIT Patches ]"));

	if(initPatchesDir.exists())
	{		
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
			int pixelWidth = QFontMetrics(font).width(item);
			if(maxLenght < pixelWidth) maxLenght = pixelWidth;
		};	

		initPatchComboBox->setMaxVisibleItems(itemsCount + 1);
		initPatchComboBox->view()->setMinimumWidth( maxLenght + 25 );		

		QObject::connect(initPatchComboBox, SIGNAL(currentIndexChanged(int)),
                this, SLOT(loadInitPatch(int)));
		QObject::connect(this, SIGNAL(updateSignal()),
			this->parent(), SIGNAL(updateSignal()));
	};

	initPatchComboBox->setGeometry(geometry);
	initPatchComboBox->setEditable(false);
	initPatchComboBox->setFont(font);
	initPatchComboBox->setPalette(pal);
	initPatchComboBox->setFrame(false);
};

void floorBoardDisplay::loadInitPatch(int index)
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
				emit updateSignal();
			};
		};
	};
};

void floorBoardDisplay::connectSignal(bool value)
{
	QString replyMsg;

	if(value == true)
	{
		midiIO *midi = new midiIO();

		Preferences *preferences = Preferences::Instance(); bool ok;
		int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);
		int midiIn = preferences->getPreferences("Midi", "MidiIn", "device").toInt(&ok, 10);

		QString sysxOut = "F0 7E 00 06 01 F7"; // GT-8 Identity Request.
		//QString sysxOut = "f0 41 00 00 00 06 12 0d 00 12 00 53 79 73 65 78 20 54 45 53 54 20 20 20 20 20 20 47 f7";
		replyMsg = midi->sendSysxMsg(sysxOut, midiOut, midiIn);
	};


	if(replyMsg.contains("0006") && value == true)
	{
		this->connectButton->setValue(true);
		emit connectedToDevice();
	}
	else if(!replyMsg.isEmpty())
	{
		this->connectButton->setValue(false);

		QMessageBox *msgBox = new QMessageBox();
		msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard - Connection Error"));
		msgBox->setIcon(QMessageBox::Warning);
		msgBox->setText(tr("The device connected is not a GT-8 Guitar Effects Processor."));
		msgBox->setStandardButtons(QMessageBox::Ok);
		msgBox->exec();
	}
	else
	{
		this->connectButton->setValue(false);
	};
};