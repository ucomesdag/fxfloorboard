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

#ifndef EDITPAGE_H
#define EDITPAGE_H

#include <QWidget>
#include <QtGui>

class editPage : public QWidget
{
    Q_OBJECT

public:
    editPage(QWidget *parent = 0);
	
	void addKnob(int row, int column, int rowSpan, int columnSpan,
		QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
		QString background = "normal", 
		QString direction = "bottom", 
		int lenght = 45,
		Qt::Alignment alignment = 0);
	void addSwitch(int row, int column, int rowSpan, int columnSpan,
		QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
		QString direction = "bottom",
		Qt::Alignment alignment = 0);
	void addComboBox(int row, int column, int rowSpan, int columnSpan,
		QString hex1 = "void",
		QString hex2 = "void",
		QString hex3 = "void",
		QString direction = "bottom",
		Qt::Alignment alignment = 0);
	void addLabel(int row, int column, int rowSpan, int columnSpan, QString text, Qt::Alignment alignment = 0);
	void newGroupBox(QString title, Qt::Alignment alignment = Qt::AlignCenter);
	void addGroupBox(int row, int column, int rowSpan, int columnSpan);
	void setGridLayout();
	void newStackControl(int id, 
		int row, int column, int rowSpan, int columnSpan,
		Qt::Alignment alignment = 0);
	void addStackControl();
	void newStackField(int id, Qt::Alignment alignment = Qt::AlignTop | Qt::AlignLeft);
	void addStackField();

protected:
	void paintEvent(QPaintEvent *event);

public slots:
	void valueChanged(bool value, QString hex1, QString hex2, QString hex3); // Not used.
	void updateDialog(int index);

signals:
	void dialogUpdateSignal();
	void updateSignal();

private:
	int currentIndex;
	QGridLayout* layout;
	QList<QGridLayout*> groupBoxLayouts;
	QList<QGroupBox*> groupBoxes;
	QGridLayout* groupBoxLayout;
	QGroupBox* groupBox;
	QList<int> groupBoxIndexList;
	bool groupBoxMode;
	int groupBoxLevel;
	int groupBoxIndex;
	int parentBoxDif;	
	QList<QStackedWidget*> stackedFields;
	QGridLayout* stackField;
	bool stackControlMode;
	bool stackFieldMode;
	int stackFieldId;
};

#endif // EDITPAGE_H
