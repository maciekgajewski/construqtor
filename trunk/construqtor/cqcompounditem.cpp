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

// local
#include "cqcompounditem.h"

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
}

// =============================== add child =====================
void CqCompoundItem::addChild( CqItem* pChild )
{
	Q_ASSERT( pChild );
	
	// sanitize child
	pChild->setEditorFlags( pChild->editorFlags() & ~Selectable );
	
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
void CqCompoundItem::paint( QPainter*, const QStyleOptionGraphicsItem*, QWidget* )
{
	// no painting
}

// ================================= bounding rect ==================
QRectF CqCompoundItem::boundingRect() const
{
	QRectF rect;
	
	foreach ( CqItem* pChild, _children )
	{
		QRectF childRect = pChild->boundingRect();
		rect |= childRect.translated( pChild->pos() );
	}
	
	return rect;
}

// =================================== shape ======================
QPainterPath CqCompoundItem::shape() const
{
	QPainterPath s;
	
	foreach ( CqItem* pChild, _children )
	{
		QMatrix m;
		m.translate( pChild->pos().x(), pChild->pos().y() );
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

// EOF



