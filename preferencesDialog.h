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

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include "preferencesPages.h"

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class preferencesDialog : public QDialog
{
	Q_OBJECT

public:
	preferencesDialog();

	GeneralPage* generalSettings;
	MidiPage* midiSettings;
	WindowPage* windowSettings;

public slots:
	void changePage(QListWidgetItem *current, QListWidgetItem *previous);

private:
	void createIcons();

	QListWidget *contentsWidget;
	QStackedWidget *pagesWidget;
};

#endif // PREFERENCESDIALOG_H
