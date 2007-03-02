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
#include "floorBoardDisplay.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "SysxIO.h"
#include "sysxWriter.h"
#include "globalVariables.h"

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
	this->fontDisplay = fontDisplay;

	QTextEdit *patchNumDisplay = new QTextEdit(this);
	patchNumDisplay->setGeometry(25, 5, 50, 34);
	patchNumDisplay->setFont(fontDisplay);
	patchNumDisplay->setPalette(pal);
	patchNumDisplay->setReadOnly(true);
	patchNumDisplay->setAcceptRichText(true);
	patchNumDisplay->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchNumDisplay->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	patchNumDisplay->setLineWrapMode(QTextEdit::NoWrap);
	patchNumDisplay->setAlignment(Qt::AlignCenter);
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
	str.append("<table width='143' cellspacing='0' cellpadding='0' border='0'><tr><td align='center'>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td colspan='2' align='left'>");
	str.append("GT-8 Fx FloorBoard");
	str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
	str.append(tr("version"));
	str.append("</font></td><td align='right' valign='top'><font size='-1'>");
	str.append(version);
	str.append("</font></td></tr></table>");
	str.append("</td></tr></table>");
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
	//this->manualButton = new customButton(tr("Manual"), false, QPoint(583, 5), this, ":/images/ledbutton.png");
	//this->assignButton = new customButton(tr("Assign"), false, QPoint(583, 24), this, ":/images/pushbutton.png");
	//this->masterButton = new customButton(tr("Master"), false, QPoint(672, 5), this, ":/images/pushbutton.png");
	//this->systemButton = new customButton(tr("System"), false, QPoint(672, 24), this, ":/images/pushbutton.png");

	QObject::connect(this->connectButton, SIGNAL(valueChanged(bool)), this, SLOT(connectSignal(bool)));
	QObject::connect(this->writeButton, SIGNAL(valueChanged(bool)), this, SLOT(writeSignal(bool)));

	setInitPatchComboBox(QRect(405, 24, 168, 15));
	this->patchLoadError = false;

	timer = new QTimer(this);

	SysxIO *sysxIO = SysxIO::Instance();

	this->blinkCount = 0;

	QObject::connect(this, SIGNAL(setStatusSymbol(int)),
                sysxIO, SIGNAL(setStatusSymbol(int)));
	QObject::connect(this, SIGNAL(setStatusProgress(int)),
                sysxIO, SIGNAL(setStatusProgress(int)));
	QObject::connect(this, SIGNAL(setStatusMessage(QString)),
                sysxIO, SIGNAL(setStatusMessage(QString)));

	QObject::connect(sysxIO, SIGNAL(notConnectedSignal()),
                this, SLOT(notConnected()));
	QObject::connect(this, SIGNAL(notConnectedSignal()),
                this, SLOT(notConnected()));
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
	if(sysxIO->getFileName() == tr("init patch") || sysxIO->getFileName() == ":default.syx")
	{
		sysxIO->setFileName("");
	}
	else
	{
		if(sysxIO->getFileName() == tr("GT-8 patch"))
		{
			sysxIO->setFileName("");
			if(this->patchLoadError)
			{
				QMessageBox *msgBox = new QMessageBox();
				msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
				msgBox->setIcon(QMessageBox::Warning);
				msgBox->setTextFormat(Qt::RichText);
				QString msgText;
				msgText.append("<font size='+1'><b>");
				msgText.append(tr("Error while changing banks."));
				msgText.append("<b></font><br>");
				msgText.append(tr("An incorrect patch has been loaded. Please try to load the patch again."));
				msgBox->setText(msgText);
				msgBox->setInformativeText(tr("This is a known bug, it occures when changing the bank 'LSB'.\n"
					"For an unkown reason it didn't change."));
				msgBox->setStandardButtons(QMessageBox::Ok);
				msgBox->exec();

				sysxIO->setBank(0);
				sysxIO->setPatch(0);
			};
		};
		this->initPatchComboBox->setCurrentIndex(0);
	};	
};

