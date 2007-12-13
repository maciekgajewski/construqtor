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

// box2d
#include "b2Body.h"

// local
#include "cqphysicalbody.h"
#include "cqitemtypes.h"
#include "cqjoint.h"
#include "cqsimulation.h"

// ========================== constructor =================================
CqPhysicalBody::CqPhysicalBody( QGraphicsItem* parent, CqWorld* world )
	: CqItem( parent )
	, world()( world )
{
	init();
}

CqPhysicalBody::CqPhysicalBody( CqWorld* world )
	: CqItem( NULL )
	, world()( world )
{
	init();
}

// =========================== destructor ===================================
CqPhysicalBody::~CqPhysicalBody()
{
}

// =========================== init ===================================
void CqPhysicalBody::init()
{
	_pBody = NULL;
	
	// make rotatable
	setCqFlags( cqFlags() | Rotatable );
}

// =========================== set world ===================================
/// destroys current body, and creates new one. Calls createShape() along the way
void CqPhysicalBody::recreateBody()
{
	if ( world() )
	{
		if ( _pBody )
		{
			// destroy current body
			destroyBody( world() );
		}
		// NOTE: body will be created when needed, by simulation with assureBodyCreated()
	}
}

// =========================== create body ===================================
void CqPhysicalBody::createBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	
	b2BodyDef bodyDef;
	
	// create body shape(s)
	QList< b2ShapeDef* > shapes = createShape();
	
	// add shapes to body
	foreach( b2ShapeDef* pShape, shapes )
	{
		bodyDef.AddShape( pShape );
	}
	
	// attach pointer to self to the box2d body
	// TODO is this needed?
	bodyDef.userData = this;
	
	// damping TODO: experimental
	bodyDef.linearDamping	= 0.01;
	bodyDef.angularDamping	= 0.01;
	
	// create body
	_pBody = pWorld->CreateBody(&bodyDef);
	
	// set body position
	updatePhysicalPos();
	
	// delete shapes
	foreach ( b2ShapeDef* pShape, shapes )
	{
		delete pShape;
	}
	
}

// =========================== simulaton step ===================================
/// Called by simulation after each simulation step.
/// Updates Grpahics ITem position and rotation to body's
void CqPhysicalBody::simulationStep()
{
	// call inherited
	CqItem::simulationStep();
	
	if ( _pBody )
	{
		b2Vec2		b2pos		= _pBody->GetCenterPosition();
		double		b2rotation	= _pBody->GetRotation();
		
		setPos( mapFromPhysical( QPointF( b2pos.x, b2pos.y ) ) );
		setRotationRadians( mapFromPhysical( b2rotation ) );
		
	}
}

// =========================== assure body created ===================================
/// Called by simulation before it starts.
/// It couses body creation on bodies which does not have body yet
/// This method is called late, so body creation is deffered as much as possible
void CqPhysicalBody::assureBodyCreated()
{
	Q_ASSERT( world() );
	
	if ( ! _pBody )
	{
		createBody(world());
	}
}

// =========================== destroy body ===================================
void CqPhysicalBody::destroyBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	Q_ASSERT( _pBody );
	
	pWorld->DestroyBody( _pBody );
	_pBody = NULL;
}

// =========================== type  ===================
int CqPhysicalBody::type() const
{
	return CQ_BODY;
}

// =================================== breaks all atached joints ===
void CqPhysicalBody::breakAllJoints()
{
	// get copy of joint list, couse they will be removed from 'official' list
	// during the process
	QList< CqJoint* > joints = _joints;
	
	foreach( CqJoint* pJoint, joints )
	{
		pJoint->breakJoint();
	}
}

// ============================== update physical pos ===============
void CqPhysicalBody::updatePhysicalPos()
{
	CqItem::updatePhysicalPos();
	if ( _pBody )
	{
		// translate coords to physical
		QPointF pp	= mapToPhysical( pos() );
		double r	= mapToPhysical( rotationRadians() );
		
		_pBody->SetCenterPosition( b2Vec2( pp.x(), pp.y() ), r );
		update();
	}
}

// =============================== add joint ======================
void CqPhysicalBody::addJoint( CqJoint* pJoint )
{
	// duplicate check
	if ( _joints.contains( pJoint ) )
	{
		qWarning("joint added once again to same body");
	}
	else
	{
		_joints.append( pJoint );
	}
}

// =============================== remove joint ======================
void CqPhysicalBody::removeJoint( CqJoint* pJoint )
{
	// existence check
	if ( _joints.contains( pJoint ) )
	{
		_joints.removeAll( pJoint );
	}
	else
	{
		qWarning("joint remove from body, to each it doesn;t belogns");
	}
}

// EOF

