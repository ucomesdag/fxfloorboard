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

#include <QList>

#include "dragBar.h"
#include "bankTreeList.h"
#include "floorBoardDisplay.h"
#include "floorPanelBar.h"
#include "floorBoard.h"
#include "Preferences.h"
#include "MidiTable.h"
#include "sysxWriter.h"
#include "SysxIO.h"

#include "stompbox_fx1.h"
#include "stompbox_cs.h"
#include "stompbox_wah.h"
#include "stompbox_lp.h"
#include "stompbox_od.h"
#include "stompbox_pre.h"
#include "stompbox_eq.h"
#include "stompbox_fx2.h"
#include "stompbox_dd.h"
#include "stompbox_ce.h"
#include "stompbox_rv.h"
#include "stompbox_fv.h"
#include "stompbox_ns.h"
#include "stompbox_dgt.h"

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
	
	QList<signed int> fx;
	fx << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13;
	/*    FX1  CS   WAH  LP   OD   PRE  EQ   FX2  DD   CE   RV    FV    NS    DGT */
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

	QObject::connect(this, SIGNAL(valueChanged(QString, QString, QString)), 
		display, SLOT(setValueDisplay(QString, QString, QString)));
	QObject::connect(panelBar, SIGNAL(resizeSignal(int)), this, SLOT(setWidth(int)));
	QObject::connect(panelBar, SIGNAL(collapseSignal()), this, SLOT(setCollapse()));
	QObject::connect(this, SIGNAL(setCollapseState(bool)), panelBar, SIGNAL(collapseState(bool)));
	QObject::connect(this, SIGNAL(setDisplayPos(QPoint)), display, SLOT(setPos(QPoint)));
	QObject::connect(this, SIGNAL(setFloorPanelBarPos(QPoint)), panelBar, SLOT(setPos(QPoint)));
	QObject::connect(this->parent(), SIGNAL(updateSignal()), this, SIGNAL(updateSignal()));
	QObject::connect(this, SIGNAL(updateSignal()), this, SLOT(updateStompBoxes()));

	QObject::connect(this, SIGNAL(setDragBarOffset(QList<int>)), panelBar, SIGNAL(setDragBarOffset(QList<int>)));
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
	QList<QPoint> fxPos; 
	
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
		fxPos.append(QPoint::QPoint(offset + x, y));
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
	/* FX1 */
	stompBox *fx1 = new stompbox_fx1(this);
	fx1->setId(0);
	fx1->setPos(this->getStompPos(0));
	this->stompBoxes.append(fx1);
	this->stompNames.append("fx1");

	/* COMP */	
	stompBox *cs = new stompbox_cs(this);
	cs->setId(1);
	cs->setPos(this->getStompPos(1));
	this->stompBoxes.append(cs);
	this->stompNames.append("cs");

	/* WAH */
	stompBox *wah = new stompbox_wah(this);
	wah->setId(2);
	wah->setPos(this->getStompPos(2));
	this->stompBoxes.append(wah);
	this->stompNames.append("wah");	

	/* LOOP	*/
	stompBox *lp = new stompbox_lp(this);
	lp->setId(3);
	lp->setPos(this->getStompPos(3));
	this->stompBoxes.append(lp);
	this->stompNames.append("lp");	

	/* OD/DS */
	stompBox *od = new stompbox_od(this);
	od->setId(4);
	od->setPos(this->getStompPos(4));
	this->stompBoxes.append(od);
	this->stompNames.append("od");	

	/* AMP */ 
	stompBox *pre = new stompbox_pre(this);
	pre->setId(5);
	pre->setPos(this->getStompPos(5));
	this->stompBoxes.append(pre);
	this->stompNames.append("pre");

	/* EQ */
	stompBox *eq = new stompbox_eq(this);
	eq->setId(6);
	eq->setPos(this->getStompPos(6));
	this->stompBoxes.append(eq);
	this->stompNames.append("eq");

	/* FX 2 */
	stompBox *fx2 = new stompbox_fx2(this);
	fx2->setId(7);
	fx2->setPos(this->getStompPos(7));
	this->stompBoxes.append(fx2);
	this->stompNames.append("fx2");

	/* Delay */
	stompBox *dd = new stompbox_dd(this);
	dd->setId(8);
	dd->setPos(this->getStompPos(8));
	this->stompBoxes.append(dd);
	this->stompNames.append("dd");

	/* Chorus */
	stompBox *ce = new stompbox_ce(this);
	ce->setId(9);
	ce->setPos(this->getStompPos(9));
	this->stompBoxes.append(ce);
	this->stompNames.append("ce");

	/* REVERB */
	stompBox *rv = new stompbox_rv(this);
	rv->setId(10);
	rv->setPos(this->getStompPos(10));
	this->stompBoxes.append(rv);
	this->stompNames.append("rv");

	/* VOLUME */
	stompBox *fv = new stompbox_fv(this);
	fv->setId(11);
	fv->setPos(this->getStompPos(11));
	this->stompBoxes.append(fv);
	this->stompNames.append("fv");

	/* NS */
	stompBox *ns = new stompbox_ns(this);
	ns->setId(12);
	ns->setPos(this->getStompPos(12));
	this->stompBoxes.append(ns);
	this->stompNames.append("ns");

	/* D-OUT */
	stompBox *dgt = new stompbox_dgt(this);
	dgt->setId(13);
	dgt->setPos(this->getStompPos(13));
	this->stompBoxes.append(dgt);
	this->stompNames.append("dgt");
};

void floorBoard::setStomps(QList<QString> stompOrder)
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
	QList<QString> fxChain = sysxIO->getFileSource("11", "00");

	MidiTable *midiTable = MidiTable::Instance();
	QList<QString> stompOrder;
	for(int i=11;i<fxChain.size() - 2;i++ )
	{
		stompOrder.append( midiTable->getMidiMap("Stucture", "11", "00", "00", fxChain.at(i)).name.toLower() );
	};
	setStomps(stompOrder);
};