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

#include "floorBoardDisplay.h"
#include "Preferences.h"	

floorBoardDisplay::floorBoardDisplay(QWidget *parent, QPoint pos)
    : QWidget(parent)
{
	Preferences *preferences = Preferences::Instance();
	QString version = preferences->getPreferences("General", "Application", "version");
	
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

void floorBoardDisplay::setValueDisplay(int num)
{
	QString str;
	str.append("<html><body>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td colspan='2' align='left'>");
	str.append("{FX NAME}");
	str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
	str.append("{ValueName}");
	str.append("</font></td><td align='right' valign='top'><font size='-1'>");
	str.append(QString::number(num, 10));
	str.append(tr("dB"));
	str.append("</font></td></tr></table>");
	str.append("</body></html>");
	valueDisplay->setHtml(str);
};

void floorBoardDisplay::setPatchDisplay(QString patchName)
{
	QString str;
	str.append("<html><body>");
	str.append("<table width='140' cellspacing='0' cellpadding='0' border='0'><tr><td align='left'>");
	str.append("{PATCH NAME}");
	str.append("</td></tr><tr><td align='left' valign='top'><font size='-1'>");
	str.append("{patch_file_name_name.syx}");
	str.append("</font></td></tr></table>");
	str.append("</body></html>");
	valueDisplay->setHtml(str);
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
