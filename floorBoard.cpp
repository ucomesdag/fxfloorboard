/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "FX Floor Board".
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
#include "stompBox.h"
#include "dragBar.h"
#include "bankTreeList.h"
#include "floorBoardDisplay.h"
#include "floorPanelBar.h"
#include "floorBoard.h"

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

	QObject::connect(this, SIGNAL( knobValue(int) ),
                display, SLOT( setValueDisplay(int) ) );

	QObject::connect(panelBar, SIGNAL( resizeSignal(int) ),
                this, SLOT( setWidth(int) ) );

	QObject::connect(panelBar, SIGNAL( collapseSignal() ),
                this, SLOT( setCollapse() ) );

	QObject::connect(this, SIGNAL( setCollapseState(bool) ),
                panelBar, SIGNAL( collapseState(bool) ) );

	QObject::connect(this, SIGNAL( setDisplayPos(QPoint) ),
                display, SLOT( setPos(QPoint) ) );

	QObject::connect(this, SIGNAL( setFloorPanelBarPos(QPoint) ),
                panelBar, SLOT( setPos(QPoint) ) );

	QObject::connect(this, SIGNAL( setDragBarOffset(QVector<int>) ),
                panelBar, SIGNAL( setDragBarOffset(QVector<int>) ) );

	QObject::connect(panelBar, SIGNAL( showDragBar(QPoint) ),
                this, SIGNAL( showDragBar(QPoint) ) );

	QObject::connect(panelBar, SIGNAL( hideDragBar() ),
                this, SIGNAL( hideDragBar() ) );
};

void floorBoard::paintEvent(QPaintEvent *)
{
	QRectF target(pos.x(), pos.y(), floorSize.width(), floorSize.height());
	QRectF source(0.0, 0.0, floorSize.width(), floorSize.height());

	QPainter painter(this);
	painter.drawPixmap(target, image, source);
	setAcceptDrops(true);
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
    if (event->mimeData()->hasFormat("application/x-stompbox")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
};

void floorBoard::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-stompbox")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
};

void floorBoard::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-stompbox")) 
	{
        QByteArray itemData = event->mimeData()->data("application/x-stompbox");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        unsigned int stompId;
        QPoint stompPos;
		QSize stompSize;
		QPoint topLeftOffset;
        dataStream >> stompId >> stompPos >> stompSize >> topLeftOffset;

		QPoint dragPoint = (event->pos() - topLeftOffset) + QPoint::QPoint(stompSize.width()/2, stompSize.height()/2);
		
		signed int stompSpacing = fxPos.at(1).x() - (fxPos.at(0).x() + stompSize.width());                                                                                             

		unsigned int fxOrgPos = fx.at(stompId);;
		unsigned int fxDestPos;
		for(int x=0;x<fxPos.size();x++)
		{
			QRect dropRect = QRect::QRect(fxPos.at(x).x() - stompSpacing - (stompSize.width()/2), fxPos.at(x).y(), stompSize.width() + stompSpacing, stompSize.height());
			QRect lastDropRect = QRect::QRect(fxPos.at(x).x() + (stompSize.width()/2), fxPos.at(x).y(), stompSize.width() + stompSpacing, stompSize.height());
			
			fxDestPos = x;
			if( dropRect.contains(dragPoint) )
			{
				if(fxDestPos < fxOrgPos)
				{
					for(unsigned int i=fxOrgPos;i>fxDestPos;i--)
					{
						this->childAt(fxPos.at(i-1))->move(fxPos.at(i));
						this->fx.replace(fx.indexOf(i-1), i);
					};
				}
				else if(fxDestPos > fxOrgPos)
				{
					fxDestPos = fxDestPos - 1;
					for(unsigned int i=fxOrgPos;i<fxDestPos;i++)
					{
						this->childAt(fxPos.at(i+1))->move(fxPos.at(i));
						this->fx.replace(fx.indexOf(i+1), i);
					};
				};
				this->fx.replace(stompId, fxDestPos);
				event->source()->move(fxPos.at(fx.at(stompId)));

				break;
			} 
			else if( (fxDestPos == (unsigned int)fxPos.size()-1 || ( fxDestPos == (unsigned int)(fxPos.size()/2)-1 && fx.at(stompId) != fxPos.size()/2 ) ) 
						&& lastDropRect.contains(dragPoint))
			{
				if(fxDestPos < fxOrgPos)
				{
					fxDestPos = fxDestPos + 1;
					for(unsigned int i=fxOrgPos;i>fxDestPos;i--)
					{
						this->childAt(fxPos.at(i-1))->move(fxPos.at(i));
						this->fx.replace(fx.indexOf(i-1), i);
					};
				}
				else if(fxDestPos > fxOrgPos)
				{
					for(unsigned int i=fxOrgPos;i<fxDestPos;i++)
					{
						this->childAt(fxPos.at(i+1))->move(fxPos.at(i));
						this->fx.replace(fx.indexOf(i+1), i);
					}
				};
				this->fx.replace(stompId, fxDestPos);
				event->source()->move(fxPos.at(fx.at(stompId)));

				break;
			};
		};
	} 
	else 
	{
        event->ignore();
    }
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

