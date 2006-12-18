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

#include <QVector>
#include <QComboBox>
#include "customDial.h"
#include "customButton.h"
#include "customLed.h"
#include "dragBar.h"
#include "bankTreeList.h"
#include "floorBoardDisplay.h"
#include "floorPanelBar.h"
#include "floorBoard.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "sysxWriter.h"
#include "SysxIO.h"

floorBoard::floorBoard(QWidget *parent, 
						QString imagePathFloor, 
						QString imagePathStompBG, 
						QString imagePathInfoBar,
						unsigned int marginStompBoxesTop, 
						unsigned int marginStompBoxesBottom, 
						unsigned int marginStompBoxesWidth,
						unsigned int panelBarOffset,
						unsigned int borderWidth,
						QPoint pos)
    : QWidget(parent)
{

	this->imagePathFloor = imagePathFloor;
	this->imagePathStompBG = imagePathStompBG;
	this->imagePathInfoBar = imagePathInfoBar;

	this->marginStompBoxesTop = marginStompBoxesTop;
	this->marginStompBoxesBottom = marginStompBoxesBottom;
	this->marginStompBoxesWidth = marginStompBoxesWidth;
	this->panelBarOffset = panelBarOffset;
	this->borderWidth = borderWidth;
	this->pos = pos;
	
	QVector<signed int> fx(14);
	fx[0]  = 0; //FX1
	fx[1]  = 1; //CS
	fx[2]  = 2; //WAH
	fx[3]  = 3; //LP
	fx[4]  = 4; //OD
	fx[5]  = 5; //PRE
	fx[6]  = 6; //EQ
	fx[7]  = 7; //FX2
	fx[8]  = 8; //DD
	fx[9]  = 9; //CE
	fx[10] = 10; //RV
	fx[11] = 11; //FV
	fx[12] = 12; //NS
	fx[13] = 13; //DGT
	this->fx = fx;

	bankTreeList *bankList = new bankTreeList(this);
	
	QObject::connect(this, SIGNAL( resizeSignal(QRect) ),
                bankList, SLOT( updateSize(QRect) ) );

	setFloorBoard();
	initStomps();

	floorBoardDisplay *display = new floorBoardDisplay(this);
	display->setPos(displayPos);

	floorPanelBar *panelBar = new floorPanelBar(this);
	panelBar->setPos(panelBarPos);	

	dragBar *bar = new dragBar(this);
	bar->setDragBarSize(QSize::QSize(4, panelBar->height() ));
	bar->setDragBarMinOffset(2, 8);
	bar->setDragBarMaxOffset(offset - panelBarOffset + 5);

	QObject::connect(this, SIGNAL(knobValue(int)), display, SLOT(setValueDisplay(int)));
	QObject::connect(panelBar, SIGNAL(resizeSignal(int)), this, SLOT(setWidth(int)));
	QObject::connect(panelBar, SIGNAL(collapseSignal()), this, SLOT(setCollapse()));
	QObject::connect(this, SIGNAL(setCollapseState(bool)), panelBar, SIGNAL(collapseState(bool)));
	QObject::connect(this, SIGNAL(setDisplayPos(QPoint)), display, SLOT(setPos(QPoint)));
	QObject::connect(this, SIGNAL(setFloorPanelBarPos(QPoint)), panelBar, SLOT(setPos(QPoint)));
	QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SIGNAL(updateSignal()));
	QObject::connect(this, SIGNAL(updateSignal()), this, SLOT(updateStompBoxes()));

	QObject::connect(this, SIGNAL(setDragBarOffset(QVector<int>)), panelBar, SIGNAL(setDragBarOffset(QVector<int>)));
	QObject::connect(panelBar, SIGNAL(showDragBar(QPoint)), this, SIGNAL(showDragBar(QPoint)));
	QObject::connect(panelBar, SIGNAL(hideDragBar()), this, SIGNAL(hideDragBar()));

	bool ok;
	Preferences *preferences = Preferences::Instance();
	QString collapseState = preferences->getPreferences("Window", "Collapsed", "bool");
	QString width = preferences->getPreferences("Window", "Size", "width");
	QString default_width = preferences->getPreferences("Window", "Size", "default_width");
	
	if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true")
	{
		this->l_floorSize = QSize::QSize(width.toInt(&ok, 10), floorSize.height());
		
		if(collapseState=="true")
		{ 
			this->setSize(l_floorSize);
			this->colapseState = true;
			emit setCollapseState(true);
		}
		else
		{ 
			this->setSize(minSize);
			this->colapseState = false;
			emit setCollapseState(false);
		};
	}
	else
	{
		this->l_floorSize = QSize::QSize(default_width.toInt(&ok, 10), floorSize.height());
		this->setSize(minSize);
		this->colapseState = false;
		emit setCollapseState(false);
	};
};

