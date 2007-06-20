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

#include <QLayout>
#include <QMessageBox>
#include "bankTreeList.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "globalVariables.h"

bankTreeList::bankTreeList(QWidget *parent)
    : QWidget(parent)
{
	QFont font;
	font.setStretch(85);

	this->treeList = newTreeList();
	this->treeList->setObjectName("banklist");

	QObject::connect(treeList, SIGNAL(itemExpanded(QTreeWidgetItem*)), 
			this, SLOT(setOpenItems(QTreeWidgetItem*)));
	QObject::connect(treeList, SIGNAL(itemCollapsed(QTreeWidgetItem*)), 
			this, SLOT(setClosedItems(QTreeWidgetItem*)));
	QObject::connect(treeList, SIGNAL(itemClicked(QTreeWidgetItem*, int)), 
			this, SLOT(setItemClicked(QTreeWidgetItem*, int)));
	QObject::connect(treeList, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), 
			this, SLOT(setItemDoubleClicked(QTreeWidgetItem*, int)));

	QObject::connect(this, SIGNAL(updateSignal()), 
			this->parent(), SIGNAL(updateSignal()));
	
	QVBoxLayout *treeListLayout = new QVBoxLayout;
	treeListLayout->addWidget(treeList);
	treeListLayout->setMargin(0);
	treeListLayout->setSpacing(0);
	setLayout(treeListLayout);

	SysxIO *sysxIO = SysxIO::Instance();
	QObject::connect(this, SIGNAL(setStatusSymbol(int)),
                sysxIO, SIGNAL(setStatusSymbol(int)));
	QObject::connect(this, SIGNAL(setStatusProgress(int)),
                sysxIO, SIGNAL(setStatusProgress(int)));
	QObject::connect(this, SIGNAL(setStatusMessage(QString)),
                sysxIO, SIGNAL(setStatusMessage(QString)));

	QObject::connect(this, SIGNAL(notConnectedSignal()),
                sysxIO, SIGNAL(notConnectedSignal()));
};

void bankTreeList::updateSize(QRect newrect)
{
	this->setGeometry(newrect);
};

void bankTreeList::setClosedItems(QTreeWidgetItem *item)
{
	if(item->childCount() == patchPerBank)
	{
		int index = openPatchTreeItems.indexOf(item);
		openPatchTreeItems.removeAt(index);
	}
	else if(item->childCount() == 5)
	{
		int index = openBankTreeItems.indexOf(item);
		openBankTreeItems.removeAt(index);
		closeChildren(item);
	}
	else
	{
		closeChildren(item);
	};
};

void bankTreeList::closeChildren(QTreeWidgetItem *item)
{
	for(int i=0;i<item->childCount();++i)
	{
		for(int n=0;n<item->child(i)->childCount();++n)
		{
			item->child(i)->child(n)->setExpanded(false);
		};
		item->child(i)->setExpanded(false);
	};
};

