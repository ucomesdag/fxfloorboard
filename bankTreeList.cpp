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

#include <QLayout>
#include "bankTreeList.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "midiIO.h"

bankTreeList::bankTreeList(QWidget *parent)
    : QWidget(parent)
{
	QPalette pal;
    pal.setColor(QPalette::Base,QColor(0,1,62));
    pal.setColor(QPalette::Text,QColor(0,255,204));
	pal.setColor(QPalette::Highlight,QColor(0,1,62));
	pal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	//pal.setColor(QPalette::Window,QColor(Qt::red));
	//pal.setColor(QPalette::WindowText,QColor(0,255,204));	//List Border
	//pal.setColor(QPalette::Button,QColor(0,1,62));
	//pal.setColor(QPalette::ButtonText,QColor(0,255,204));

	//pal.setColor(QPalette::Light,QColor(Qt::red));				//Lighter than Button color.
	//pal.setColor(QPalette::Midlight,QColor(Qt::red));			//Between Button and Light.
	//pal.setColor(QPalette::Dark,QColor(Qt::gray));				//Darker than Button. (Dots)
	//pal.setColor(QPalette::Mid,QColor(0,1,62));					//Between Button and Dark.
	//pal.setColor(QPalette::Shadow,QColor(Qt::red));
	this->pal = pal;

	QFont font;
	font.setFamily("Arial");
	font.setBold(true);
	font.setPixelSize(10);
	font.setStretch(120);
	this->font = font;

	this->treeList = newTreeList(false);
	
	QVBoxLayout *treeListLayout = new QVBoxLayout;
	treeListLayout->addWidget(treeList);
	treeListLayout->setMargin(0);
	treeListLayout->setSpacing(0);
	setLayout(treeListLayout);
};

QPalette bankTreeList::getPal()
{
	return this->pal;
};

QFont bankTreeList::getFont()
{
	return this->font;
};

void bankTreeList::updateSize(QRect newrect)
{
	this->setGeometry(newrect);
};

void bankTreeList::connectedToDevice()
{
	//this->treeList = newTreeList(true);
	QString snork = getPatchName(1, 1);
};

QTreeWidget* bankTreeList::newTreeList(bool connected)
{
	QTreeWidget *newTreeList = new QTreeWidget();
	newTreeList->setColumnCount(1);
	newTreeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	newTreeList->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	newTreeList->setPalette(getPal());
	newTreeList->setFont(getFont());
	
	QStringList headers;
    headers << "Boss GT-8";
    newTreeList->setHeaderLabels(headers);

	QTreeWidgetItem *user = new QTreeWidgetItem(newTreeList);
	user->setText(0, "User");
	user->setWhatsThis(0, "User Banks");
	//user->setIcon(...);

    QList<QTreeWidgetItem *> userBankRanges;
    for (int a=1; a<=35; a++)
	{
		QTreeWidgetItem* bankRange = new QTreeWidgetItem; // don't pass a parent here!
		bankRange->setText(0, QString::QString("Bank ").append(QString::number(a, 10)).append(" - ").append(QString::number(a+4, 10)) );
		bankRange->setWhatsThis(0, "");
		//bankRange->setIcon(...);

		for (int b=a; b<=(a+4); b++)
		{
			QTreeWidgetItem* bank = new QTreeWidgetItem(bankRange);
			bank->setText(0, QString::QString("Bank ").append(QString::number(b, 10)));
			bank->setWhatsThis(0, "");
			//bank->setIcon(...);

			for (int c=1; c<=4; c++)
			{
				QTreeWidgetItem* patch = new QTreeWidgetItem(bank);
				patch->setText(0, QString::QString("Patch ").append(QString::number(c, 10)));
				patch->setWhatsThis(0, "");
				//patch->setIcon(...);
			};
		};
		userBankRanges << bankRange;
		a += 4;
	};
	user->addChildren(userBankRanges);


	QTreeWidgetItem *preset = new QTreeWidgetItem(newTreeList);
	preset->setText(0, "Preset");
	preset->setWhatsThis(0, "Preset Banks");
	//user->setIcon(...);

    QList<QTreeWidgetItem *> presetBankRanges;
    for (int a=36; a<=85; a++)
	{
		QTreeWidgetItem* bankRange = new QTreeWidgetItem; // don't pass a parent here!
		bankRange->setText(0, QString::QString("Bank ").append(QString::number(a, 10)).append(" - ").append(QString::number(a+4, 10)) );
		bankRange->setWhatsThis(0, "");
		//bankRange->setIcon(...);

		for (int b=a; b<=(a+4); b++)
		{
			QTreeWidgetItem* bank = new QTreeWidgetItem(bankRange);
			bank->setText(0, QString::QString("Bank ").append(QString::number(b, 10)));
			bank->setWhatsThis(0, "");
			//bank->setIcon(...);

			for (int c=1; c<=4; c++)
			{
				QTreeWidgetItem* patch = new QTreeWidgetItem(bank);
				patch->setText(0, QString::QString("Patch ").append(QString::number(c, 10)));
				patch->setWhatsThis(0, "");
				//patch->setIcon(...);
			};
		};
		presetBankRanges << bankRange;
		a += 4;
	};
	preset->addChildren(presetBankRanges);

	newTreeList->setExpanded(newTreeList->model()->index(0, 0), true);
	newTreeList->setExpanded(newTreeList->model()->index(1, 0), true);
	return newTreeList;
};

QString bankTreeList::getPatchName(int bank, int patch)
{
	midiIO *midi = new midiIO();

	Preferences *preferences = Preferences::Instance(); bool ok;
	int midiOut = preferences->getPreferences("Midi", "MidiOut", "device").toInt(&ok, 10);
	int midiIn = preferences->getPreferences("Midi", "MidiIn", "device").toInt(&ok, 10);

	/* Patch name request. */
	MidiTable *midiTable = MidiTable::Instance();
	QString sysxOut = midiTable->nameRequest(bank, patch);
	QString replyMsg = midi->sendSysxMsg(sysxOut, midiOut, midiIn).remove(" ").toUpper();
	
	QString patchName; 
	int count = 0;
	int dataStartOffset = 11;
	QString hex1, hex2, hex3, hex4;
	for(int i=dataStartOffset*2; i<replyMsg.size()-(2*2);++i)
	{
		hex1 = replyMsg.mid(9*2, 2);
		hex2 = replyMsg.mid(10*2, 2);
		hex3 = QString::number(count, 16).toUpper();
		if (hex3.length() < 2) hex3.prepend("0");
		hex4 = replyMsg.mid(i, 2);;
		patchName.append( midiTable->getValue("Stucture", hex1, hex2, hex3, hex4) );
		i++;
	};

	return patchName.trimmed();
};