floorBoard::~floorBoard()
{
	Preferences *preferences = Preferences::Instance();
	preferences->setPreferences("Window", "Size", "width", QString::number(this->l_floorSize.width(), 10));
	preferences->setPreferences("Window", "Collapsed", "bool", QString(this->colapseState?"true":"false"));
};					

void floorBoard::paintEvent(QPaintEvent *)
{
	QRectF target(pos.x(), pos.y(), floorSize.width(), floorSize.height());
	QRectF source(0.0, 0.0, floorSize.width(), floorSize.height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
	this->setAcceptDrops(true);
};

void floorBoard::setFloorBoard() {
	QPixmap imageFloor(imagePathFloor);
	QPixmap imagestompBG(imagePathStompBG);
	QPixmap imageInfoBar(imagePathInfoBar);
	QPixmap buffer = imageFloor;
	QPainter painter(&buffer);

	this->offset = imageFloor.width() - imageInfoBar.width();
	this->infoBarWidth = imageInfoBar.width();
	this->stompSize = imagestompBG.size();

	initSize(imageFloor.size());
	this->maxSize = floorSize;
	this->minSize = QSize::QSize(imageInfoBar.width() + borderWidth + panelBarOffset, imageFloor.height());

	// Draw InfoBar
	QRectF sourceInfoBar(0.0, 0.0, imageInfoBar.width(), imageInfoBar.height());
	QRectF targetInfoBar(offset, 0.0, imageInfoBar.width(), imageInfoBar.height());
	painter.drawPixmap(targetInfoBar, imageInfoBar, sourceInfoBar);

	// Draw stomp boxes background
	QRectF source(0.0, 0.0, imagestompBG.width(), imagestompBG.height());
	for(int i=0;i<fxPos.size();i++)
	{
		QRectF target(fxPos.at(i).x(), fxPos.at(i).y(), imagestompBG.width(), imagestompBG.height());
		painter.drawPixmap(target, imagestompBG, source);
	};
	painter.end();

	this->baseImage = buffer;
	this->image = buffer;
	this->floorHeight = imageFloor.height();

	QPoint newPanelBarPos = QPoint::QPoint(offset - panelBarOffset, borderWidth);
	this->panelBarPos = newPanelBarPos;
	
	QPoint newDisplayPos = QPoint::QPoint(offset, 0);
	this->displayPos = newDisplayPos;

	QRect newBankListRect = QRect::QRect(borderWidth, borderWidth, offset - panelBarOffset - (borderWidth*2), floorHeight - (borderWidth*2));
	emit resizeSignal(newBankListRect);
};

void floorBoard::dragEnterEvent(QDragEnterEvent *event)
{
	QByteArray data = event->mimeData()->data("text/uri-list");
	QString uri(data);
	
	if (event->mimeData()->hasFormat("application/x-stompbox") ||
		uri.contains(".syx", Qt::CaseInsensitive)) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        };
    } else {
        event->ignore();
    };
};

void floorBoard::dragMoveEvent(QDragMoveEvent *event)
{
	QByteArray data = event->mimeData()->data("text/uri-list");
	QString uri(data);
	
	if (event->mimeData()->hasFormat("application/x-stompbox") ||
		uri.contains(".syx", Qt::CaseInsensitive)) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        };
    } else {
        event->ignore();
    };
};