void floorBoardDisplay::setPatchNumDisplay(int bank, int patch)
{
	if(bank > 0)
	{
		QString str, color;
		str.append("<html><body>");
		str.append("<font size='-1'>");
		if(bank <= bankTotalUser)
		{
			str.append("<font size='-1'>User</font>");
		}
		else
		{
			color = "color='red' ";
			str.append("<font "+color+"size='-1'>Preset</font>");
		};
		str.append("</font>");
		str.append("<table cellspacing='0' cellpadding='0' border='0'><tr>");
		str.append("<td align='right' valign='middle'>");
		str.append("<font "+color+"size='+2'>");
		if(bank < 10)
		{
			str.append("0");
		};
		str.append(QString::number(bank, 10));
		str.append("</font>");
		str.append("</td><td align='center' valign='middle'>");
		str.append("<font "+color+"size='+1'>");
		str.append(":");
		str.append("</font>");
		str.append("</td><td align='right' valign='middle'>");
		str.append("<font "+color+"size='+2'>");
		str.append(QString::number(patch, 10));
		str.append("</font>");
		str.append("</td></tr></table>");
		str.append("</body></html>");
		patchNumDisplay->setHtml(str);
	}
	else
	{
		patchNumDisplay->clear();
	};
};

void floorBoardDisplay::updateDisplay()
{
	SysxIO *sysxIO = SysxIO::Instance();
	QList<QString> nameArray = sysxIO->getFileSource("12", "00");

	MidiTable *midiTable = MidiTable::Instance();
	QString patchName;
	for(int i=sysxDataOffset;i<nameArray.size() - 2;i++ )
	{
		patchName.append( midiTable->getMidiMap("Stucture", "12", "00", "00", nameArray.at(i)).name);
	};	

	patchName = patchName.trimmed();
	if(sysxIO->getRequestName().trimmed() != patchName.trimmed())
	{
		this->patchLoadError = true;
	}
	else
	{
		this->patchLoadError = false;
	};

	patchName.replace("<", "&lt;");
	patchName.replace(">", "&gt;");
	int maxWidth = patchDisplay->width() - 2;
	int nameWidth = QFontMetrics(fontDisplay).width(patchName);

	patchName.replace("&lt;", "<font face='Verdana'>&lt;</font>");
	patchName.replace("&gt;", "<font face='Verdana'>&gt;</font>");
	patchName.replace("-", "<font face='Verdana'>-</font>");

	if(nameWidth > maxWidth)
	{
		patchName.prepend("<small>");
		patchName.append("</small>&nbsp;");
	};

	setPatchDisplay(patchName);
	if(sysxIO->isDevice())
	{
		int bank = sysxIO->getBank();
		int patch = sysxIO->getPatch();
		setPatchNumDisplay(bank, patch);
	}
	else
	{
		patchNumDisplay->clear();
	};
	valueDisplay->clear();

	if(sysxIO->isDevice())
	{
		if(sysxIO->getBank() > 35)
		{
			this->writeButton->setBlink(false);
			this->writeButton->setValue(true);
		}
		else
		{
			this->writeButton->setBlink(false);
			this->writeButton->setValue(true);
		};
		int bank = sysxIO->getBank();
		int patch = sysxIO->getPatch();
		setPatchNumDisplay(bank, patch);
	}
	else if(sysxIO->getBank() != 0)
	{
		if(sysxIO->isConnected())
		{
			this->writeButton->setBlink(true);
			this->writeButton->setValue(false);
		};
		int bank = sysxIO->getBank();
		int patch = sysxIO->getPatch();
		setPatchNumDisplay(bank, patch);
	}
	else
	{
		patchNumDisplay->clear();
		this->writeButton->setBlink(false);
		this->writeButton->setValue(false);
	};
};

