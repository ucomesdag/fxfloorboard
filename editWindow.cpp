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
	this->setFixedSize(450, 350);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	this->setWindowFlags(Qt::WindowStaysOnTopHint 
		| Qt::WindowTitleHint 
		| Qt::WindowMinimizeButtonHint 
		| Qt::MSWindowsFixedSizeDialogHint);

	this->title = new QLabel;

	QFont titleFont;
	titleFont.setFamily("Arial");
	titleFont.setBold(true);
	titleFont.setPixelSize(14);
	titleFont.setStretch(140);

	QPalette palette;
	palette.setColor(this->title->foregroundRole(), Qt::white);
	this->title->setPalette(palette);
	this->title->setFont(titleFont);

	this->pageComboBox = new QComboBox;
	this->pageComboBox->setVisible(false);

	QHBoxLayout *headerLayout = new QHBoxLayout;
	headerLayout->addWidget(this->title);
	headerLayout->addWidget(this->pageComboBox);

	this->pagesWidget = new QStackedWidget;

	QVBoxLayout *pagesLayout = new QVBoxLayout;
	pagesLayout->addWidget(this->pagesWidget);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(headerLayout);
	mainLayout->addSpacing(5);
	mainLayout->addLayout(pagesLayout);
	setLayout(mainLayout);

	this->tempPage = new editPage;

	QObject::connect(this->pageComboBox, SIGNAL(activated(int)),
             this->pagesWidget, SLOT(setCurrentIndex(int)));
};

void editWindow::paintEvent(QPaintEvent *)
{
	QRectF target(0.0, 0.0, this->width(), this->height());
	QRectF source(0.0, 0.0, this->width(), this->height());

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
	this->setWindowTitle(title);
	this->title->setText(title);
	this->pagesWidget->setCurrentIndex(0);
};

QString editWindow::getTitle()
{
	return this->title->text();
};

void editWindow::addPage()
{
	this->pagesWidget->addWidget(this->tempPage);
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
};

editPage* editWindow::page()
{
	return this->tempPage;
};

void editWindow::closeEvent(QCloseEvent* ce)
{
	ce->accept();
};