void bankTreeList::setOpenItems(QTreeWidgetItem *item)
{
	QString type = "root";
	if(item->childCount() == patchPerBank)
	{
		openPatchTreeItems.append(item);
		updateTree(item);
		type = "patch";
	}
	else if(item->childCount() == 5)
	{
		openBankTreeItems.append(item);
		type = "bank";
	};
	
	int c = openPatchTreeItems.size();
	int b = openBankTreeItems.size();
	int a = 0;
	if(treeList->isExpanded(treeList->model()->index(0, 0)) && 
		treeList->isExpanded(treeList->model()->index(1, 0)))
	{
		a = 2;
	}
	else if(treeList->isExpanded(treeList->model()->index(0, 0)) || 
		treeList->isExpanded(treeList->model()->index(1, 0)))
	{
		a = 1;
	};

	int userBankCount = 0;
	for(int i=0;i<openBankTreeItems.size();++i)
	{
		if(openBankTreeItems.at(i)->parent()->text(0).contains("User"))
		{
			userBankCount++;
		};
	};


	if(type == "root")
	{
		if(a > 1)
		{
			int maxExpandedItems;
			while(c > 3) 
			{
				openPatchTreeItems.first()->setExpanded(false); 
				c = openPatchTreeItems.size();			
			};
			switch(c)
			{
				case 0: maxExpandedItems = 3; break;
				case 1: maxExpandedItems = 2; break;
				case 2: maxExpandedItems = 1; break;
				case 3: maxExpandedItems = 1; break;
			};
			
			while(b > maxExpandedItems)
			{
				openBankTreeItems.first()->setExpanded(false);;
				b = openBankTreeItems.size();
			};
		};
	}
	else if(type == "bank")
	{
		int maxExpandedItems;
		if(a > 1)
		{	
			switch(c)
			{
				case 0: maxExpandedItems = 3; break;
				case 1: maxExpandedItems = 2; break;
				case 2: maxExpandedItems = 1; break;
				case 3: maxExpandedItems = 1; break;
			};
		}
		else
		{
			if(treeList->isExpanded(treeList->model()->index(0, 0)))
			{
				switch(c)
				{
					case 0: maxExpandedItems = 5; break;	
					case 1: maxExpandedItems = 4; break;
					case 2: maxExpandedItems = 3; break;
					case 3: maxExpandedItems = 3; break;
					case 4: maxExpandedItems = 2; break;
					case 5: maxExpandedItems = 1; break;
				};
			}
			else
			{
				switch(c)
				{
					case 0: maxExpandedItems = 4; break;	
					case 1: maxExpandedItems = 4; break;
					case 2: maxExpandedItems = 3; break;
					case 3: maxExpandedItems = 3; break;
					case 4: maxExpandedItems = 1; break;
					case 5: maxExpandedItems = 1; break;
				};
			};
		};

		if(maxExpandedItems == 1)
		{
			openPatchTreeItems.first()->setExpanded(false);
			if(treeList->isExpanded(treeList->model()->index(1, 0)))
			{
				openPatchTreeItems.first()->setExpanded(false);
			}
			maxExpandedItems = 2;
		};
		if(b > maxExpandedItems)
		{
			openBankTreeItems.first()->setExpanded(false);;
		};
	}
	else if(type == "patch")
	{
		for(int i=0;i<openBankTreeItems.size();++i)
		{
			bool allCollapsed = true;
			for(int n=0;n<openBankTreeItems.at(i)->childCount();++n)
			{
				if(openBankTreeItems.at(i)->child(n)->isExpanded())
				{
					allCollapsed = false;
				};
			};
			if(allCollapsed)
			{
				openBankTreeItems.at(i)->setExpanded(false);
				b = openBankTreeItems.size();
				break;
			};
		};

		int maxExpandedItems;
		if(a > 1)
		{	
			switch(b)
			{
				case 1: maxExpandedItems = 3; break;
				case 2: maxExpandedItems = 1; break;
				case 3: maxExpandedItems = 0; break;
			};
		}
		else
		{
			if(treeList->isExpanded(treeList->model()->index(0, 0)))
			{
				switch(b)
				{
					case 1: maxExpandedItems = 5; break;
					case 2: maxExpandedItems = 4; break;
					case 3: maxExpandedItems = 3; break;
					case 4: maxExpandedItems = 1; break;
				};
			}
			else
			{
				switch(b)
				{
					case 1: maxExpandedItems = 4; break;
					case 2: maxExpandedItems = 3; break;
					case 3: maxExpandedItems = 3; break;
					case 4: maxExpandedItems = 1; break;
				};
			};
		};

		if(maxExpandedItems == 0)
		{
			openBankTreeItems.first()->setExpanded(false);
			maxExpandedItems = 1;
		};
		if(c > maxExpandedItems)
		{
			openPatchTreeItems.first()->setExpanded(false);
		};
	};
};