void floorBoardDisplay::setInitPatchComboBox(QRect geometry)
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
				sysxIO->setSyncStatus(false);
				sysxIO->setDevice(false);
				emit updateSignal();
			};
		};
	};
};

void floorBoardDisplay::connectSignal(bool value)
{
	QString replyMsg;
	SysxIO *sysxIO = SysxIO::Instance();
	this->connectButtonActive = value;
	if(connectButtonActive == true && sysxIO->deviceReady())
	{
		emit setStatusSymbol(2);
		emit setStatusMessage(tr("Connecting"));

		this->connectButton->setBlink(true);
		sysxIO->setDeviceReady(false); // Reserve the device for interaction.

		QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)));
		QObject::connect(sysxIO, SIGNAL(sysxReply(QString)), 
			this, SLOT(connectionResult(QString)));

		sysxIO->sendSysx(idRequestString); // GT-8 Identity Request.
	}
	else
	{
		emit notConnected();
		sysxIO->setNoError(true);		// Reset the error status (else we could never retry :) ).
	};
};

void floorBoardDisplay::connectionResult(QString sysxMsg)
{
	SysxIO *sysxIO = SysxIO::Instance();
	QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)), 
			this, SLOT(connectionResult(QString)));

	sysxIO->setDeviceReady(true); // Free the device after finishing interaction.

	if(sysxIO->noError())
	{
		if(sysxMsg.contains(idReplyPatern) && connectButtonActive == true)
		{
			this->connectButton->setBlink(false);
			this->connectButton->setValue(true);
			sysxIO->setConnected(true);
			emit connectedSignal();

			emit setStatusSymbol(1);
			emit setStatusProgress(0);
			emit setStatusMessage(tr("Ready"));

			if(sysxIO->getBank() != 0)
			{
				this->writeButton->setBlink(true);
				this->writeButton->setValue(false);
			};
		}
		else if(!sysxMsg.isEmpty())
		{
			this->connectButton->setBlink(false);
			this->connectButton->setValue(false);
			sysxIO->setConnected(false);

			emit setStatusSymbol(0);
			emit setStatusProgress(0);
			emit setStatusMessage(tr("Not connected"));

			QMessageBox *msgBox = new QMessageBox();
			msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setTextFormat(Qt::RichText);
			QString msgText;
			msgText.append("<font size='+1'><b>");
			msgText.append(tr("The device connected is not a Boss GT-8 Guitar Effects Processor."));
			msgText.append("<b></font>");
			msgBox->setText(msgText);
			msgBox->setStandardButtons(QMessageBox::Ok);
			msgBox->exec();
		}
		else
		{
			this->connectButton->setBlink(false);
			this->connectButton->setValue(false);
			sysxIO->setConnected(false);

			emit setStatusSymbol(0);
			emit setStatusProgress(0);
			emit setStatusMessage(tr("Not connected"));

			QMessageBox *msgBox = new QMessageBox();
			msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setTextFormat(Qt::RichText);
			QString msgText;
			msgText.append("<font size='+1'><b>");
			msgText.append(tr("The Boss GT-8 Guitar Effects Processor was not found."));
			msgText.append("<b></font>");
			msgBox->setText(msgText);
			msgBox->setStandardButtons(QMessageBox::Ok);
			msgBox->exec();
		};
	}
	else
	{
		notConnected();
		sysxIO->setNoError(true);		// Reset the error status (else we could never retry :) ).
	};
};

