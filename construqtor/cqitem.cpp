/***************************************************************************
 *   Copyright (C) 2007 by Maciek Gajewski                                 *
 *   maciej.gajewski0@gmail.com                                            *
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
#include "cqcompounditem.h"

// constants - xml tags
static const char* TAG_CENTER	= "center";
static const char* TAG_ROTATION	= "rotation";
static const char* TAG_POSITION	= "position";
static const char* TAG_NAME		= "name";
static const char* TAG_FLAGS	= "flags";


// ========================== constructor ======================
CqItem::CqItem( QGraphicsItem* parent )
	: QGraphicsItem(parent)
{
	init();
}

// ========================== destructor ======================
CqItem::~CqItem()
{
	// remove self from parents lists
	CqCompoundItem* pCompoundParent = dynamic_cast<CqCompoundItem*>( _pPhysicalParent );
	if ( pCompoundParent )
	{
		pCompoundParent->removeChild( this );
	}
}

// ==================================== init ==================
void CqItem::init()
{
	setFlag( QGraphicsItem::ItemIsSelectable, true ); // TODO test - maybe better would be to turn it off and use own selection
	setAcceptedMouseButtons( Qt::NoButton );
	
	_pSimulation = NULL;
	_pWorld = NULL;
	_flags = 0; // no flag set by default
	_pPhysicalParent = NULL;
	_selected = false;
	_rotation = 0.0;
	
	_id = QUuid::createUuid(); // random id
}

// =========================== simulation step ===================
void CqItem::simulationStep()
{
	updatePosToPhysical();
	// TODO split these two
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

// ======================= set rotation [radians] ==========================
void CqItem::setRotationRadians( double radians )
{
	if ( fabs( _rotation - radians ) > 0.01 ) // some fuse to avoid frequrnt updates
	{
		prepareGeometryChange();
		_rotation = radians;
		QTransform t;
		// rotate around center
		t.translate( _center.x(), _center.y() );
		t.rotateRadians( _rotation );
		t.translate( -_center.x(), - _center.y() );
		setTransform( t );
	}
}

// ======================== map to physical ==================================
QPointF CqItem::mapToWorld( const QPointF& local )
{
	// TODO currently simple imeplementation, when scene coords are physical coords
	return mapToScene( local );
	// TODO what about rotation?
}

// ========================== map to physical =================================
/// maps current rotation to physical
double	CqItem::mapToWorld( double rotation )
{
	CqItem* pParent = physicalParent();
	
	if ( pParent )
	{
		return rotation + pParent->mapToWorld( pParent->rotationRadians() );
	}
	else
	{
		return rotation;
	}
}

// ============================ map from physical ==========================
QPointF CqItem::mapFromWorld( const QPointF& physical )
{
	return mapFromScene( physical );
}

// ============================ map from physical ==========================
double CqItem::mapFromWorld( double rotation )
{
	CqItem* pParent = physicalParent();
	
	if ( pParent )
	{
		return rotation - pParent->mapToWorld( pParent->rotationRadians() );
	}
	else
	{
		return rotation;
	}
}

// ================================ set world ===================================
void CqItem::setWorld ( CqWorld* pWorld )
{
	_pWorld = pWorld;

}

// ============================== set selected =============================
void CqItem::setSelected( bool selected )
{
	if ( _selected != selected )
	{
		_selected = selected;
		update();
	}
}

// ============================== world pos =========================
QPointF CqItem::worldPos() const
{
	if ( _pPhysicalParent )
	{
		return _pPhysicalParent->mapToWorld( pos() );
	}
	
	return pos();
}

// ================================= wordl rotation =====================
double CqItem::worldRotation() const
{
	if ( _pPhysicalParent )
	{
		return _pPhysicalParent->mapToWorld( _rotation );
	}
	else
	{
		return _rotation;
	}
}

// =============================== set world pos =========================
void CqItem::setWorldPos( const QPointF pos )
{
	if ( _pPhysicalParent )
	{
		QPointF p = _pPhysicalParent->mapFromWorld( pos );
		setPos( p );
	}
	else
	{
		setPos( pos );
	}
}

// =============================== set world rotation ======================
void CqItem::setWorldRotation( double rotation )
{
	if ( _pPhysicalParent )
	{
		setRotationRadians( _pPhysicalParent->mapFromWorld( rotation ) );
	}
	else
	{
		setRotationRadians( rotation );
	}
}

// ================================ store ==============================
void CqItem::store( CqElement& element ) const
{
	element.appendPointF( TAG_CENTER, _center );
	element.appendPointF( TAG_POSITION, pos() );
	element.appendString( TAG_NAME, _name );
	element.appendDouble( TAG_ROTATION, _rotation );
	element.appendInt( TAG_FLAGS, _flags );
	
}

// ================================ load =============================
void CqItem::load( const CqElement& element )
{
	_center = element.readPointF( TAG_CENTER );
	setPhysicalPos( element.readPointF( TAG_POSITION ) );
	_name = element.readString( TAG_NAME );
	setPhysicalRotation( element.readDouble( TAG_ROTATION ) );
	_flags = element.readInt( TAG_FLAGS );
}

// EOF




