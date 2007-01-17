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

#include <QList>
#include <QVector>

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

	bankTreeList *bankList = new bankTreeList(this);
	
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

	/*floorBoardDisplay *display2 = new floorBoardDisplay(this);
	display2->setPos(liberainPos);*/

	QObject::connect(this, SIGNAL( resizeSignal(QRect) ),
                bankList, SLOT( updateSize(QRect) ) );
	QObject::connect(display, SIGNAL(connectedSignal()), 
		bankList, SLOT(connectedSignal()));
	QObject::connect(this, SIGNAL(valueChanged(QString, QString, QString)), 
		display, SLOT(setValueDisplay(QString, QString, QString)));
	QObject::connect(panelBar, SIGNAL(resizeSignal(int)), 
		this, SLOT(setWidth(int)));
	QObject::connect(panelBar, SIGNAL(collapseSignal()), 
		this, SLOT(setCollapse()));
	QObject::connect(this, SIGNAL(setCollapseState(bool)), 
		panelBar, SIGNAL(collapseState(bool)));
	QObject::connect(this, SIGNAL(setDisplayPos(QPoint)), 
		display, SLOT(setPos(QPoint)));
	QObject::connect(this, SIGNAL(setFloorPanelBarPos(QPoint)), 
		panelBar, SLOT(setPos(QPoint)));
	QObject::connect(this->parent(), SIGNAL(updateSignal()), 
		this, SIGNAL(updateSignal()));
	QObject::connect(this, SIGNAL(updateSignal()), 
		this, SLOT(updateStompBoxes()));

	QObject::connect(panelBar, SIGNAL(showDragBar(QPoint)), 
		this, SIGNAL(showDragBar(QPoint)));
	QObject::connect(panelBar, SIGNAL(hideDragBar()), 
		this, SIGNAL(hideDragBar()));

	bool ok;
	Preferences *preferences = Preferences::Instance();
	QString collapseState = preferences->getPreferences("Window", "Collapsed", "bool");
	QString width = preferences->getPreferences("Window", "Collapsed", "width");
	QString defaultwidth = preferences->getPreferences("Window", "Collapsed", "defaultwidth");
	if(width.isEmpty()){ width = defaultwidth; }

	this->l_floorSize = QSize::QSize(width.toInt(&ok, 10), floorSize.height());
	
	if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true")
	{
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
		//this->l_floorSize = QSize::QSize(defaultwidth.toInt(&ok, 10), floorSize.height());
		this->setSize(minSize);
		this->colapseState = false;
		emit setCollapseState(false);
	};

	emit updateSignal();
};

floorBoard::~floorBoard()
{
	Preferences *preferences = Preferences::Instance();
	if(preferences->getPreferences("Window", "Restore", "sidepanel")=="true")
	{
		preferences->setPreferences("Window", "Collapsed", "width", QString::number(this->l_floorSize.width(), 10));
	}
	else
	{
		preferences->setPreferences("Window", "Collapsed", "width", "");
	};
	preferences->setPreferences("Window", "Collapsed", "bool", QString(this->colapseState?"true":"false"));
	preferences->savePreferences();
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

	// Draw LiberianBar
	QRectF sourceLiberianBar(0.0, 0.0, imageInfoBar.width(), imageInfoBar.height());
	QRectF targetLiberianBar(offset, (imageFloor.height() - imageInfoBar.height()) - 2, imageInfoBar.width(), imageInfoBar.height());
	painter.drawPixmap(targetLiberianBar, imageInfoBar, sourceLiberianBar);

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

	/*QPoint newLiberainPos = QPoint::QPoint(offset, floorHeight);
	this->liberainPos = newLiberainPos;*/

	QRect newBankListRect = QRect::QRect(borderWidth, borderWidth, offset - panelBarOffset - (borderWidth*2), floorHeight - (borderWidth*2));
	emit resizeSignal(newBankListRect);
};