void floorBoard::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-stompbox")) 
	{
        QByteArray itemData = event->mimeData()->data("application/x-stompbox");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        int stompId;
        QPoint stompPos;
		QSize stompSize;
		QPoint topLeftOffset;
        dataStream >> stompId >> stompPos >> stompSize >> topLeftOffset;
		QPoint dragPoint = (event->pos() - topLeftOffset) + QPoint::QPoint(stompSize.width()/2, stompSize.height()/2);
		int stompSpacing = fxPos.at(1).x() - (fxPos.at(0).x() + stompSize.width());
		
		int destIndex;
		int orgIndex = fx.indexOf(stompId);
		for(int x=0;x<fx.size();x++)
		{
			QRect dropRect = QRect::QRect(fxPos.at(x).x() - stompSpacing - (stompSize.width()/2), fxPos.at(x).y(), stompSize.width() + stompSpacing, stompSize.height());
			QRect lastDropRect = QRect::QRect(fxPos.at(x).x() + (stompSize.width()/2), fxPos.at(x).y(), stompSize.width() + stompSpacing, stompSize.height());
			if( dropRect.contains(dragPoint) )
			{
				destIndex = fx.indexOf(fx.at(x));
			}
			else if( (x == (int)fx.size()-1 || 
			( x == (int)(fx.size()/2)-1 && fx.at(stompId) != fx.size()/2 ) ) 
						&& lastDropRect.contains(dragPoint))
			{
				destIndex = fx.indexOf(fx.at(x)) + 1;
			};
		};

		if( orgIndex < destIndex )
		{
			destIndex = destIndex - 1;
			for(int i=orgIndex;i<destIndex ;i++)
			{
				setStompPos(fx.at(i + 1), i);
			};
			setStompPos(stompId, destIndex );
		}
		else if( orgIndex > destIndex )
		{
			for(int i=orgIndex;i>destIndex;i--)
			{
				setStompPos(fx.at(i - 1), i);
			};
			setStompPos(stompId, destIndex);
		};
	} 
	else 
	{
		if(event->mimeData()->hasFormat("text/uri-list"))
		{
			QByteArray data = event->mimeData()->data("text/uri-list");
			QString uri(data);
			if(uri.contains(".syx", Qt::CaseInsensitive))
			{
				QString removeFromStart = "file:///";
				QString removeFromEnd = ".syx";
				QString filePath = uri;
				filePath.replace(0, filePath.indexOf(removeFromStart) + removeFromStart.length(), "");
				filePath.truncate(filePath.indexOf(removeFromEnd) + removeFromEnd.length());
				filePath.replace("%20", " ");

				sysxWriter file;
				file.setFile(filePath);
				if(file.readFile())
				{	
					emit updateSignal();
				}
				else
				{
					event->ignore();
				};
			}
			else
			{
				event->ignore();
			};
		}
		else
		{
			event->ignore();
		};
    };
};

void floorBoard::initSize(QSize floorSize)
{
	this->floorSize = floorSize;
	this->l_floorSize = floorSize;
	QVector<QPoint> fxPos(14); 
	
	unsigned int spacingV = (floorSize.height() - (marginStompBoxesTop + marginStompBoxesBottom)) - (stompSize.height() * 2);
	unsigned int spacingH = ( (floorSize.width() - offset - (marginStompBoxesWidth * 2)) - (stompSize.width() * 7) ) / 6;
	for(unsigned int i=0;i<14;i++)
	{
		unsigned int y = marginStompBoxesTop;
		unsigned int x = marginStompBoxesWidth + (( stompSize.width() + spacingH ) * i);
		if(i>6)
		{
			y = y + stompSize.height() + spacingV;
			x = x - (( stompSize.width() + spacingH ) * 7);
		};
		fxPos[i] = QPoint::QPoint(offset + x, y);
	};

	this->fxPos = fxPos;
	this->setFixedSize(floorSize);
};

QPoint floorBoard::getStompPos(int id) 
{
	return fxPos.at(id);
};

QSize floorBoard::getSize()
{
	return this->floorSize;
};

void floorBoard::setCollapse()
{
	if(floorSize.width() > minSize.width())
	{ 
		this->l_floorSize = floorSize;
		this->setSize(minSize);
		this->colapseState = false;
		emit setCollapseState(false);
	}
	else
	{ 
		this->setSize(l_floorSize);
		emit setCollapseState(true);
		this->colapseState = true;
	};
};

