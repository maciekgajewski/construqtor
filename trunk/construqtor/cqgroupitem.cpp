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

// local
#include "cqgroupitem.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqGroupItem );


// ============================== constructor =======================
CqGroupItem::CqGroupItem( CqItem* pParent )
	: CqCompoundItem( pParent )
{
	init();
}
// ============================== constructor =======================
CqGroupItem::CqGroupItem( QList<CqItem*> items, CqItem* pParent )
	: CqCompoundItem( pParent )
{
	init();
	setGroupContent( items );
}

// ==================================================================
void CqGroupItem::setGroupContent( QList<CqItem*> items )
{
	// TODO remove all children now
	if ( ! items.isEmpty() )
	{
		// calculate medium pos
		QPointF center;
		foreach( CqItem* pItem, items )
		{
			center += pItem->pos();
		}
		center /= items.size();
		
		setPhysicalPos( center );
		
		// add all items
		addToGroup( items );
		
		// follow first item - simplest solutio for now
		setFollowedChild( items[0] );
	}
}

// ============================== destructor ========================
CqGroupItem::~CqGroupItem()
{
	// nope
}

// ============================== init ========================
void CqGroupItem::init()
{
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
}

// =================================================================
void CqGroupItem::addToGroup( QList<CqItem*> items )
{
	blockConnectionUpdate();
		
	foreach( CqItem* pItem, items )
	{
		addToGroup( pItem );
	}
	
	unblockCkonnectionUpdate();
}

// =================================================================
void CqGroupItem::addToGroup( CqItem* pItem )
{
	QPointF worldPos		= pItem->worldPos();
	double worldRotation	= pItem->worldRotation();
	
	addChild( pItem );
	
	pItem->setWorldPos( worldPos );
	pItem->setWorldRotation( worldRotation );
}

// =================================================================
void CqGroupItem::removeFromGroup( CqItem* pItem )
{
	QPointF worldPos		= pItem->worldPos();
	double worldRotation	= pItem->worldRotation();
	
	removeChild( pItem );
	pItem->setPhysicalParent( physicalParent() ); // move item back to parent
	
	pItem->setWorldPos( worldPos );
	pItem->setWorldRotation( worldRotation );
	
}

// =================================================================
void CqGroupItem::clearGroupContent()
{
	blockConnectionUpdate();
	
	foreach( CqItem* pItem, physicalChildren() )
	{
		removeFromGroup( pItem );
	}
	
	unblockCkonnectionUpdate();
}

// EOF

