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
#include "cqphysicalbody.h"

// =========================== constructor ===================
CqJoint::CqJoint(CqWorld* world)
	: CqItem()
	, world()( world )
{
	init();
}
// =========================== constructor ===================
CqJoint::CqJoint(QGraphicsItem* parent, CqWorld* world)
	: CqItem(parent)
	, world()( world )
{
	init();
}

// =========================== destructor ===================
CqJoint::~CqJoint()
{
	if ( _pJoint )
	{
		Q_ASSERT( world() );
		destroyJoint( world() );
	}
}

// =========================== init ===================
void CqJoint::init()
{
	_pBody1 = NULL;
	_pBody2 = NULL;
	_pJoint = NULL;
}

// =========================== assure joint created  ===================
void CqJoint::assureJointCreated()
{
	Q_ASSERT( world() );
	Q_ASSERT( _pBody1 );
	Q_ASSERT( _pBody2 );
	
	if ( ! _pJoint )
	{
		_pJoint = createJoint(world());
	}
}

// =========================== set connected bodies  ===================
void CqJoint::setConnectedBodies( CqPhysicalBody* pBody1, CqPhysicalBody* pBody2 )
{
	// TODO remove joints from earlier bodies
	_pBody1 = pBody1;
	_pBody2 = pBody2;
	// let bodoes know they have new joint
	_pBody1->addJoint( this );
	_pBody2->addJoint( this );
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

// ============================ break joint ==============
/// Breaks the joint, frees objectm removes it from scene etc
void CqJoint::breakJoint()
{
	Q_ASSERT( world() );
	
	// remove from body lists
	if ( _pBody1 ) _pBody1->removeJoint( this );
	if ( _pBody2 ) _pBody2->removeJoint( this );
	
	// delete this
	delete this;
}

// EOF