void floorBoard::setSize(QSize newSize)
{
	unsigned int oldOffset = offset;
	this->offset = newSize.width() - infoBarWidth;
	QSize oldFloorSize = this->floorSize;
	this->floorSize = newSize;

	for(int i=0;i<fxPos.size();i++)
	{
		fxPos[i] = fxPos[i] + QPoint::QPoint(offset - oldOffset, 0);
	};
	emit updateStompOffset(offset - oldOffset);
	
	QPoint newPanelBarPos = QPoint::QPoint(offset - panelBarOffset, panelBarPos.y());
	emit setFloorPanelBarPos(newPanelBarPos);
	this->panelBarPos = newPanelBarPos;
	
	QPoint newDisplayPos = QPoint::QPoint(offset, displayPos.y());
	emit setDisplayPos(newDisplayPos);
	this->displayPos = newDisplayPos;

	QPixmap buffer = QPixmap::QPixmap(floorSize);
	QPainter painter(&buffer);

	// Redraw new resized floor
	QRectF source(maxSize.width() - floorSize.width(), 0.0, floorSize.width(), floorSize.height());
	QRectF target(0.0, 0.0, floorSize.width(), floorSize.height());
	painter.drawPixmap(target, baseImage, source);

	// Redraw clipped border
	QRectF sourceBorder(0.0, 0.0, borderWidth, floorSize.height());
	QRectF targetBorder(0.0, 0.0, borderWidth, floorSize.height());
	painter.drawPixmap(sourceBorder, baseImage, targetBorder);

	painter.end();

	this->image = buffer;	
	this->setFixedSize(floorSize);
	
	QRect newBankListRect = QRect::QRect(borderWidth, borderWidth, offset - panelBarOffset - (borderWidth*2), floorHeight - (borderWidth*2));
	emit resizeSignal(newBankListRect);
	
	emit sizeChanged(floorSize, oldFloorSize);
};

void floorBoard::setWidth(int dist)
{
	QSize newSize;
	if(floorSize.width() + dist < minSize.width())
	{
		newSize = minSize;
		this->colapseState = false;
		emit setCollapseState(false);
	}
	else if(floorSize.width() + dist > maxSize.width())
	{
		newSize = maxSize;
		this->l_floorSize = newSize;
		this->colapseState = true;
		emit setCollapseState(true);
	}
	else
	{
		newSize = QSize::QSize(floorSize.width() + dist, floorSize.height());
		this->l_floorSize = newSize;
		this->colapseState = true;
		emit setCollapseState(true);
	};
	setSize(newSize);
};