void floorBoard::dragEnterEvent(QDragEnterEvent *event)
{
	QByteArray data = event->mimeData()->data("text/uri-list");
	QString uri(data);
	
	if (event->mimeData()->hasFormat("application/x-stompbox") ||
		uri.contains(".syx", Qt::CaseInsensitive) &&
		event->answerRect().intersects(this->geometry())) 
	{
        if (children().contains(event->source())) 
		{
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
	
	if ( event->mimeData()->hasFormat("application/x-stompbox") ||
		uri.contains(".syx", Qt::CaseInsensitive) &&
		event->answerRect().intersects(this->geometry()) ) 
	{
        if (children().contains(event->source())) 
		{
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
		
		int destIndex = -1; // Set to out of range by default.
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
 

		if(destIndex > -1 && destIndex < fx.size() + 1)  
			// Make sure we are not dropping the stomp out of range. 
		{
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

			SysxIO *sysxIO = SysxIO::Instance();
			for(int index=0;index<fx.size();index++)
			{
				QString hexIndex = QString::number(fx.indexOf(index), 16).toUpper();
				if(hexIndex.length() < 2) hexIndex.prepend("0");
				
				QString hexValue = QString::number(index, 16).toUpper();
				if(hexValue.length() < 2) hexValue.prepend("0");
		
				sysxIO->setFileSource("11", "00", hexIndex, hexValue);
			};
		}
		else
		{
			event->ignore();
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
				
				SysxIO *sysxIO = SysxIO::Instance();
				sysxIO->setFileName(filePath);
				
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
	//for(unsigned int i=0;i<14;i++)
	for(int i=13;i>=0;i--)
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
	QList<signed int> fx;
	fx << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13;
	/*   FX   CS   WAH   LP   OD   PRE  EQ   FX2  DD   CE   RV    NS    FV    DGT :depend on midi.xml   */
	this->fx = fx;

	QVector<QString> initStompNames(14);
	this->stompNames = initStompNames.toList();;

	QVector<stompBox *> initStompBoxes(14);
	this->stompBoxes = initStompBoxes.toList();;

	MidiTable *midiTable = MidiTable::Instance();
	Midi midimap = midiTable->getMidiMap("Stucture", "11", "00", "00");
	QList<int> fxID;
	QList<QString> fxNAMES;
	for(int i=0;i<=13;i++)
	{
		bool ok;
		fxID.append(midimap.level.at(i).value.toInt(&ok, 16));
		fxNAMES.append(midimap.level.at(i).name);
	};

	/* FX1 */
	stompBox *fx1 = new stompbox_fx1(this);
	fx1->setId( fxID.at(fxNAMES.indexOf("FX1")) );
	fx1->setPos(this->getStompPos(fx1->getId()));
	this->stompBoxes.replace(fx1->getId(), fx1);
	this->stompNames.replace(fx1->getId(), "FX1");

	/* COMP */	
	stompBox *cs = new stompbox_cs(this);
	cs->setId( fxID.at(fxNAMES.indexOf("CS")) );
	cs->setPos(this->getStompPos(cs->getId()));
	this->stompBoxes.replace(cs->getId(), cs);
	this->stompNames.replace(cs->getId(), "CS");

	/* WAH */
	stompBox *wah = new stompbox_wah(this);
	wah->setId( fxID.at(fxNAMES.indexOf("WAH")) );
	wah->setPos(this->getStompPos(wah->getId()));
	this->stompBoxes.replace(wah->getId(), wah);
	this->stompNames.replace(wah->getId(), "WAH");

	/* LOOP	*/
	stompBox *lp = new stompbox_lp(this);
	lp->setId( fxID.at(fxNAMES.indexOf("LP")) );
	lp->setPos(this->getStompPos(lp->getId()));
	this->stompBoxes.replace(lp->getId(), lp);
	this->stompNames.replace(lp->getId(), "LP");

	/* OD/DS */
	stompBox *od = new stompbox_od(this);
	od->setId( fxID.at(fxNAMES.indexOf("OD")) );
	od->setPos(this->getStompPos(od->getId()));
	this->stompBoxes.replace(od->getId(), od);
	this->stompNames.replace(od->getId(), "OD");

	/* AMP */ 
	stompBox *pre = new stompbox_pre(this);
	pre->setId( fxID.at(fxNAMES.indexOf("PRE")) );
	pre->setPos(this->getStompPos(pre->getId()));
	this->stompBoxes.replace(pre->getId(), pre);
	this->stompNames.replace(pre->getId(), "PRE");

	/* EQ */
	stompBox *eq = new stompbox_eq(this);
	eq->setId( fxID.at(fxNAMES.indexOf("EQ")) );
	eq->setPos(this->getStompPos(eq->getId()));
	this->stompBoxes.replace(eq->getId(), eq);
	this->stompNames.replace(eq->getId(), "EQ");

	/* FX 2 */
	stompBox *fx2 = new stompbox_fx2(this);
	fx2->setId( fxID.at(fxNAMES.indexOf("FX2")) );
	fx2->setPos(this->getStompPos(fx2->getId()));
	this->stompBoxes.replace(fx2->getId(), fx2);
	this->stompNames.replace(fx2->getId(), "FX2");

	/* Delay */
	stompBox *dd = new stompbox_dd(this);
	dd->setId( fxID.at(fxNAMES.indexOf("DD")) );
	dd->setPos(this->getStompPos(dd->getId()));
	this->stompBoxes.replace(dd->getId(), dd);
	this->stompNames.replace(dd->getId(), "DD");

	/* Chorus */
	stompBox *ce = new stompbox_ce(this);
	ce->setId( fxID.at(fxNAMES.indexOf("CE")) );
	ce->setPos(this->getStompPos(ce->getId()));
	this->stompBoxes.replace(ce->getId(), ce);
	this->stompNames.replace(ce->getId(), "CE");

	/* REVERB */
	stompBox *rv = new stompbox_rv(this);
	rv->setId( fxID.at(fxNAMES.indexOf("RV")) );
	rv->setPos(this->getStompPos(rv->getId()));
	this->stompBoxes.replace(rv->getId(), rv);
	this->stompNames.replace(rv->getId(), "RV");

	/* NS */
	stompBox *ns = new stompbox_ns(this);
	ns->setId( fxID.at(fxNAMES.indexOf("NS")) );
	ns->setPos(this->getStompPos(ns->getId()));
	this->stompBoxes.replace(ns->getId(), ns);
	this->stompNames.replace(ns->getId(), "NS");

	/* D-OUT */
	stompBox *dgt = new stompbox_dgt(this);
	dgt->setId( fxID.at(fxNAMES.indexOf("DGT")) );
	dgt->setPos(this->getStompPos(dgt->getId()));
	this->stompBoxes.replace(dgt->getId(), dgt);
	this->stompNames.replace(dgt->getId(), "DGT");

	/* VOLUME */
	stompBox *fv = new stompbox_fv(this);
	fv->setId( fxID.at(fxNAMES.indexOf("FV")) );
	fv->setPos(this->getStompPos(fv->getId()));
	this->stompBoxes.replace(fv->getId(), fv);
	this->stompNames.replace(fv->getId(), "FV");
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
		stompOrder.append( midiTable->getMidiMap("Stucture", "11", "00", "00", fxChain.at(i)).name );
	};
	setStomps(stompOrder);
};