void floorBoardDisplay::writeSignal(bool value)
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(sysxIO->isConnected() && sysxIO->deviceReady()) /* Check if we are connected and if the device is free. */
	{
		this->writeButton->setBlink(true);

		if(sysxIO->getBank() == 0) /* Check if a bank is sellected. */
		{
			QMessageBox *msgBox = new QMessageBox();
			msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
			msgBox->setIcon(QMessageBox::Warning);
			msgBox->setTextFormat(Qt::RichText);
			QString msgText;
			msgText.append("<font size='+1'><b>");
			msgText.append(tr("You didn't select a bank to write to."));
			msgText.append("<b></font><br>");
			msgText.append(tr("Please select a user bank to write this patch to and try again."));
			msgBox->setText(msgText);
			msgBox->setStandardButtons(QMessageBox::Ok);
			msgBox->exec();
			this->writeButton->setBlink(false);
			this->writeButton->setValue(false);
		}
		else /* Bank is sellected. */
		{
			sysxIO->setDeviceReady(false);			// Reserve the device for interaction.

			if(!sysxIO->getSyncStatus())			// Check if the data is allready in sync. with the device.
			{	/* If not we send the data to the (temp) buffer. So we don't change the patch default address "0D 00". */

				
				if(sysxIO->getBank() != sysxIO->getLoadedBank() || sysxIO->getPatch() != sysxIO->getLoadedPatch())// Check if a different patch is sellected
				{															// else load the selected one.
					emit setStatusSymbol(2);
					emit setStatusProgress(0);
					emit setStatusMessage("Sending");
					
					int bank = sysxIO->getBank();
					int patch = sysxIO->getPatch();
					patchLoadSignal(bank, patch);	// Tell to stop blinking a sellected patch and prepare to load this one instead.
					setPatchNumDisplay(bank, patch);

					QObject::disconnect(sysxIO, SIGNAL(isChanged()),	
						this, SLOT(writeToBuffer()));
					QObject::connect(sysxIO, SIGNAL(isChanged()),	// Connect the isChanged message
						this, SLOT(writeToBuffer()));				// to writeToBuffer.

					sysxIO->requestPatchChange(bank, patch);
				}
				else
				{
					writeToBuffer();
				};		
			}
			else /* If sync we will write (save) the patch directly to sellected bank. So we will have to change the patch adsress */
			{
				if(sysxIO->getBank() > bankTotalUser) // Preset banks are NOT writable so we check.
				{
					QMessageBox *msgBox = new QMessageBox();
					msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
					msgBox->setIcon(QMessageBox::Warning);
					msgBox->setTextFormat(Qt::RichText);
					QString msgText;
					msgText.append("<font size='+1'><b>");
					msgText.append(tr("You can't write to the preset banks."));
					msgText.append("<b></font><br>");
					msgText.append(tr("Please select a user bank to write this patch to and try again."));
					msgBox->setText(msgText);
					msgBox->setStandardButtons(QMessageBox::Ok);
					msgBox->exec();
					this->writeButton->setBlink(false); // Allready sync with the buffer so no blinking
					this->writeButton->setValue(true);	// and so we will also leave the write button active.
					sysxIO->setDeviceReady(true);
				}
				else /* User bank so we can write to it after confirmation to overwrite stored data. */
				{
					QMessageBox *msgBox = new QMessageBox();
					msgBox->setWindowTitle(tr("GT-8 Fx FloorBoard"));
					msgBox->setIcon(QMessageBox::Warning);
					msgBox->setTextFormat(Qt::RichText);
					QString msgText;
					msgText.append("<font size='+1'><b>");
					msgText.append(tr("You have chosen to write the patch permanently into memory."));
					msgText.append("<b></font><br>");
					msgText.append(tr("This will overwrite the patch currently stored at this location\n"
						"and can't be undone."));
					msgBox->setInformativeText(tr("Are you sure you want to continue?"));
					msgBox->setText(msgText);
					msgBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No);

					if(msgBox->exec() == QMessageBox::Yes)
					{	/* Accepted to overwrite data. So now we have to set destination address by replacing the default (buffer). */
						
						writeToMemory();
					}
					else
					{
						sysxIO->setDeviceReady(true);
						this->writeButton->setBlink(false);
						this->writeButton->setValue(true);
					};					
				};
			};

			/* DEBUGGING OUTPUT 
			QString snork;
			for(int i=0;i<sysxOut.size();++i)
			{
				snork.append(sysxOut.mid(i, 2));
				snork.append(" ");
				i++;
			};
			snork.replace("F7", "F7 }\n");
			snork.replace("F0", "{ F0");
			snork.append("\n{ size=");
			snork.append(QString::number(sysxOut.size()/2, 10));
			snork.append("}");		

			QMessageBox *msgBox = new QMessageBox();
			msgBox->setWindowTitle("dBug Result");
			msgBox->setIcon(QMessageBox::Information);
			msgBox->setText(snork);
			msgBox->setStandardButtons(QMessageBox::Ok);
			msgBox->exec();*/
		};
	}
	else if(sysxIO->isConnected() != true) /* We are NOT connected */
	{
		notConnected();
	}
	else /* The device was NOT free. */
	{
		//emit notConnected();
	};
};

