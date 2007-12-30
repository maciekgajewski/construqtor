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
#ifndef CQGROUPITEM_H
#define CQGROUPITEM_H

// local
#include "cqcompounditem.h"
class CqJoint;
class CqPhysicalBody;

/**
	Group item, which is used to 'bind' itmes togheter temporarly.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqGroupItem : public CqCompoundItem
{
public:
	CqGroupItem( QGraphicsItem* pParent = NULL );
	CqGroupItem( QList<CqItem*> items, QGraphicsItem* pParent = NULL );
	virtual ~CqGroupItem();
	
	void setGroupContent( QList<CqItem*> items );	///< Sets entire group content at once
	void addToGroup( QList<CqItem*> items );	///< Moves multiple items to groups
	void addToGroup( CqItem* pItem );			///< Moves single item to group
	
	void removeFromGroup( CqItem* pItem );		///< Removes item from group
	void clearGroupContent();					///< Removes all items from group
	
	// editor behavior
	virtual bool canBeMoved() const;
	virtual bool canBeRotated() const;

private:

	// methods
	void init();
	void updateConnectionLists();				///< Updates list of conncted boides and joints
	
	QList< CqJoint* >	_connectedJoints;		///< List of joins conncted to group
	QList< CqPhysicalBody* > _connectedBodies;	///< List of conected bodies
	bool				_blockConnectionsUpdate;	///< Blocks update of connection lists
};

#endif // CQGROUPITEM_H

// EOF


