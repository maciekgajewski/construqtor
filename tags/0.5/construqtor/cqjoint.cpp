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
#include "cqjoint.h"
#include "cqworld.h"
#include "cqitemtypes.h"
#include "cqphysicalbody.h"

// XML tags
static const char* TAG_BODY1	= "body1";
static const char* TAG_BODY2	= "body2";

// =========================== constructor ===================
CqJoint::CqJoint( CqItem* parent )
	: CqItem( parent )
{
	init();
}

// =========================== destructor ===================
CqJoint::~CqJoint()
{
	// remove from bodies list
	if ( _pBody1 ) _pBody1->removeJoint( this );
	if ( _pBody2 ) _pBody2->removeJoint( this );
	
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
	if ( _pBody1 ) _pBody1->addJoint( this );
	if ( _pBody2 ) _pBody2->addJoint( this );
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
	
	// remove from body lists
	if ( _pBody1 ) _pBody1->removeJoint( this );
	if ( _pBody2 ) _pBody2->removeJoint( this );
	
	// delete this
	deleteLater(); // deffered deletion, a bless!
}

// ================================= store ========================
void CqJoint::store( CqElement& element ) const
{
	CqItem::store( element );
	
	element.appendItemPointer( TAG_BODY1, _pBody1 );
	element.appendItemPointer( TAG_BODY2, _pBody2 );
	
	// TODO any joint physical properties here
}

// ================================= load ========================
void CqJoint::load( const CqElement& element )
{
	CqItem::load( element );

	CqPhysicalBody* pB1 = (CqPhysicalBody*)element.readItemPointer( TAG_BODY1 );
	CqPhysicalBody* pB2 = (CqPhysicalBody*)element.readItemPointer( TAG_BODY2 );
	setConnectedBodies( pB1, pB2 );	

}

// ====================================================================
void CqJoint::recreateJoint()
{
	if ( _pJoint )
	{
		CqWorld* pWorld = world();
		Q_ASSERT( pWorld );
		
		destroyJoint( pWorld );
		_pJoint = createJoint( pWorld );
	}
}

// ====================================================================
void CqJoint::wakeUpBodies()
{
	if ( _pBody1 ) _pBody1->wakeUp();
	if ( _pBody2 ) _pBody2->wakeUp();
}

// EOF

