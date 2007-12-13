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
#include <QGraphicsSceneMouseEvent>

// local
#include "cqsimulation.h"
#include "cqitem.h"

// ========================== constructor ======================
CqItem::CqItem( QGraphicsItem* parent )
	: QGraphicsItem(parent)
{
	init();
}

// ========================== destructor ======================
CqItem::~CqItem()
{
	// none
}

// ==================================== init ==================
void CqItem::init()
{
	setFlag( QGraphicsItem::ItemIsSelectable, true ); // TODO test - maybe better would be to turn it off and use own selection
	setAcceptedMouseButtons( Qt::NoButton );
	
	_pSimulation = NULL;
	_pWorld = NULL;
	_flags = 0; // no flag set by default
}

// =========================== simulation step ===================
void CqItem::simulationStep()
{
	// nothing here
}

// =========================== simulationstarted ===================
void CqItem::simulationStarted()
{
	// nothing here
}

// =========================== simulation stopped ===================
void CqItem::simulationStopped()
{
	// nothing here
}

// ============================== mouse dbl click ==================
void CqItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	pEvent->ignore();
}

// ============================== mouse move ==================
void CqItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent  )
{
	pEvent->ignore();
}

// ============================== mouse press ==================
void CqItem::mousePressEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	pEvent->ignore();
}
// ============================== mouse release ==================
void CqItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent *  )
{
	// nope
}

// ======================== can item be moved here? =============
bool CqItem::canBeMovedHere( const QPointF& scenePos )
{
	return canBeMoved() && _pSimulation->canBeMovedHere( this, scenePos );
}

// ============================ can be selected =====================
bool CqItem::canBeSelected() const
{
	// only top-level items shoud be selected
	return ( _flags & Selectable ) && ( physicalParent() == NULL ) && _pSimulation->canBeSelected( this );
}

// ================================== can be moved ==================
bool CqItem::canBeMoved() const
{
	return ( _flags & Movable ) && _pSimulation->canBeMoved( this );
}

// ==================================== can be rotated ==============
bool CqItem::canBeRotated() const
{
	return ( _flags && Rotatable ) && _pSimulation->canBeRotated( this );
}

// ============================== set physical pos ===============
/// Proper way to move the item. It alse updates all neccesary data
void CqItem::setPhysicalPos( const QPointF& pos )
{
	setPos( pos );
	updatePhysicalPos();
}

// ============================== set physical rotation ===============
/// Proper way to rtate item. Also rotates physical counterpart
void CqItem::setPhysicalRotation( double radians )
{
	setRotationRadians( radians );
	updatePhysicalPos();
}

// ============================ physical children ======================
QList< CqItem* > CqItem::physicalChildren()
{
	QList< CqItem* > physicalChildren;
	QList< QGraphicsItem* > allChildren = children();
	
	foreach( QGraphicsItem* pItem, allChildren )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		if ( pCqItem )
		{
			physicalChildren.append( pCqItem );
		}
	}
	
	return physicalChildren;
}

// ============================== physical parent ======================
CqItem* CqItem::physicalParent()
{
	return dynamic_cast< CqItem* >( parentItem() );
}

// ============================== physical parent (const) ======================
const CqItem* CqItem::physicalParent() const
{
	return dynamic_cast< const CqItem* >( parentItem() );
}

// ======================= set rotation [radians] ==========================
void CqItem::setRotationRadians( double radians )
{
	if ( fabs( _rotation - radians ) > 0.01 ) // some fuse to avoid frequrnt updates
	{
		prepareGeometryChange();
		_rotation = radians;
	}
}

// ======================== map to physical ==================================
QPointF CqItem::mapToPhysical( const QPointF& local )
{
	// TODO currently simple imeplementation, when scene coords are physical coords
	return mapToScene( local );
	// TODO what about rotation?
}

// ========================== map to physical =================================
/// maps current rotation to physical
double	CqItem::mapToPhysical( double rotation )
{
	CqItem* pParent = physicalParent();
	
	if ( pParent )
	{
		return rotation + pParent->mapToPhysical( pParent->rotationRadians() );
	}
	else
	{
		return rotation;
	}
}

// ============================ map from physical ==========================
QPointF CqItem::mapFromPhysical( const QPointF& physical )
{
	return mapFromScene( physical );
}

// ============================ map from physical ==========================
double CqItem::mapFromPhysical( double rotation )
{
	CqItem* pParent = physicalParent();
	
	if ( pParent )
	{
		return rotation - pParent->mapToPhysical( pParent->rotationRadians() );
	}
	else
	{
		return rotation;
	}
}

// ============================= update physical pos ===========================
void CqItem::updatePhysicalPos()
{
	// update children
	QList< CqItem* > children = physicalChildren();
	
	foreach( CqItem* pChild, children )
	{
		pChild->updatePhysicalPos();
	}
	
}

// ================================ set world ===================================
void CqItem::setWorld ( CqWorld* pWorld )
{
	_pWorld = pWorld;

	QList< CqItem* > children = physicalChildren();
	
	foreach( CqItem* pItem, children )
	{
		pItem->setWorld( pItem );
	}	
}

// EOF




