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
	, _pWorld( world )
{
	init();
}

CqPhysicalBody::CqPhysicalBody( CqWorld* world )
	: CqItem( NULL )
	, _pWorld( world )
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
	_rotation = 0;
}

// =========================== set world ===================================
/// Sets pohysical world to use. b2d Body is (re) cretaed.
void CqPhysicalBody::setWorld ( CqWorld* pWorld )
{
	// TODO destroy burrent body if world already set
	if ( pWorld && pWorld != _pWorld )
	{
		_pWorld = pWorld;
		Q_ASSERT( _pBody ); // TODO this could be habndked - body rempoved from one world and addetd to another
	}
}

// =========================== set world ===================================
/// destroys current body, and creates new one. Calls createShape() along the way
void CqPhysicalBody::recreateBody()
{
	if ( _pWorld )
	{
		if ( _pBody )
		{
			// destroy current body
			destroyBody( _pWorld );
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
	
	// create body
	_pBody = pWorld->CreateBody(&bodyDef);
	
	// set body position
	QPointF position = pos();
	b2Vec2 b2pos;
	b2pos.x = position.x();
	b2pos.y = position.y();
	_pBody->SetCenterPosition( b2pos, _rotation );
	
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
		
		setPos( b2pos.x, b2pos.y );
		setRotationRadians( b2rotation );
		
	}
}

// =========================== assure body created ===================================
/// Called by simulation before it starts.
/// It couses body creation on bodies which does not have body yet
/// This method is called late, so body creation is deffered as much as possible
void CqPhysicalBody::assureBodyCreated()
{
	Q_ASSERT( _pWorld );
	
	if ( ! _pBody )
	{
		createBody(_pWorld);
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

// ======================= set rotation [radians] ==========================
void CqPhysicalBody::setRotationRadians( double radians )
{
	if ( fabs( _rotation - radians ) > 0.01 )
	{
		prepareGeometryChange();
		_rotation = radians;
	}
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

// ============================== set physical pos ===============
/// Proper way to move the item. It alse updates all neccesary data
void CqPhysicalBody::setPhysicalPos( const QPointF& pos )
{
	CqItem::setPhysicalPos( pos );
	if ( _pBody )
	{
		_pBody->SetCenterPosition( b2Vec2( pos.x(), pos.y() ), _rotation );
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

// =============================== remove joint ======================
void CqPhysicalBody::setPhysicalRotation( double radians )
{
	CqItem::setPhysicalRotation( radians );
	
	if ( _pBody )
	{
		_pBody->SetCenterPosition( b2Vec2( pos().x(), pos().y() ), _rotation );
		update();
	}
}

// =========================== can be rotated =========================
bool CqPhysicalBody::canBeRotated() const
{
	// yes if no joints. ask editor otherwise
	
	if ( _joints.empty() )
	{
		return simulation()->canBeRotated( this );
	}
	
	return false;
}

// EOF

