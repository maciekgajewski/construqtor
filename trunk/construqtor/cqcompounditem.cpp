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
#include <QPainter>

// local
#include "cqcompounditem.h"
#include "cqjoint.h"
#include "cqphysicalbody.h"

// tags
static const char* TAG_FOLLOWED_CHILD	= "followed";

// ======================== constructor =====================
CqCompoundItem::CqCompoundItem( QGraphicsItem *pParent )
 : CqItem(pParent)
{
	init();
}

// =========================== destructor =====================
CqCompoundItem::~CqCompoundItem()
{
	// none
}

// ================================== init =====================
void CqCompoundItem::init()
{
	_followedChild = NULL;
	_blockConnectionsUpdate	= false;
	_conectionUpdateNeeded	= false;
}

// =============================== add child =====================
void CqCompoundItem::addChild( CqItem* pChild )
{
	Q_ASSERT( pChild );
	
	// take ownership (in QObject terms )
	pChild->setParent( this );
	
	// make QGraphicsITem child
	pChild->setParentItem( this );
	
	// add to list
	_children.append( pChild );
	
	// introduce yourself to the child: "Luke, I am your father"
	pChild->setPhysicalParent( this );
	
	// if world and/or simulation know, show it to the children
	if ( world() )
	{
		pChild->setWorld( world() );
	}
	if ( simulation() )
	{
		pChild->setSimulation( simulation() );
	}
	
	if ( ! _blockConnectionsUpdate )
	{
		updateConnectionLists();
	}
	else
	{
		_conectionUpdateNeeded = true;
	}
}

// =============================== remove child ====================
void CqCompoundItem::removeChild( CqItem* pChild )
{
	Q_ASSERT( pChild );

	if ( ! _children.removeAll( pChild ) )
	{
		qWarning("CqCompoundItem::removeChild: item not a child" );
	}
	
	// return child to parent
	pChild->setPhysicalParent( physicalParent() ); // Cq physical parenthood 
}

// =============================== set world ====================
/// Sets world for each self and it's children
void CqCompoundItem::setWorld ( CqWorld* pWorld )
{
	CqItem::setWorld( pWorld );
	
	foreach ( CqItem* pChild, _children )
	{
		pChild->setWorld( pWorld );
	}
}

// ============================== set selected ===================
/// Select/unselect self and children
void CqCompoundItem::setSelected( bool selected )
{
	CqItem::setSelected( selected );
	
	foreach ( CqItem* pChild, _children )
	{
		pChild->setSelected( selected );
	}
}

// =============================== paint ============================
void CqCompoundItem::paint( QPainter* p, const QStyleOptionGraphicsItem*, QWidget* )
{
	// no painting
	
	/* excellent debug tool:
	QPainterPath s = shape();
	p->strokePath( s, QPen(Qt::red) );
	*/
	/* another debug tool 
	p->drawRect( boundingRect() );
	*/
}

// ================================= bounding rect ==================
QRectF CqCompoundItem::boundingRect() const
{
	QRectF rect;
	
	foreach ( CqItem* pChild, _children )
	{
		QRectF childRect = pChild->boundingRect();
		
		QTransform t;
		t.translate( pChild->pos().x(), pChild->pos().y() );
		t.rotateRadians( pChild->rotationRadians() );
		QRectF trasformedRect = t.map( childRect ).boundingRect();
		
		rect |= trasformedRect;
	}
	
	return rect;
}

// =================================== shape ======================
QPainterPath CqCompoundItem::shape() const
{
	QPainterPath s;
	
	foreach ( CqItem* pChild, _children )
	{
		QTransform m;
		m.translate( pChild->pos().x(), pChild->pos().y() );
		m.rotateRadians( pChild->rotationRadians() );
		QPainterPath childShape = pChild->shape();
		s.addPath( m.map( childShape ) );
	}
	
	s.setFillRule( Qt::WindingFill ); // fille entire shape
	
	return s;
}

// ============================= update physical pos ===========================
void CqCompoundItem::updatePhysicalPos()
{
	CqItem::updatePhysicalPos();
	
	// update children
	foreach( CqItem* pChild, _children )
	{
		pChild->updatePhysicalPos();
	}
}

// ================================== update pos to physical =====================
void CqCompoundItem::updatePosToPhysical()
{
	CqItem::updatePosToPhysical();
	
	// first - update followed child (if any)
	if ( _followedChild )
	{
		QPointF posBefore = _followedChild->pos(); // position before updating
		QPointF posBeforeInParent = mapToParent( posBefore );
		_followedChild->updatePosToPhysical();
		QPointF posAfterInParent = mapToParent( _followedChild->pos() );
		
		QPointF diff = posAfterInParent - posBeforeInParent;
		// follow child
		moveBy( diff.x(), diff.y() );
		
		// put child back on start pos
		_followedChild->setPos( posBefore );
	}
	
	// update children
	foreach( CqItem* pChild, _children )
	{
		if ( pChild != _followedChild ) // omit follwoed child
		{
			pChild->updatePosToPhysical();
		}
	}
}

