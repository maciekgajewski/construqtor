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

// Box 2d
#include "b2RevoluteJoint.h"

// local
#include "cqrevolutejoint.h"
#include "cqphysicalbody.h"

// =========================== constructor ===================
CqRevoluteJoint::CqRevoluteJoint(QGraphicsItem* parent, CqWorld* world): CqJoint(parent, world)
{
	// nothing
}


// =========================== constructor ===================
CqRevoluteJoint::CqRevoluteJoint(CqWorld* world): CqJoint(world)
{
	// nothing
}


// =========================== destructor ===================
CqRevoluteJoint::~CqRevoluteJoint()
{
	// nothing
}


// =========================== create joint ===================
b2Joint* CqRevoluteJoint::createJoint(CqWorld* pWorld)
{
	Q_ASSERT( pWorld );
	
	if ( body1() && body2() )
	{
		b2RevoluteJointDef jointDef;
		jointDef.body1 = body1()->b2body();
		jointDef.body2 = body2()->b2body();
		
		jointDef.collideConnected = false;
		
		QPointF a = anchorPoint();
		jointDef.anchorPoint = b2Vec2( a.x(), a.y() );
		
		return pWorld->CreateJoint( & jointDef );
	}
	
	qWarning("Tired to create revolute joint without bodies");
}

// ===========================set anchor ===================
void CqRevoluteJoint::setAnchorPoint( const QPointF& point )
{
	setPos( point );
}

// =========================== get anchor ==================
QPointF CqRevoluteJoint::anchorPoint() const
{
	return pos();
}

// =========================== simulation step ==================
void CqRevoluteJoint::simulationStep()
{
	if ( b2joint() )
	{
		b2RevoluteJoint* pJoint = (b2RevoluteJoint*)b2joint();
		
		b2Vec2 pos = pJoint->GetAnchor1();
		
		setPos( pos.x, pos.y );
		//qDebug("pos: %lf, %lf", double( pos.x ), double( pos.y ) ); // TODO remove
	}
}

// EOF

