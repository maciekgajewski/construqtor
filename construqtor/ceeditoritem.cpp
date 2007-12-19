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

// libc
#include <math.h>

// Qt
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

// Cq
#include "cqitem.h"

// local
#include "ceeditoritem.h"

const double CeEditorItem::MoveHandler::SIZE	= 15.0;		///< Move handler size [px]
const double CeEditorItem::RotateHandler::SIZE	= 10.0;		///< Move handler size [px]
const double CeEditorItem::SIZE				= 60.0;		///< Editor's size [px]

// ============================== constructor =====================
CeEditorItem::CeEditorItem( CqItem* pItem )
	: QGraphicsItem(pItem)
{
	Q_ASSERT( pItem );
	_pItem			= pItem;
	_pMoveHandler	= NULL;
	_pRotateHandler	= NULL;
	createHandles();
	setPos( 0, 0); // sit exactly on edited item
	setZValue( 10.0 ); // TODO manage layers or what
}


// ============================== destructor =====================
CeEditorItem::~CeEditorItem()
{
	// none
}

// =================================== paint ==============================
void CeEditorItem::paint ( QPainter * pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	pPainter->setPen( QPen( Qt::black, 0, Qt::DotLine ) );
	
	pPainter->drawEllipse( -SIZE/2, -SIZE/2, SIZE, SIZE );
}

// =================================== bounding rect ==============================
QRectF CeEditorItem::boundingRect () const
{
	double brsize = SIZE * 1.1;
	return QRectF( - brsize/2.0, -brsize/2.0, brsize, brsize );
}

// =================================== bounding rect ==============================
void CeEditorItem::createHandles()
{
	// if item can be movable - create move handler
	if ( _pItem->canBeMoved() )
	{
		_pMoveHandler = new MoveHandler( _pItem, this );
	}
	
	// fopr rotatable items: rotation handler
	if ( _pItem->canBeRotated() )
	{
		_pRotateHandler = new RotateHandler( _pItem, this );
	}
}

// ============================== move handler constructor =========================
CeEditorItem::MoveHandler::MoveHandler( CqItem* pItem, CeEditorItem* parent )
	: QGraphicsItem( parent )
{
	_pItem = pItem;
	_dragging = false;
	setAcceptedMouseButtons( Qt::LeftButton );
}

// ==================================== move handler paint ============================
void CeEditorItem::MoveHandler::paint ( QPainter * pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	QRectF rect(- SIZE/2.0, -SIZE/2.0, SIZE, SIZE ); 
	pPainter->setPen( Qt::red );
	pPainter->setBrush( QColor( 0xff, 0xff, 0x00, 0x80 ) ); // 50% transparent yellow
	
	pPainter->drawRect( rect );
}

// ==================================== move handler bounding rect ====================
QRectF CeEditorItem::MoveHandler::boundingRect () const
{
	double brsize = SIZE * 1.1;
	return QRectF( - brsize/2.0, -brsize/2.0, brsize, brsize );
}

// ====================================== move handler mouse move ======================
void CeEditorItem::MoveHandler::mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	if ( _dragging )
	{
		
		// move object
		_pItem->setPhysicalPos( mapToScene( pEvent->pos() ) );
		
		
		pEvent->accept();
	}
}

// ====================================== mouse handler press ============================
void CeEditorItem::MoveHandler::mousePressEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	_dragging = true;
	pEvent->accept();
}

// ====================================== mouse handler release ==========================
void CeEditorItem::MoveHandler::mouseReleaseEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	_dragging = false;
	pEvent->accept();
}

// ==================================== hndle contains ==============================
bool CeEditorItem::handleContains( const QPointF& point) const
{
	if ( _pMoveHandler &&  _pMoveHandler->contains( _pMoveHandler->mapFromScene( point ) ) )
	{
		return true;
	}
	else if ( _pRotateHandler && _pRotateHandler->contains( _pRotateHandler->mapFromScene( point ) ) )
	{
		return true;
	}
	
	return false;
}

// ============================ Rotate Handler =======================
CeEditorItem::RotateHandler::RotateHandler( CqItem* pItem, CeEditorItem* parent )
	: QGraphicsItem( parent )
{
	_pItem = pItem;
	_dragging = false;
	setAcceptedMouseButtons( Qt::LeftButton );
	adjustPosToAngle();
}

// ============================= Rotate handle: adjust pos to angle =========
void CeEditorItem::RotateHandler::adjustPosToAngle()
{
	Q_ASSERT( _pItem );
	
	double angle = _pItem->rotationRadians();
	
	setPos( CeEditorItem::SIZE / 2.0, 0 );
}

// =========================== Rotate Hndler: paint ==================================
void CeEditorItem::RotateHandler::paint ( QPainter * pPainter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
	QRectF rect(- SIZE/2.0, -SIZE/2.0, SIZE, SIZE ); 
	pPainter->setPen( Qt::red );
	pPainter->setBrush( QColor( 0xff, 0xff, 0x00, 0x80 ) ); // 50% transparent yellow
	
	pPainter->drawRect( rect );
}

// =========================== Rotate Hndler: bounding rect ==================================
QRectF CeEditorItem::RotateHandler::boundingRect () const
{
	double brsize = SIZE * 1.1;
	return QRectF( - brsize/2.0, -brsize/2.0, brsize, brsize );
}

// ============================== Rotate Handler: move event ===============================
void CeEditorItem::RotateHandler::mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	if ( _dragging )
	{
		Q_ASSERT( _pItem );
		
		QPointF pointerPos	= mapToScene( pEvent->pos() );
		QPointF pivot		= _pItem->scenePos();
		QPointF diff		= pointerPos - pivot;
		
		double angle = atan2( diff.y(), diff.x() );
		
		_pItem->setPhysicalRotation( angle );
		adjustPosToAngle();
	}
}

// ============================== Rotate Handler: move press ===============================
void CeEditorItem::RotateHandler::mousePressEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	_dragging = true;
}

// ============================== Rotate Handler: move release ===============================
void CeEditorItem::RotateHandler::mouseReleaseEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	_dragging = false;
}

		

// EOF
