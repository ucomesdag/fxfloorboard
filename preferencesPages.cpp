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

#include <QtGui>
#include "midiIO.h"
#include "preferencesPages.h"
#include "Preferences.h"

GeneralPage::GeneralPage(QWidget *parent)
	: QWidget(parent)
{
	Preferences *preferences = Preferences::Instance();
	QString dir = preferences->getPreferences("General", "Files", "dir");

	QGroupBox *patchGroup = new QGroupBox(tr("Patch folder"));

	QLabel *descriptionLabel = new QLabel(tr("Select the default folder for storing patches."));
	QLabel *dirLabel = new QLabel(tr("Default patch folder:"));
	QLineEdit *dirEdit = new QLineEdit(dir);
	QPushButton *browseButton = new QPushButton(tr("Browse"));

	connect(browseButton, SIGNAL(clicked()), this, SLOT(browseDir()));

	this->dirEdit = dirEdit;

	QHBoxLayout *dirEditLayout = new QHBoxLayout;
	dirEditLayout->addWidget(dirEdit);
	dirEditLayout->addWidget(browseButton);

	QVBoxLayout *dirLayout = new QVBoxLayout;
	dirLayout->addWidget(descriptionLabel);
	dirLayout->addSpacing(12);
	dirLayout->addWidget(dirLabel);
	dirLayout->addLayout(dirEditLayout);

	QVBoxLayout *patchLayout = new QVBoxLayout;
	patchLayout->addLayout(dirLayout);
	patchGroup->setLayout(patchLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(patchGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
};

MidiPage::MidiPage(QWidget *parent)
	: QWidget(parent)
{
	bool ok; int id;
	midiIO *midi = new midiIO();
	Preferences *preferences = Preferences::Instance();
	QString midiInDevice = preferences->getPreferences("Midi", "MidiIn", "device");
	QString midiOutDevice = preferences->getPreferences("Midi", "MidiOut", "device");
	int midiInDeviceID = midiInDevice.toInt(&ok, 10);
	int midiOutDeviceID = midiOutDevice.toInt(&ok, 10);
	QList<QString> midiInDevices = midi->getMidiInDevices();
	QList<QString> midiOutDevices = midi->getMidiOutDevices();
	
	QGroupBox *midiGroup = new QGroupBox(tr("Midi settings"));

	QLabel *descriptionLabel = new QLabel(tr("Select your midi in and out device."));
	QLabel *midiInLabel = new QLabel(tr("Midi in:"));
	QLabel *midiOutLabel = new QLabel(tr("Midi out:"));

	QComboBox *midiInCombo = new QComboBox;
	this->midiInCombo = midiInCombo;
	midiInCombo->addItem(tr("Select midi-in device"));
	id = 0;
	for (QList<QString>::iterator dev = midiInDevices.begin(); dev != midiInDevices.end(); ++dev)
    {
		QString str(*dev);
		midiInCombo->addItem(str.toAscii().data());
		id++;
    };
	if(!midiInDevice.isEmpty())
	{
		midiInCombo->setCurrentIndex(midiInDeviceID + 1); // +1 because there is a default entry at 0
	};
	
	QComboBox *midiOutCombo = new QComboBox;
	this->midiOutCombo = midiOutCombo;
	midiOutCombo->addItem(tr("Select midi-out device"));
	id = 0;
	for (QList<QString>::iterator dev = midiOutDevices.begin(); dev != midiOutDevices.end(); ++dev)
    {
		QString str(*dev);
		midiOutCombo->addItem(str.toAscii().data());
		id++;
    };
	if(!midiOutDevice.isEmpty())
	{
		midiOutCombo->setCurrentIndex(midiOutDeviceID + 1); // +1 because there is a default entry at 0
	};

	QVBoxLayout *midiLabelLayout = new QVBoxLayout;
	midiLabelLayout->addWidget(midiInLabel);
	midiLabelLayout->addWidget(midiOutLabel);

	QVBoxLayout *midiComboLayout = new QVBoxLayout;
	midiComboLayout->addWidget(midiInCombo);
	midiComboLayout->addWidget(midiOutCombo);

	QHBoxLayout *midiSelectLayout = new QHBoxLayout;
	midiSelectLayout->addLayout(midiLabelLayout);
	midiSelectLayout->addLayout(midiComboLayout);
	
	QVBoxLayout *midiDevLayout = new QVBoxLayout;
	midiDevLayout->addWidget(descriptionLabel);
	midiDevLayout->addSpacing(12);
	midiDevLayout->addLayout(midiSelectLayout);

	QVBoxLayout *midiLayout = new QVBoxLayout;
	midiLayout->addLayout(midiDevLayout);
	midiGroup->setLayout(midiLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(midiGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
};

WindowPage::WindowPage(QWidget *parent)
	: QWidget(parent)
{
	Preferences *preferences = Preferences::Instance();
	QString windowRestore = preferences->getPreferences("Window", "Restore", "window");
	QString sidepanelRestore = preferences->getPreferences("Window", "Restore", "sidepanel");
	QString splashScreen = preferences->getPreferences("Window", "Splash", "bool");

	QGroupBox *windowGroup = new QGroupBox(tr("Window settings"));

	QLabel *restoreDescriptionLabel = new QLabel(tr("Select if you want the window position to be saved on exit."));
	QCheckBox *windowCheckBox = new QCheckBox(tr("Restore window"));
	QCheckBox *sidepanelCheckBox = new QCheckBox(tr("Restore sidepanel"));
	this->windowCheckBox = windowCheckBox;
	this->sidepanelCheckBox = sidepanelCheckBox;

	if(windowRestore=="true")
	{
		windowCheckBox->setChecked(true);
	};
	if(sidepanelRestore=="true")
	{
		sidepanelCheckBox->setChecked(true);
	};

	QVBoxLayout *restoreLayout = new QVBoxLayout;
	restoreLayout->addWidget(restoreDescriptionLabel);
	restoreLayout->addSpacing(12);
	restoreLayout->addWidget(windowCheckBox);
	restoreLayout->addWidget(sidepanelCheckBox);
	
	QVBoxLayout *windowLayout = new QVBoxLayout;
	windowLayout->addLayout(restoreLayout);
	windowGroup->setLayout(windowLayout);

	QGroupBox *splashScreenGroup = new QGroupBox(tr("Show splash screen"));

	QLabel *splashDescriptionLabel = new QLabel(tr("Disable or enable the splash screen."));
	QCheckBox *splashCheckBox = new QCheckBox(tr("Splash screen"));
	this->splashCheckBox = splashCheckBox;

	if(splashScreen=="true")
	{
		splashCheckBox->setChecked(true);
	};

	QVBoxLayout *splashLayout = new QVBoxLayout;
	splashLayout->addWidget(splashDescriptionLabel);
	splashLayout->addSpacing(12);
	splashLayout->addWidget(splashCheckBox);

	QVBoxLayout *splashScreenLayout = new QVBoxLayout;
	splashScreenLayout->addLayout(splashLayout);
	splashScreenGroup->setLayout(splashScreenLayout);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(windowGroup);
	mainLayout->addWidget(splashScreenGroup);
	mainLayout->addStretch(1);
	setLayout(mainLayout);
};

void GeneralPage::browseDir()
{
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Select the default folder for storing patches."),
		this->dirEdit->text(),
        QFileDialog::ShowDirsOnly);
	if(!dirName.isEmpty())
	{
		this->dirEdit->setText(dirName);
	};
};
