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
#include "cqjoint.h"
#include "cqphysicalbody.h"

// ============================== constructor =======================
CqGroupItem::CqGroupItem( QGraphicsItem* pParent )
	: CqCompoundItem( pParent )
{
	init();
}
// ============================== constructor =======================
CqGroupItem::CqGroupItem( QList<CqItem*> items, QGraphicsItem* pParent )
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
	//save all children
	clearGroupContent();
}

// ============================== init ========================
void CqGroupItem::init()
{
	_blockConnectionsUpdate = false;
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
}

// =================================================================
void CqGroupItem::addToGroup( QList<CqItem*> items )
{
	_blockConnectionsUpdate = true;
	
	foreach( CqItem* pItem, items )
	{
		addToGroup( pItem );
	}
	
	_blockConnectionsUpdate =true;
	updateConnectionLists();
}

// =================================================================
void CqGroupItem::addToGroup( CqItem* pItem )
{
	QPointF worldPos		= pItem->worldPos();
	double worldRotation	= pItem->worldRotation();
	
	addChild( pItem );
	
	pItem->setWorldPos( worldPos );
	pItem->setWorldRotation( worldRotation );
	
	if ( ! _blockConnectionsUpdate )
	{
		updateConnectionLists();
	}
}

// =================================================================
void CqGroupItem::removeFromGroup( CqItem* pItem )
{
	QPointF worldPos		= pItem->worldPos();
	double worldRotation	= pItem->worldRotation();
	
	removeChild( pItem );
	
	pItem->setWorldPos( worldPos );
	pItem->setWorldRotation( worldRotation );
	
	if ( ! _blockConnectionsUpdate )
	{
		updateConnectionLists();
	}
}

// =================================================================
void CqGroupItem::clearGroupContent()
{
	_blockConnectionsUpdate = true;
	foreach( CqItem* pItem, physicalChildren() )
	{
		removeFromGroup( pItem );
	}
	
	_connectedBodies.clear();
	_connectedJoints.clear();
	_blockConnectionsUpdate = false;
}

// =================================================================
bool CqGroupItem::canBeMoved() const
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
	
	// query all children for flags only
	/* TODO or not
	foreach( CqItem* pItem, physicalChildren() )
	{
		if ( ! ( pItem->editorFlags() & Movable ) )
		{
			return false;
		}
	}
	*/
	
	return true;
}

// =================================================================
bool CqGroupItem::canBeRotated() const
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
	
	// query all children
	/* TODO or not
	foreach( CqItem* pItem, physicalChildren() )
	{
		if (! ( pItem->editorFlags() & Rotatable ) )
		{
			return false;
		}
	}
	*/
	
	return true;
}

// =================================================================
void CqGroupItem::updateConnectionLists()
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
			if ( ! children.contains( pJoint->body1() ) )
			{
				if ( ! _connectedBodies.contains( pJoint->body1() ) )
					_connectedBodies.append( pJoint->body1() );
			}
			if ( ! children.contains( pJoint->body2() ) )
			{
				if ( ! _connectedBodies.contains( pJoint->body2() ) )
					_connectedBodies.append( pJoint->body2() );
			}
		}
	}
	
	// TODO debug report
	qDebug("%d connected joints and %d connected bodies found", _connectedJoints.size(), _connectedBodies.size() );
}