void floorBoardDisplay::writeToBuffer() 
{
	SysxIO *sysxIO = SysxIO::Instance();
	QObject::disconnect(sysxIO, SIGNAL(isChanged()),	
					this, SLOT(writeToBuffer()));

	QString sysxMsg;
	QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
	QList<QString> patchAddress = sysxIO->getFileSource().address;

	emit setStatusSymbol(2);
	emit setStatusProgress(0);
	emit setStatusMessage(tr("Sending"));

	for(int i=0;i<patchData.size();++i) // Prepare the data to be send at ones.
	{
		QList<QString> data = patchData.at(i);
		for(int x=0;x<data.size();++x)
		{
			sysxMsg.append(data.at(x));
		};
	}; 
	sysxIO->setSyncStatus(true);		// Inadvance of the actuale data transfer we set it allready to sync.
	this->writeButton->setBlink(false);	// Sync so we stop blinking the button
	this->writeButton->setValue(true);	// and activate the write button.

	QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),	// Connect the result signal 
		this, SLOT(resetDevice(QString)));					// to a slot that will reset the device after sending.

	sysxIO->sendSysx(sysxMsg);								// Send the data.
};

void floorBoardDisplay::writeToMemory() 
{
	SysxIO *sysxIO = SysxIO::Instance();

	QString sysxMsg; bool ok;
	QList< QList<QString> > patchData = sysxIO->getFileSource().hex; // Get the loaded patch data.
	QList<QString> patchAddress = sysxIO->getFileSource().address;

	emit setStatusSymbol(2);
	emit setStatusProgress(0);
	emit setStatusMessage(tr("Sending"));

	int bank = sysxIO->getBank();
	int patch = sysxIO->getPatch();
	int patchOffset = (((bank - 1 ) * patchPerBank) + patch) - 1;
	int memmorySize = QString("7F").toInt(&ok, 16) + 1;
	int emptyAddresses = (memmorySize) - ((bankTotalUser * patchPerBank) - (memmorySize));
	if(bank > bankTotalUser) patchOffset += emptyAddresses; //System patches start at a new memmory range.
	int addrMaxSize = QString("80").toInt(&ok, 16);
	int n = (int)(patchOffset / addrMaxSize);
	
	QString addr1 = QString::number(8 + n, 16).toUpper();
	QString addr2 = QString::number(patchOffset - (addrMaxSize * n), 16).toUpper();
	
	for(int i=0;i<patchData.size();++i)
	{
		QList<QString> data = patchData.at(i);
		for(int x=0;x<data.size();++x)
		{
			QString hex;
			if(x == sysxAddressOffset)
			{ 
				hex = addr1;
			}
			else if(x == sysxAddressOffset + 1)
			{
				hex = addr2;
			}
			else
			{
				hex = data.at(x);
			};
			if (hex.length() < 2) hex.prepend("0");
			sysxMsg.append(hex);
		};
	};
	sysxIO->setSyncStatus(true);		// Still in sync
	this->writeButton->setBlink(false); // so no blinking here either...
	this->writeButton->setValue(true);	// ... and still the button will be active also ...

	QObject::connect(sysxIO, SIGNAL(sysxReply(QString)),	// Connect the result signal 
		this, SLOT(resetDevice(QString)));					// to a slot that will reset the device after sending.

	sysxIO->sendSysx(sysxMsg);								// Send the data.
};

