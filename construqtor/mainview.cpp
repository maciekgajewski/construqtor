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
#include <QWheelEvent>


// CQ
#include "cqitem.h"
#include "cqjoint.h"
#include "cqphysicalbody.h"

// Local
#include "mainview.h"
#include "ceeditoritem.h"


// ========================= constructor ======================
MainView::MainView(QWidget* parent): QGraphicsView(parent)
{
	init();
}

// ========================= constructor ======================
MainView::MainView(QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent)
{
	init();
}

// ========================== init =============================
void MainView::init()
{
	//_draggedItem = NULL;
	_pEditor = NULL;
	
	setRenderHint( QPainter::Antialiasing, true );
}

// ========================= destructor ======================
MainView::~MainView()
{
}

// ============================= dbl click ===================
void MainView::mouseDoubleClickEvent(QMouseEvent* pEvent )
{
	// QGraphicsView::mouseDoubleClickEvent(event);
	
	// break joint woth left double click
	if ( pEvent->button() == Qt::LeftButton )
	{
		breakJointUnderPoint( pEvent->pos() );
	}
}

// =========================== move =====================
void MainView::mouseMoveEvent(QMouseEvent* pEvent)
{
	QPointF pos = mapToScene( pEvent->pos() );
	emit pointerPos( pos.x(), pos.y() );
	
	QGraphicsView::mouseMoveEvent( pEvent );
	
	if ( pEvent->isAccepted() ) return;
	// drag
	/* dragging moved to editor item 
	if ( _draggedItem )
	{
		// get scene pos
		QPointF scenePos = mapToScene( pEvent->pos() );
		
		if ( _draggedItem->canBeMovedHere( scenePos ) )
		{
			_draggedItem->setPhysicalPos( scenePos );
		}
	}
	*/
	
}

// =========================== press =====================
void MainView::mousePressEvent(QMouseEvent* pEvent)
{
	// hanlde only if outside editor
	if ( _pEditor && _pEditor->handleContains( mapToScene( pEvent->pos() ) ) )
	{
		QGraphicsView::mousePressEvent( pEvent );
		return;
	}
	
	
	// select with left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		QPoint pos = pEvent->pos();
		
		unselectAll();
		
		// start drag
		/* no dragiing here
		startDragUnderPoint( pos );
		
		// if not dragging anything, at least try to select
		if ( ! _draggedItem )
		{
			selectUnderPoint( pos );
		}
		*/
			selectUnderPoint( pos );
	}
}

// =========================== release =====================
void MainView::mouseReleaseEvent(QMouseEvent* pEvent)
{
	QGraphicsView::mouseReleaseEvent( pEvent );
	
	if ( pEvent->isAccepted() ) return;
	
	/* no draggign, move to editor 
	if ( _draggedItem )
	{
		// stop dragging
		_draggedItem = NULL;
	}
	*/
}

// ======================== select under point ===========
void MainView::selectUnderPoint( const QPoint& pos )
{
	QList<QGraphicsItem *> itemList = items( pos );
	// simplest implementation - select first selectable CqItem
	foreach( QGraphicsItem* pItem, itemList )
	{
		CqItem* pcqitem = dynamic_cast<CqItem*>( pItem );
		if ( pcqitem )
		{
			// selectable
			if ( pcqitem->canBeSelected() )
			{
				pcqitem->setSelected( ! pcqitem->isSelected() );
				pcqitem->update();
				// create editor for selected
				delete _pEditor;
				_pEditor = new CeEditorItem( pcqitem );
				_pEditor->setMatrix( matrix().inverted() ); //set inverted transform
				break;
			}
		}
	}
}

// ========================= unselect all ================
void MainView::unselectAll()
{
	QList<QGraphicsItem *> itemList = items();
	foreach( QGraphicsItem* pItem, itemList )
	{
		pItem->setSelected( false );
	}
	
	// destroy editor
	delete _pEditor;
	_pEditor = NULL;
}

// ========================= start drag under point ================
/*
void MainView::startDragUnderPoint( const QPoint& pos )
{
	QList<QGraphicsItem *> itemList = items( pos );
	// simplest implementation - select first selectable CqItem
	foreach( QGraphicsItem* pItem, itemList )
	{
		CqItem* pcqitem = dynamic_cast<CqItem*>( pItem );
		if ( pcqitem )
		{
			// selectable
			if ( pcqitem->canBeMoved() )
			{
				pcqitem->setSelected( pcqitem->canBeSelected() );
				pcqitem->update();
				_draggedItem = pcqitem;
				break;
			}
		}
	}
}
*/

// ======================= break joint under point ==============
void MainView::breakJointUnderPoint( const QPoint& pos )
{
	QList<QGraphicsItem *> itemList = items( pos );
	// simplest implementation - break first body or joint discovered
	foreach( QGraphicsItem* pItem, itemList )
	{
		// body?
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		if ( pBody )
		{
			pBody->breakAllJoints();
			break;
		}
		// joint?
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		if ( pJoint )
		{
			pJoint->breakJoint();
		}
	}
}

// ======================= wheel event  ==============
void MainView::wheelEvent ( QWheelEvent * pEvent )
{
	double factor = 1.0;
	int delta = pEvent->delta();
	
	if ( delta > 0 )
	{
		factor = 1.0 + delta/1000.0;
	}
	else if ( delta < 0 )
	{
		factor = 1.0 / ( 1.0 - delta/1000.0 );
	}
	
	scale( factor, factor );
	// update editor if needed
	if ( _pEditor )
	{
		_pEditor->setMatrix( matrix().inverted() ); //set inverted transform
	}


}

// ========================= on simulation started ===========================
void MainView::simulationStarted()
{
	unselectAll();
}

// ========================= on simulation paused ===========================
void MainView::simulationPaused()
{
	// nope
}

// EOF