QTreeWidget* bankTreeList::newTreeList()
{
	QTreeWidget *newTreeList = new QTreeWidget();
	newTreeList->setColumnCount(1);
	newTreeList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	newTreeList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Qt::ScrollBarAsNeeded
	
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

/*********************** setItemClicked() ***********************************
 * Expands and colapses on a single click and sets patch sellection.
 ****************************************************************************/
void bankTreeList::setItemClicked(QTreeWidgetItem *item, int column)
{
	column; // not used
	if(item->childCount() != 0)
	{
		if(item->isExpanded())
		{
			item->setExpanded(false);
		}
		else
		{
			item->setExpanded(true);
		};
	} 
	else if (item->childCount() == 0)
	{
		SysxIO *sysxIO = SysxIO::Instance();
		if(sysxIO->isConnected() && sysxIO->deviceReady())
		{
			bool ok;
			int bank = item->parent()->text(0).section(" ", 1, 1).trimmed().toInt(&ok, 10);
			int patch = item->parent()->indexOfChild(item) + 1;

			emit patchSelectSignal(bank, patch);
			sysxIO->setRequestName(item->text(0));	// Set the name of the patch we have sellected in case we load it.
		};
	};
};

/*********************** setItemDoubleClicked() *****************************
 * Handels when a patch is double clicked in the tree list. Patch will be 
 * loaded into the temp buffer and will tell to request the data afterwards.
 ****************************************************************************/
void bankTreeList::setItemDoubleClicked(QTreeWidgetItem *item, int column)
{	
	column; // not used
	SysxIO *sysxIO = SysxIO::Instance();
	if(item->childCount() == 0 && sysxIO->deviceReady() && sysxIO->isConnected()) 
		// Make sure it's a patch (Patches are the last in line so no children).
	{
		emit setStatusSymbol(2);
		//emit setStatusProgress(0);
		emit setStatusMessage(tr("Sending"));
		
		sysxIO->setDeviceReady(false);
		sysxIO->setRequestName(item->text(0));	// Set the name of the patch we are going to load, so we can check if we have loaded the correct patch at the end.

		bool ok;
		int bank = item->parent()->text(0).section(" ", 1, 1).trimmed().toInt(&ok, 10); // Get the bank
		int patch = item->parent()->indexOfChild(item) + 1;								// and the patch number.
		
		if(bank == sysxIO->getLoadedBank() && patch == sysxIO->getLoadedPatch())
		{
			requestPatch(bank, patch);
		}
		else
		{
			emit patchLoadSignal(bank, patch); // Tell to stop blinking a sellected patch and prepare to load this one instead.
			
			QObject::disconnect(sysxIO, SIGNAL(isChanged()),	
				this, SLOT(requestPatch()));
			QObject::connect(sysxIO, SIGNAL(isChanged()),	// Connect the isChanged message
				this, SLOT(requestPatch()));				// to requestPatch.

			sysxIO->requestPatchChange(bank, patch);
		};
	};
};
/*********************** requestPatch() *******************************
 * Does the actual requesting of the patch data and hands the 
 * reception over to updatePatch function.
 **********************************************************************/
void bankTreeList::requestPatch() 
{
	SysxIO *sysxIO = SysxIO::Instance();
	
	QObject::disconnect(sysxIO, SIGNAL(isChanged()),	
			this, SLOT(requestPatch()));
	
	if(sysxIO->isConnected())
	{
		QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),	// Connect the result of the request
			this, SLOT(updatePatch(QString)));					// to updatePatch function.

		emit setStatusSymbol(3);
		//emit setStatusProgress(0);
		emit setStatusMessage(tr("Receiving"));
		
		sysxIO->requestPatch(0, 0);
	};
};

void bankTreeList::requestPatch(int bank, int patch) 
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(sysxIO->isConnected())
	{
		QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),	// Connect the result of the request
			this, SLOT(updatePatch(QString)));					// to updatePatch function.

		emit setStatusSymbol(3);
		//emit setStatusProgress(0);
		emit setStatusMessage(tr("Receiving"));
		
		sysxIO->requestPatch(bank, patch);
	};
};

/*********************** updatePatch() *******************************
 * Updates the source of the currently handeled patch and set the 
 * attributes accordingly.
 *********************************************************************/
void bankTreeList::updatePatch(QString replyMsg)
{
	emit setStatusSymbol(1);
	emit setStatusMessage(tr("Ready"));

	SysxIO *sysxIO = SysxIO::Instance();

	sysxIO->setDeviceReady(true); // Free the device after finishing interaction.
	
	QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)),
		this, SLOT(updatePatch(QString)));		
	
	replyMsg = replyMsg.remove(" ").toUpper();
	if(replyMsg != "" && replyMsg.size()/2 == 1010)
	{
		sysxIO->setFileSource(replyMsg);		// Set the source to the data received.
		sysxIO->setFileName(tr("GT-8 patch"));	// Set the file name to GT-8 patch forthe display.
		sysxIO->setDevice(true);				// Patch received from the device so this is set to true.
		sysxIO->setSyncStatus(true);			// We can't be more in sync than right now! :)

		sysxIO->setLoadedBank(sysxIO->getBank());
		sysxIO->setLoadedPatch(sysxIO->getPatch());

		emit updateSignal();
		emit setStatusProgress(0);
	}
	else
	{
		emit notConnectedSignal();				// No message returned so connection must be lost.
	};

	/* DEBUGGING OUTPUT 
	QString snork;
	for(int i=0;i<replyMsg.size();++i)
	{
		snork.append(replyMsg.mid(i, 2));
		snork.append(" ");
		i++;
	};
	snork.replace("F7", "F7 }\n");
	snork.replace("F0", "{ F0");
	snork.append("\n{ size=");
	snork.append(QString::number(replyMsg.size()/2, 10));
	snork.append("}");		

	QMessageBox *msgBox = new QMessageBox();
	msgBox->setWindowTitle("dBug Result");
	msgBox->setIcon(QMessageBox::Information);
	msgBox->setText(snork);
	msgBox->setStandardButtons(QMessageBox::Ok);
	msgBox->exec();*/
};