void floorBoard::initStomps()
{
	QPalette pal;
    pal.setColor(QPalette::Base,QColor(0,1,62));
    pal.setColor(QPalette::Text,QColor(0,255,204));
	pal.setColor(QPalette::Highlight,QColor(0,1,62));
	pal.setColor(QPalette::HighlightedText,QColor(0,255,204));

	pal.setColor(QPalette::Window,QColor(0,1,62));
	pal.setColor(QPalette::WindowText,QColor(0,255,204));		//List Border
	pal.setColor(QPalette::Button,QColor(0,1,62));
	pal.setColor(QPalette::ButtonText,QColor(0,255,204));

	pal.setColor(QPalette::Light,QColor(0,1,62));				//Lighter than Button color.
	pal.setColor(QPalette::Midlight,QColor(0,1,62));			//Between Button and Light.
	pal.setColor(QPalette::Dark,QColor(0,1,62));				//Darker than Button.
	pal.setColor(QPalette::Mid,QColor(0,1,62));					//Between Button and Dark.
	pal.setColor(QPalette::Shadow,QColor(0,1,62));

	   QFont font;
	font.setFamily("Arial");
	font.setBold(true);
	font.setPixelSize(10);
	font.setStretch(120);

	/* FX1 */
	stompBox *fx1 = new stompBox(this);
	this->stompBoxes.append(fx1);
	this->stompNames.append("fx1");
	fx1->setId(0);
	fx1->setImage(":/images/fx1.png");
	fx1->setPos(this->getStompPos(0)); 

	QComboBox *fx1_comboBox = new QComboBox(fx1); 
	fx1_comboBox->addItem( "Test 1" ); 
	fx1_comboBox->addItem( "Test 2" ); 
	fx1_comboBox->addItem( "Test 3" );

	fx1_comboBox->setGeometry(8, 31, 79, 13);
	fx1_comboBox->setEditable(false);
	fx1_comboBox->setFont(font);
	fx1_comboBox->setPalette(pal);
	fx1_comboBox->setFrame(false);

	customButton *fx1_button = new customButton(false, QPoint::QPoint(4, 110), fx1);
	customLed *fx1_led = new customLed(false, QPoint::QPoint(41, 4), fx1);
	QObject::connect(fx1_button, SIGNAL(valueChanged(bool)),
                         fx1_led, SLOT(setValue(bool)));

	/* COMP */
	stompBox *cs = new stompBox(this);
	this->stompBoxes.append(cs);
	this->stompNames.append("cs");

	cs->setId(1);
	cs->setImage(":/images/cs.png");
	cs->setPos(this->getStompPos(1));

	QComboBox *comp_comboBox = new QComboBox(cs); 
	comp_comboBox->addItem( "Test 1" ); 
	comp_comboBox->addItem( "Test 2" ); 
	comp_comboBox->addItem( "Test 3" );

	comp_comboBox->setGeometry(7, 79, 80, 13);
	comp_comboBox->setEditable(false);
	comp_comboBox->setFont(font);
	comp_comboBox->setPalette(pal);
	comp_comboBox->setFrame(false);

	customDial *comp_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), cs);
	customDial *comp_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), cs);
	customButton *comp_button = new customButton(false, QPoint::QPoint(4, 110), cs);
	customLed *comp_led = new customLed(false, QPoint::QPoint(41, 4), cs);
	QObject::connect(comp_button, SIGNAL(valueChanged(bool)),
                         comp_led, SLOT(setValue(bool)));	
	/* WAH */
	stompBox *wah = new stompBox(this);
	this->stompBoxes.append(wah);
	this->stompNames.append("wah");
	wah->setId(2);
	wah->setImage(":/images/wah.png");
	wah->setPos(this->getStompPos(2)); 

	QComboBox *wah_comboBox = new QComboBox(wah); 
	wah_comboBox->addItem( "Test 1" ); 
	wah_comboBox->addItem( "Test 2" ); 
	wah_comboBox->addItem( "Test 3" );

	wah_comboBox->setGeometry(7, 79, 80, 13);
	wah_comboBox->setEditable(false);
	wah_comboBox->setFont(font);
	wah_comboBox->setPalette(pal);
	wah_comboBox->setFrame(false);

	customButton *wah_button = new customButton(false, QPoint::QPoint(0, 109), wah, ":/images/pedal.png", QSize::QSize(85, 59));
	customLed *wah_led = new customLed(false, QPoint::QPoint(41, 4), wah);
	QObject::connect(wah_button, SIGNAL(valueChanged(bool)),
                         wah_led, SLOT(setValue(bool)));
	/* LOOP */
	stompBox *lp = new stompBox(this);
	this->stompBoxes.append(lp);
	this->stompNames.append("lp");
	lp->setId(3);
	lp->setImage(":/images/lp.png");
	lp->setPos(this->getStompPos(3)); 

	QComboBox *lp_comboBox = new QComboBox(lp); 
	lp_comboBox->addItem( "Test 1" ); 
	lp_comboBox->addItem( "Test 2" ); 
	lp_comboBox->addItem( "Test 3" );

	lp_comboBox->setGeometry(7, 79, 80, 13);
	lp_comboBox->setEditable(false);
	lp_comboBox->setFont(font);
	lp_comboBox->setPalette(pal);
	lp_comboBox->setFrame(false);

	customDial *lp_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), lp);
	customDial *lp_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), lp);
	customButton *lp_button = new customButton(false, QPoint::QPoint(4, 110), lp);
	customLed *lp_led = new customLed(false, QPoint::QPoint(41, 4), lp);
	QObject::connect(lp_button, SIGNAL(valueChanged(bool)),
                         lp_led, SLOT(setValue(bool)));	
	/* OD/DS */
	stompBox *od = new stompBox(this);
	this->stompBoxes.append(od);
	this->stompNames.append("od");
	od->setId(4);
	od->setImage(":/images/od.png");
	od->setPos(this->getStompPos(4)); 

	QComboBox *od_comboBox = new QComboBox(od); 
	od_comboBox->addItem( "Test 1" ); 
	od_comboBox->addItem( "Test 2" ); 
	od_comboBox->addItem( "Test 3" );

	od_comboBox->setGeometry(7, 79, 80, 13);
	od_comboBox->setEditable(false);
	od_comboBox->setFont(font);
	od_comboBox->setPalette(pal);
	od_comboBox->setFrame(false);

	customDial *od_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), od);
	customDial *od_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), od);
	customButton *od_button = new customButton(false, QPoint::QPoint(4, 110), od);
	customLed *od_led = new customLed(false, QPoint::QPoint(41, 4), od);
	QObject::connect(od_button, SIGNAL(valueChanged(bool)),
                         od_led, SLOT(setValue(bool)));
	/* AMP */
	stompBox *pre = new stompBox(this);
	this->stompBoxes.append(pre);
	this->stompNames.append("pre");
	pre->setId(5);
	pre->setImage(":/images/pre.png");
	pre->setPos(this->getStompPos(5)); 

	/* EQ */
	stompBox *eq = new stompBox(this);
	this->stompBoxes.append(eq);
	this->stompNames.append("eq");
	eq->setId(6);
	eq->setImage(":/images/eq.png");
	eq->setPos(this->getStompPos(6)); 

	customButton *eq_button = new customButton(false, QPoint::QPoint(4, 110), eq);
	customLed *eq_led = new customLed(false, QPoint::QPoint(41, 4), eq);
	QObject::connect(eq_button, SIGNAL(valueChanged(bool)),
                         eq_led, SLOT(setValue(bool)));	
	/* FX 2 */
	stompBox *fx2 = new stompBox(this);
	this->stompBoxes.append(fx2);
	this->stompNames.append("fx2");
	fx2->setId(7);
	fx2->setImage(":/images/fx2.png");
	fx2->setPos(this->getStompPos(7)); 

	QComboBox *fx2_comboBox = new QComboBox(fx2); 
	fx2_comboBox->addItem( "Test 1" ); 
	fx2_comboBox->addItem( "Test 2" ); 
	fx2_comboBox->addItem( "Test 3" );

	fx2_comboBox->setGeometry(8, 31, 79, 13);
	fx2_comboBox->setEditable(false);
	fx2_comboBox->setFont(font);
	fx2_comboBox->setPalette(pal);
	fx2_comboBox->setFrame(false);

	customButton *fx2_button = new customButton(false, QPoint::QPoint(4, 110), fx2);
	customLed *fx2_led = new customLed(false, QPoint::QPoint(41, 4), fx2);
	QObject::connect(fx2_button, SIGNAL(valueChanged(bool)),
                         fx2_led, SLOT(setValue(bool)));	
	/* Delay */
	stompBox *dd = new stompBox(this);
	this->stompBoxes.append(dd);
	this->stompNames.append("dd");
	dd->setId(8);
	dd->setImage(":/images/dd.png");
	dd->setPos(this->getStompPos(8));

	QComboBox *dd_comboBox = new QComboBox(dd); 
	dd_comboBox->addItem( "Test 1" ); 
	dd_comboBox->addItem( "Test 2" ); 
	dd_comboBox->addItem( "Test 3" );

	dd_comboBox->setGeometry(7, 79, 80, 13);
	dd_comboBox->setEditable(false);
	dd_comboBox->setFont(font);
	dd_comboBox->setPalette(pal);
	dd_comboBox->setFrame(false);

	customDial *dd_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), dd);
	customDial *dd_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), dd);
	customButton *dd_button = new customButton(false, QPoint::QPoint(4, 110), dd);
	customLed *dd_led = new customLed(false, QPoint::QPoint(41, 4), dd);
	QObject::connect(dd_button, SIGNAL(valueChanged(bool)),
                         dd_led, SLOT(setValue(bool)));	
	/* Chorus */
	stompBox *ce = new stompBox(this);
	this->stompBoxes.append(ce);
	this->stompNames.append("ce");
	ce->setId(9);
	ce->setImage(":/images/ce.png");
	ce->setPos(this->getStompPos(9));

	QComboBox *cc_comboBox = new QComboBox(ce); 
	cc_comboBox->addItem( "Test 1" ); 
	cc_comboBox->addItem( "Test 2" ); 
	cc_comboBox->addItem( "Test 3" );

	cc_comboBox->setGeometry(7, 79, 80, 13);
	cc_comboBox->setEditable(false);
	cc_comboBox->setFont(font);
	cc_comboBox->setPalette(pal);
	cc_comboBox->setFrame(false);

	customDial *cc_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), ce);
	customDial *cc_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), ce);
	customButton *cc_button = new customButton(false, QPoint::QPoint(4, 110), ce);
	customLed *cc_led = new customLed(false, QPoint::QPoint(41, 4), ce);
	QObject::connect(cc_button, SIGNAL(valueChanged(bool)),
                         cc_led, SLOT(setValue(bool)));	
	/* REVERB */
	stompBox *rv = new stompBox(this);
	this->stompBoxes.append(rv);
	this->stompNames.append("rv");
	rv->setId(10);
	rv->setImage(":/images/rv.png");
	rv->setPos(this->getStompPos(10)); 

	QComboBox *rev_comboBox = new QComboBox(rv); 
	rev_comboBox->addItem( "Test 1" ); 
	rev_comboBox->addItem( "Test 2" ); 
	rev_comboBox->addItem( "Test 3" );

	rev_comboBox->setGeometry(7, 79, 80, 13);
	rev_comboBox->setEditable(false);
	rev_comboBox->setFont(font);
	rev_comboBox->setPalette(pal);
	rev_comboBox->setFrame(false);

	customDial *rev_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), rv);
	customDial *rev_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), rv);
	customButton *rev_button = new customButton(false, QPoint::QPoint(4, 110), rv);
	customLed *rev_led = new customLed(false, QPoint::QPoint(41, 4), rv);
	QObject::connect(rev_button, SIGNAL(valueChanged(bool)),
                         rev_led, SLOT(setValue(bool)));	
	/* VOLUME */
	stompBox *fv = new stompBox(this);
	this->stompBoxes.append(fv);
	this->stompNames.append("fv");
	fv->setId(11);
	fv->setImage(":/images/fv.png");
	fv->setPos(this->getStompPos(11)); 

	QComboBox *vol_comboBox = new QComboBox(fv); 
	vol_comboBox->addItem( "Test 1" ); 
	vol_comboBox->addItem( "Test 2" ); 
	vol_comboBox->addItem( "Test 3" );

	vol_comboBox->setGeometry(7, 79, 80, 13);
	vol_comboBox->setEditable(false);
	vol_comboBox->setFont(font);
	vol_comboBox->setPalette(pal);
	vol_comboBox->setFrame(false);

	customButton *vol_button = new customButton(false, QPoint::QPoint(0, 109), fv, ":/images/pedal.png", QSize::QSize(85, 59));
	customLed *vol_led = new customLed(false, QPoint::QPoint(41, 4), fv);
	QObject::connect(vol_button, SIGNAL(valueChanged(bool)),
                         vol_led, SLOT(setValue(bool)));
	/* NS */
	stompBox *ns = new stompBox(this);
	this->stompBoxes.append(ns);
	this->stompNames.append("ns");
	ns->setId(12);
	ns->setImage(":/images/ns.png");
	ns->setPos(this->getStompPos(12));

	customDial *ns_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), ns);
	customDial *ns_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), ns);
	customButton *ns_button = new customButton(false, QPoint::QPoint(4, 110), ns);
	customLed *ns_led = new customLed(false, QPoint::QPoint(41, 4), ns);
	QObject::connect(ns_button, SIGNAL(valueChanged(bool)),
                         ns_led, SLOT(setValue(bool)));	
	/* D-OUT */
	stompBox *dgt = new stompBox(this);
	this->stompBoxes.append(dgt);
	this->stompNames.append("dgt");
	dgt->setId(13);
	dgt->setImage(":/images/dgt.png");
	dgt->setPos(this->getStompPos(13));
};

void floorBoard::setStomps(QVector<QString> stompOrder)
{
	for(int i=0;i<stompOrder.size();i++)
	{
		QString name = stompOrder.at(i);
		setStompPos(name, i);
		this->fx.replace(i, stompNames.indexOf(name));
	};
};

void floorBoard::setStompPos(QString name, int order)
{
	this->stompBoxes.at(stompNames.indexOf(name))->setPos(this->getStompPos(order));
	this->fx.replace(order, stompNames.indexOf(name));
};

void floorBoard::setStompPos(int index, int order)
{
	this->stompBoxes.at(index)->setPos(this->getStompPos(order));
	this->fx.replace(order, index);
};

void floorBoard::updateStompBoxes()
{
	SysxIO *sysxIO = SysxIO::Instance();
	SysxData fileSource = sysxIO->getFileSource();
	QVector<QString> fxChain = fileSource.hex.at( fileSource.adress.indexOf("1100") );

	MidiTable *midiTable = MidiTable::Instance();
	QVector<QString> stompOrder;
	for(int i=11;i<fxChain.size() - 2;i++ )
	{
		stompOrder.append( midiTable->getMidiMap("Stucture", "11", "00", "00", fxChain.at(i)).name.toLower() );
	};
	setStomps(stompOrder);
};