void floorBoardDisplay::patchChangeFailed()
{
	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setBank(sysxIO->getLoadedBank());
	sysxIO->setPatch(sysxIO->getLoadedPatch());
	setPatchNumDisplay(sysxIO->getLoadedBank(), sysxIO->getLoadedPatch());
};

void floorBoardDisplay::resetDevice(QString replyMsg) 
{
	SysxIO *sysxIO = SysxIO::Instance();
	QObject::disconnect(sysxIO, SIGNAL(sysxReply(QString)),	
				this, SLOT(resetDevice(QString)));

	if(sysxIO->getBank() != sysxIO->getLoadedBank() || sysxIO->getPatch() != sysxIO->getLoadedPatch())
	{
		sysxIO->setLoadedBank(sysxIO->getBank());
		sysxIO->setLoadedPatch(sysxIO->getPatch());
	};
	sysxIO->setDeviceReady(true);	// Free the device after finishing interaction.
	emit connectedSignal();			// Emit this signal to tell we are still connected and to update the patch names in case they have changed.
};

void floorBoardDisplay::patchSelectSignal(int bank, int patch)
{
	SysxIO *sysxIO = SysxIO::Instance();
	if(blinkCount == 0)
	{
		currentSyncStatus = sysxIO->getSyncStatus();
		sysxIO->setSyncStatus(false);
		writeButton->setBlink(true);
	};

	if( sysxIO->getLoadedBank() != bank ||  sysxIO->getLoadedPatch() != patch)
	{
		sysxIO->setBank(bank);
		sysxIO->setPatch(patch);
		
		if(blinkCount == 0)
		{	
			QObject::connect(timer, SIGNAL(timeout()), this, SLOT(blinkSellectedPatch()));
			timer->start(sellectionBlinkInterval);
		}
		else
		{
			blinkCount = 0;
		};
	}
	else
	{
		blinkSellectedPatch(false);
	};
};

void floorBoardDisplay::blinkSellectedPatch(bool active)
{
	SysxIO *sysxIO = SysxIO::Instance();
	int bank = sysxIO->getBank();
	int patch = sysxIO->getPatch();
	
	if(active && blinkCount <= (sellectionBlinks * 2) - 1)
	{
		if(blinkCount % 2 == 0)
		{
			patchNumDisplay->clear();
		}
		else
		{
			setPatchNumDisplay(bank, patch);
		};
		blinkCount++;
	}
	else
	{
		QObject::disconnect(timer, SIGNAL(timeout()), this, SLOT(blinkSellectedPatch()));
		timer->stop();
		blinkCount = 0;
		sysxIO->setBank(sysxIO->getLoadedBank());
		sysxIO->setPatch(sysxIO->getLoadedPatch());
		sysxIO->setSyncStatus(currentSyncStatus);
		if(currentSyncStatus || sysxIO->getLoadedBank() == 0)
		{
			writeButton->setBlink(false);
		};
		setPatchNumDisplay(sysxIO->getLoadedBank(),  sysxIO->getLoadedPatch());
	};
};

void floorBoardDisplay::patchLoadSignal(int bank, int patch)
{
	blinkSellectedPatch(false);

	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setBank(bank);
	sysxIO->setPatch(patch);
};

void floorBoardDisplay::notConnected()
{
	emit setStatusSymbol(0);
	emit setStatusProgress(0);
	emit setStatusMessage(tr("Not connected"));

	this->connectButton->setBlink(false);
	this->connectButton->setValue(false);	
	this->writeButton->setBlink(false);
	this->writeButton->setValue(false);

	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setConnected(false);
	sysxIO->setSyncStatus(false);	
	sysxIO->setDeviceReady(true);	// Free the device after finishing interaction.	
};
