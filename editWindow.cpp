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

	QFont titleFont;
	titleFont.setFamily("Arial");
	titleFont.setBold(true);
	titleFont.setPixelSize(16);
	titleFont.setStretch(140);

	QPalette titlePalette;
	titlePalette.setColor(this->title->foregroundRole(), Qt::white);
	this->title->setPalette(titlePalette);
	this->title->setFont(titleFont);

	this->pageComboBox = new QComboBox;
	this->pageComboBox->setVisible(false);

	this->closeButton = new customControlLabel;
	this->closeButton->setButton(true);
	this->closeButton->setImage(":/images/closebutton.png");

	QHBoxLayout *headerLayout = new QHBoxLayout;
	headerLayout->addWidget(this->title);
	headerLayout->addStretch();
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

void editWindow::addPage()
{
	this->tempPage->setGridLayout();
	this->editPages.append(this->tempPage); 
    this->pagesWidget->addWidget(editPages.last()); 
    int pages = this->pagesWidget->count(); 
    QString item; 
    item.append("Page "); 
    item.append(QString::number(pages, 10)); 
    this->pageComboBox->addItem(item); 
    this->tempPage = new editPage; 

	if(pages > 1)
	{
		this->pageComboBox->setVisible(true);
	};

	/*QObject::connect(this, SIGNAL( dialogUpdateSignal() ),
		editPages.last(), SIGNAL( dialogUpdateSignal() ));
	QObject::connect(editPages.last(), SIGNAL( updateSignal() ),
		this, SIGNAL( updateSignal() ));*/
};

editPage* editWindow::page()
{
	return this->tempPage;
};

void editWindow::closeEvent(QCloseEvent* ce)
{
	ce->accept();
};