QPoint floorBoard::getStompPos(unsigned int id) 
{
	return fxPos.at(fx.at(id));
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
		emit setCollapseState(true);
	}
	else
	{ 
		this->setSize(l_floorSize);
		emit setCollapseState(false);
	};

};

void floorBoard::setSize(QSize newSize)
{
	unsigned int oldOffset = offset;
	this->offset = newSize.width() - infoBarWidth;
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
	
	emit sizeChanged(floorSize);
};

void floorBoard::setWidth(int dist)
{
	QSize newSize;
	if(floorSize.width() + dist < minSize.width())
	{
		newSize = minSize;
		emit setCollapseState(true);
	}
	else if(floorSize.width() + dist > maxSize.width())
	{
		newSize = maxSize;
		this->l_floorSize = newSize;
		emit setCollapseState(false);
	}
	else
	{
		newSize = QSize::QSize(floorSize.width() + dist, floorSize.height());
		this->l_floorSize = newSize;
		emit setCollapseState(false);
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

	stompBox *fx1 = new stompBox(this);
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
	stompBox *comp = new stompBox(this);
	comp->setId(1);
	comp->setImage(":/images/comp.png");
	comp->setPos(this->getStompPos(1));

	QComboBox *comp_comboBox = new QComboBox(comp); 
	comp_comboBox->addItem( "Test 1" ); 
	comp_comboBox->addItem( "Test 2" ); 
	comp_comboBox->addItem( "Test 3" );

	comp_comboBox->setGeometry(7, 79, 80, 13);
	comp_comboBox->setEditable(false);
	comp_comboBox->setFont(font);
	comp_comboBox->setPalette(pal);
	comp_comboBox->setFrame(false);

	customDial *comp_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), comp);
	customDial *comp_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), comp);
	customButton *comp_button = new customButton(false, QPoint::QPoint(4, 110), comp);
	customLed *comp_led = new customLed(false, QPoint::QPoint(41, 4), comp);
	QObject::connect(comp_button, SIGNAL(valueChanged(bool)),
                         comp_led, SLOT(setValue(bool)));	
	/* WAH */
	stompBox *wah = new stompBox(this);
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
	stompBox *amp = new stompBox(this);
	amp->setId(5);
	amp->setImage(":/images/amp.png");
	amp->setPos(this->getStompPos(5)); 

	/* EQ */
	stompBox *eq = new stompBox(this);
	eq->setId(6);
	eq->setImage(":/images/eq.png");
	eq->setPos(this->getStompPos(6)); 

	customButton *eq_button = new customButton(false, QPoint::QPoint(4, 110), eq);
	customLed *eq_led = new customLed(false, QPoint::QPoint(41, 4), eq);
	QObject::connect(eq_button, SIGNAL(valueChanged(bool)),
                         eq_led, SLOT(setValue(bool)));	
	/* FX 2 */
	stompBox *fx2 = new stompBox(this);
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
	stompBox *cc = new stompBox(this);
	cc->setId(9);
	cc->setImage(":/images/cc.png");
	cc->setPos(this->getStompPos(9));

	QComboBox *cc_comboBox = new QComboBox(cc); 
	cc_comboBox->addItem( "Test 1" ); 
	cc_comboBox->addItem( "Test 2" ); 
	cc_comboBox->addItem( "Test 3" );

	cc_comboBox->setGeometry(7, 79, 80, 13);
	cc_comboBox->setEditable(false);
	cc_comboBox->setFont(font);
	cc_comboBox->setPalette(pal);
	cc_comboBox->setFrame(false);

	customDial *cc_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), cc);
	customDial *cc_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), cc);
	customButton *cc_button = new customButton(false, QPoint::QPoint(4, 110), cc);
	customLed *cc_led = new customLed(false, QPoint::QPoint(41, 4), cc);
	QObject::connect(cc_button, SIGNAL(valueChanged(bool)),
                         cc_led, SLOT(setValue(bool)));	
	/* REVERB */
	stompBox *rev = new stompBox(this);
	rev->setId(10);
	rev->setImage(":/images/rev.png");
	rev->setPos(this->getStompPos(10)); 

	QComboBox *rev_comboBox = new QComboBox(rev); 
	rev_comboBox->addItem( "Test 1" ); 
	rev_comboBox->addItem( "Test 2" ); 
	rev_comboBox->addItem( "Test 3" );

	rev_comboBox->setGeometry(7, 79, 80, 13);
	rev_comboBox->setEditable(false);
	rev_comboBox->setFont(font);
	rev_comboBox->setPalette(pal);
	rev_comboBox->setFrame(false);

	customDial *rev_knob1 = new customDial(0, -20, 20, 1, 10, QPoint::QPoint(6, 9), rev);
	customDial *rev_knob2 = new customDial(50, 0, 100, 1, 10, QPoint::QPoint(53, 9), rev);
	customButton *rev_button = new customButton(false, QPoint::QPoint(4, 110), rev);
	customLed *rev_led = new customLed(false, QPoint::QPoint(41, 4), rev);
	QObject::connect(rev_button, SIGNAL(valueChanged(bool)),
                         rev_led, SLOT(setValue(bool)));	
	/* VOLUME */
	stompBox *vol = new stompBox(this);
	vol->setId(11);
	vol->setImage(":/images/vol.png");
	vol->setPos(this->getStompPos(11)); 

	QComboBox *vol_comboBox = new QComboBox(vol); 
	vol_comboBox->addItem( "Test 1" ); 
	vol_comboBox->addItem( "Test 2" ); 
	vol_comboBox->addItem( "Test 3" );

	vol_comboBox->setGeometry(7, 79, 80, 13);
	vol_comboBox->setEditable(false);
	vol_comboBox->setFont(font);
	vol_comboBox->setPalette(pal);
	vol_comboBox->setFrame(false);

	customButton *vol_button = new customButton(false, QPoint::QPoint(0, 109), vol, ":/images/pedal.png", QSize::QSize(85, 59));
	customLed *vol_led = new customLed(false, QPoint::QPoint(41, 4), vol);
	QObject::connect(vol_button, SIGNAL(valueChanged(bool)),
                         vol_led, SLOT(setValue(bool)));
	/* NS */
	stompBox *ns = new stompBox(this);
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
	stompBox *dout = new stompBox(this);
	dout->setId(13);
	dout->setImage(":/images/dout.png");
	dout->setPos(this->getStompPos(13));
};