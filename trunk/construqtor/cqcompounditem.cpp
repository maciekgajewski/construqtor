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
	// none
}

// =============================== add child =====================
void CqCompoundItem::addChild( CqItem* pChild )
{
	Q_ASSERT( pChild );
	
	// sanitize child
	pChild->setCqFlags( pChild->cqFlags() & ~Selectable );
	
	// make QGraphicsITem child
	pChild->setParentItem( this );
	
	// add to list
	_children.append( pChild );
	
	// introduce yourself to the child: "Luke, I am your father"
	pChild->setPhysicalParent( this );
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
		QPainterPath childShape = pChild->shape();
		s.moveTo( pChild->pos() );
		s.addPath( childShape );
	}
	
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


// EOF