/********************************** connectedSignal() ****************************
* This slot reloads all patch names of expanded items, if any, on (re)connection to a
* device.
*********************************************************************************/
void bankTreeList::connectedSignal()
{	
	SysxIO *sysxIO = SysxIO::Instance();
	if(this->openPatchTreeItems.size() != 0 && sysxIO->deviceReady() && sysxIO->isConnected())
	{
		sysxIO->setDeviceReady(false);

		this->listIndex = 0;
		this->itemIndex = 0;

		QObject::disconnect(sysxIO, SIGNAL(patchName(QString)),
			this, SLOT(updatePatchNames(QString)));
		QObject::connect(sysxIO, SIGNAL(patchName(QString)),
			this, SLOT(updatePatchNames(QString)));

		this->currentPatchTreeItems.clear();
		this->currentPatchTreeItems = this->openPatchTreeItems;
		qSort(this->currentPatchTreeItems);

		this->updatePatchNames("");
	};
};

/********************************** updateTree() ********************************
* This handles whether we add the newly expanded item to the current job or 
* start a new one.
*********************************************************************************/
void bankTreeList::updateTree(QTreeWidgetItem *item)
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(sysxIO->deviceReady() && sysxIO->isConnected())
	{
		sysxIO->setDeviceReady(false);

		this->listIndex = 0;
		this->itemIndex = 0;

		QObject::disconnect(sysxIO, SIGNAL(patchName(QString)),
			this, SLOT(updatePatchNames(QString)));
		QObject::connect(sysxIO, SIGNAL(patchName(QString)),
			this, SLOT(updatePatchNames(QString)));

		this->currentPatchTreeItems.append(item);
		this->updatePatchNames("");
	}
	else
	{
		this->currentPatchTreeItems.append(item);
	};
};

/***************************** updatePatchNames() ********************************
* This updates the patch names in the treeList of all expanded items. this is 
* done when a bank is expanded or when we (re)connect to a device.
*********************************************************************************/
void bankTreeList::updatePatchNames(QString name)
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(sysxIO->isConnected())
	{
		emit setStatusSymbol(3);
		emit setStatusMessage(tr("Receiving"));

		if(name != "") // If not empty we can asume that we did receive a patch name.
		{
			this->currentPatchTreeItems.at(listIndex)->child(itemIndex)->setText(0, name); // Set the patch name of the item in the tree list.
			if(itemIndex >= patchPerBank - 1) // If we reach the last patch in this bank we need to increment the bank and restart at patch 1.
			{
				this->listIndex++;
				this->itemIndex = 0;
			}
			else
			{
				this->itemIndex++;
			};
		};

		if(listIndex < currentPatchTreeItems.size()) // Aslong as we have items in the list we continue, duh! :)
		{		
			bool ok;
			int bank = this->currentPatchTreeItems.at(listIndex)->text(0).section(" ", 1, 1).trimmed().toInt(&ok, 10);
			int patch = itemIndex + 1 ;

			sysxIO->requestPatchName(bank, patch); // The patch name request.
		}
		else
		{
			sysxIO->setDeviceReady(true);

			this->currentPatchTreeItems.clear(); // We are done so we can safely reset items taht need to be named.
			this->listIndex = 0;
			this->itemIndex = 0;

			emit setStatusSymbol(1);
			emit setStatusMessage(tr("Ready"));
			emit setStatusProgress(0);
		};
	};
};
