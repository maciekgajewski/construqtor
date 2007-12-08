/***************************************************************************
 *   Copyright (C) 2007 by Maciek Gajewski   *
 *   maciej.gajewski0@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

// Qt
#include <QMouseEvent>


// CQ
#include "cqitem.h"

// Local
#include "mainview.h"


// ========================= constructor ======================
MainView::MainView(QWidget* parent): QGraphicsView(parent)
{
}

// ========================= constructor ======================
MainView::MainView(QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent)
{
}


// ========================= destructor ======================
MainView::~MainView()
{
}

// ============================= dbl click ===================
void MainView::mouseDoubleClickEvent(QMouseEvent* event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
}

// =========================== move =====================
void MainView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

// =========================== press =====================
void MainView::mousePressEvent(QMouseEvent* event)
{
	QGraphicsView::mousePressEvent(event);
	
	/*
	// select with left button
	if ( event->button() == Qt::LeftButton )
	{
		QPoint pos = event->pos();
		QList<QGraphicsItem *> itemList = items( pos );
		
		// simplest implementation - select first selectable CqItem
		foreach( QGraphicsItem* pItem, itemList )
		{
			CqItem* pcqitem = dynamic_cast<CqItem*>( pItem );
			if ( pcqitem )
			{
				// selectable
				if ( pcqitem->flags() & QGraphicsItem::ItemIsSelectable )
				{
					pcqitem->setSelected( ! pcqitem->isSelected() );
					pcqitem->update();
					break;
				}
			}
		}
	}
	*/
}

// =========================== release =====================
void MainView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

