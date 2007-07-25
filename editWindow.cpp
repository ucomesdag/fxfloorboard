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

#include "editWindow.h"
#include "MidiTable.h"
#include "SysxIO.h"

editWindow::editWindow(QWidget *parent)
    : QWidget(parent)
{
	this->image = QPixmap(":images/editwindow.png");
	this->setFixedSize(image.width(), image.height());
	/*this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setWindowFlags(Qt::WindowStaysOnTopHint 
		| Qt::WindowTitleHint 
		| Qt::WindowMinimizeButtonHint 
		| Qt::MSWindowsFixedSizeDialogHint);*/

	this->title = new QLabel;
	this->title->setObjectName("title");

	this->comboBoxLabel = new QLabel(tr("Select"));
	this->comboBoxLabel->setObjectName("selectlabel");
	this->comboBoxLabel->setVisible(false);

	this->pageComboBox = new QComboBox;
	this->pageComboBox->setObjectName("smallcombo");
	this->pageComboBox->setEditable(false);
	this->pageComboBox->setFrame(false);
	this->pageComboBox->setVisible(false);

	this->closeButton = new customControlLabel;
	this->closeButton->setButton(true);
	this->closeButton->setImage(":/images/closebutton.png");

	QHBoxLayout *headerLayout = new QHBoxLayout;
	headerLayout->addWidget(this->title);
	headerLayout->addStretch();
	headerLayout->addWidget(this->comboBoxLabel);
	headerLayout->addWidget(this->pageComboBox);
	headerLayout->addStretch();
	headerLayout->addWidget(this->closeButton);

	/*QHBoxLayout *sellectLayout = new QHBoxLayout;
	sellectLayout->addStretch();
	sellectLayout->addWidget(this->pageComboBox);
	sellectLayout->addStretch();*/	

	this->pagesWidget = new QStackedWidget;

	QHBoxLayout *pagesLayout = new QHBoxLayout;
	pagesLayout->addStretch();
	pagesLayout->addWidget(this->pagesWidget);
	pagesLayout->addStretch();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(headerLayout);
	mainLayout->addStretch();
	//mainLayout->addLayout(sellectLayout);
	mainLayout->addLayout(pagesLayout);
	mainLayout->addStretch();
	mainLayout->addSpacing(16);
	setLayout(mainLayout);

	this->tempPage = new editPage;

	QObject::connect(this->pageComboBox, SIGNAL(activated(int)),
             this->pagesWidget, SLOT(setCurrentIndex(int)));

	QObject::connect(this->closeButton, SIGNAL(mouseReleased()),
             this, SLOT(hide()));

	/*QObject::connect(this, SIGNAL( updateSignal() ),
            this->parent(), SIGNAL( updateSignal() ));*/

	QObject::connect(this, SIGNAL( dialogUpdateSignal() ),
                this, SLOT( pageUpdateSignal() ));

	QObject::connect(this->pageComboBox, SIGNAL(activated(int)),
                this, SLOT(valueChanged(int)));
};

void editWindow::paintEvent(QPaintEvent *)
{
	QRectF target(0.0, 0.0, image.width(), image.height());
	QRectF source(0.0, 0.0, image.width(), image.height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
};

editWindow::~editWindow()
{

};

void editWindow::setLSB(QString hex1, QString hex2)
{
	this->hex1 = hex1;
	this->hex2 = hex2;
};

void editWindow::setWindow(QString title)
{
	this->title->setText(title);
	this->pagesWidget->setCurrentIndex(0);
};

QString editWindow::getTitle()
{
	return this->title->text();
};

void editWindow::addPage(QString hex1, QString hex2, QString hex3, QString hex4)
{
	this->hex1 = hex1;
	this->hex2 = hex2;
	this->hex3 = hex3;	
	
	this->tempPage->setGridLayout();
	this->editPages.append(this->tempPage); 
	this->pagesWidget->addWidget(editPages.last()); 
	this->pages = this->pagesWidget->count(); 

	QObject::connect(this, SIGNAL( dialogUpdateSignal() ),
			editPages.last(), SIGNAL( dialogUpdateSignal() ));

	QObject::connect(editPages.last(), SIGNAL( updateSignal() ),
		this, SIGNAL( updateSignal() ));
	
	if(hex1 != "void" && hex2 != "void" && hex3 != "void")
	{
		MidiTable *midiTable = MidiTable::Instance();
		Midi items = midiTable->getMidiMap("Structure", hex1, hex2, hex3);
		
		int itemsCount;
		if(hex4 == "void")
		{
			itemsCount = this->pagesWidget->count() - 1;
		}
		else
		{
			bool ok;
			itemsCount = hex4.toInt(&ok, 16);
		};

		QString item;
		QString desc = items.level.at(itemsCount).desc;
		QString customdesc = items.level.at(itemsCount).customdesc;
		if(!customdesc.isEmpty())
		{
			item = customdesc;
		}
		else
		{
			item = desc;
		};
		//int pixelWidth = QFontMetrics(this->getFont()).width(item);
		//if(maxLenght < pixelWidth) maxLenght = pixelWidth;

		this->pageComboBox->addItem(item); 
		this->tempPage = new editPage; 

		this->pageComboBox->setMaxVisibleItems(this->pages);
		//this->pageComboBox->view()->setMinimumWidth( this->comboWidth );

		if(this->pages > 1)
		{
			this->pageComboBox->setVisible(true);
			this->comboBoxLabel->setVisible(true);
		};
	};
};

void editWindow::valueChanged(int index)
{
	QString valueHex = QString::number(index, 16).toUpper();
	if(valueHex.length() < 2) valueHex.prepend("0");

	SysxIO *sysxIO = SysxIO::Instance();
	sysxIO->setFileSource(this->hex1, this->hex2, this->hex3, valueHex);

	//emit updateDisplay(valueStr);
	emit updateSignal();
};

void editWindow::pageUpdateSignal()
{
	if(this->pages > 1)
	{
		SysxIO *sysxIO = SysxIO::Instance();
		int index = sysxIO->getSourceValue(this->hex1, this->hex2, this->hex3);
		this->pageComboBox->setCurrentIndex(index);
		this->pagesWidget->setCurrentIndex(index);
		//this->valueChanged(index);
	};
};

editPage* editWindow::page()
{
	return this->tempPage;
};

void editWindow::closeEvent(QCloseEvent* ce)
{
	ce->accept();
};
