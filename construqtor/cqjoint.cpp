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
#include "cqjoint.h"
#include "cqworld.h"
#include "cqitemtypes.h"

// =========================== constructor ===================
CqJoint::CqJoint(CqWorld* world)
	: QGraphicsItem()
	, _pWorld( world )
{
	init();
}
// =========================== constructor ===================
CqJoint::CqJoint(QGraphicsItem* parent, CqWorld* world)
	: QGraphicsItem(parent)
	, _pWorld( world )
{
	init();
}

// =========================== destructor ===================
CqJoint::~CqJoint()
{
	// nope
}

// =========================== init ===================
void CqJoint::init()
{
	_pBody1 = NULL;
	_pBody2 = NULL;
}

// =========================== simulation step ===================
void CqJoint::simulationStep()
{
	// nothing here
}

// =========================== assure joint created  ===================
void CqJoint::assureJointCreated()
{
	Q_ASSERT( _pWorld );
	
	if ( ! _pJoint )
	{
		_pJoint = createJoint(_pWorld);
	}
}

// =========================== set connected bodies  ===================
void CqJoint::setConnectedBodies( CqPhysicalBody* pBody1, CqPhysicalBody* pBody2 )
{
	_pBody1 = pBody1;
	_pBody2 = pBody2;
}

// =========================== destroy joint  ===================
void CqJoint::destroyJoint( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	Q_ASSERT( _pJoint );
	
	pWorld->DestroyJoint( _pJoint );
	
	_pJoint = NULL;
}

// =========================== type  ===================
int CqJoint::type() const
{
	return CQ_JOINT;
}

// EOF