// ================================ set simulation ===============================
void CqCompoundItem::setSimulation( CqSimulation* pSimulation )
{
	CqItem::setSimulation(pSimulation);
	
	// update children
	
	foreach( CqItem* pChild, _children )
	{
		pChild->setSimulation(pSimulation);
	}
}

// =================================== store ============================
void CqCompoundItem::store( CqElement& element ) const
{
	CqItem::store( element );
	
	// append child elements
	foreach( CqItem* pItem, _children )
	{
		element.appendItem( pItem );
	}
	
	// noe store followed child
	element.appendItemPointer( TAG_FOLLOWED_CHILD, _followedChild );
}

// =================================== load ============================
void CqCompoundItem::load( const CqElement& element )
{
	CqItem::load( element );
	
	// populate list of children with sub-elements
	while( CqItem* pItem = element.readItem() )
	{
		addChild( pItem );
	}
	
	// readfollowed child
	_followedChild = element.readItemPointer( TAG_FOLLOWED_CHILD );
	
	updateConnectionLists();
	
}
// ========CqCompoundItem=========================================================
bool CqCompoundItem::canBeMoved() const
{
	// check own movable flag
	if ( ! ( editorFlags() & Movable ) )
	{
		return false;
	}
	
	// check outside connections
	if ( ! _connectedBodies.isEmpty() || ! _connectedJoints.isEmpty() )
	{
		// sorry, Wonetou, but this mountain can not be moved
		return false;
	}
	
	return true;
}

// =================================================================
bool CqCompoundItem::canBeRotated() const
{
	// check own flag
	if ( ! ( editorFlags() & Rotatable ) )
	{
		return false;
	}
	
	// check outside connections
	if ( ! _connectedBodies.isEmpty() || ! _connectedJoints.isEmpty() )
	{
		// sorry, Wonetou, but this mountain can not be rotated
		return false;
	}
	
	return true;
}

// =================================================================
void CqCompoundItem::updateConnectionLists()
{
	// first - clear the lists
	_connectedBodies.clear();
	_connectedJoints.clear();
	
	const QList< CqItem* >& children = physicalChildren();
	
	foreach( CqItem* pChild, children )
	{
		// check body's joints
		CqPhysicalBody* pBody = dynamic_cast< CqPhysicalBody* > ( pChild );
		if ( pBody )
		{
			// add to list all body's joint, which does not belong to the group
			foreach( CqJoint* pJoint, pBody->joints() )
			{
				if ( ! children.contains( pJoint ) )
				{
					if ( ! _connectedJoints.contains( pJoint ) )
						_connectedJoints.append( pJoint );
				}
			}
		}
		
		// checks joint's bodies
		CqJoint* pJoint = dynamic_cast< CqJoint* > ( pChild );
		if ( pJoint )
		{
			if ( ! isChild( pJoint->body1() ) )
			{
				if ( ! _connectedBodies.contains( pJoint->body1() ) )
					_connectedBodies.append( pJoint->body1() );
			}
			if ( ! isChild( pJoint->body2() ) )
			{
				if ( ! _connectedBodies.contains( pJoint->body2() ) )
					_connectedBodies.append( pJoint->body2() );
			}
		}
	}
	_conectionUpdateNeeded = false;
	
	// TODO debug report
	//qDebug("%d connected joints and %d connected bodies found", _connectedJoints.size(), _connectedBodies.size() );
}

// ========================================================================
bool CqCompoundItem::isChild( CqItem* pItem ) const
{
	while( pItem )
	{
		if ( physicalChildren().contains( pItem ) )
		{
			return true;
		}
		
		// if no, try parent
		pItem = pItem->physicalParent();
	}
	
	return false;
}

// ========================================================================
void CqCompoundItem::blockConnectionUpdate()
{
	_blockConnectionsUpdate = true;
}

// ========================================================================
void CqCompoundItem::unblockCkonnectionUpdate()
{
	_blockConnectionsUpdate = false;
	if ( _conectionUpdateNeeded )
	{
		updateConnectionLists();
	}
}

// ========================================================================
void CqCompoundItem::childChanged( CqItem* )
{
	updateConnectionLists();
}

// ========================================================================
void CqCompoundItem::generateNewId()
{
	CqItem::generateNewId();
	
	foreach( CqItem* pChild, _children )
	{
		pChild->generateNewId();
	}
}

// EOF


