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

#include "floorBoardDisplay.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "customButton.h"

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

	customButton *connectButton = new customButton(tr("CONNECT"), false, QPoint(408, 6), this, ":/images/ledbutton.png");
	customButton *writeButton = new customButton(tr("WRITE/SYNC"), false, QPoint(493, 6), this, ":/images/ledbutton.png");
	customButton *manualButton = new customButton(tr("MANUAL"), false, QPoint(578, 6), this, ":/images/ledbutton.png");
	customButton *assignButton = new customButton(tr("ASSIGN"), false, QPoint(578, 23), this, ":/images/pushbutton.png");
	customButton *masterButton = new customButton(tr("MASTER"), false, QPoint(663, 6), this, ":/images/pushbutton.png");
	customButton *systemButton = new customButton(tr("SYSTEM"), false, QPoint(663, 23), this, ":/images/pushbutton.png");

	QFont fontComboBox;
	fontComboBox.setFamily("Arial");
	fontComboBox.setBold(true);
	fontComboBox.setPixelSize(10);
	fontComboBox.setStretch(110);

	QComboBox *initPatchcomboBox = new QComboBox(this);
	initPatchcomboBox->addItem(tr(" [ INIT Patches ] "));
	initPatchcomboBox->setGeometry(408, 23, 169, 15);
	initPatchcomboBox->setEditable(false);
	initPatchcomboBox->setFont(fontComboBox);
	initPatchcomboBox->setPalette(pal);
	initPatchcomboBox->setFrame(false);
	//initPatchcomboBox->setMaxVisibleItems(itemsCount);
	//initPatchcomboBox->view()->setMinimumWidth( maxLenght + 10 );
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
