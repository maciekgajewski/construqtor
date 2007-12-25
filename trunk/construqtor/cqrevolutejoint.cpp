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

// Box 2d
#include "b2RevoluteJoint.h"

// local
#include "cqrevolutejoint.h"
#include "cqphysicalbody.h"

// XML tags
static const char* TAG_ANCHOR_POINT		= "anchor";
static const char* TAG_MOTOR_ENABLED	= "motor";
static const char* TAG_MAX_SPEED		= "maxspeed";
static const char* TAG_MAX_TORQUE		= "maxtorque";
static const char* TAG_LIMITS_ENABLED	= "limits";
static const char* TAG_UPPER_LIMIT		= "upperlimit";
static const char* TAG_LOWER_LIMIT		= "lowerlimit";
static const char* TAG_JOINT_SPEED		= "jointspeed";
static const char* TAG_JOINT_ANGLE		= "jointangle";


// =========================== constructor ===================
CqRevoluteJoint::CqRevoluteJoint(QGraphicsItem* parent, CqWorld* world): CqJoint(parent, world)
{
	init();
}


// =========================== constructor ===================
CqRevoluteJoint::CqRevoluteJoint(CqWorld* world): CqJoint(world)
{
	init();
}


// =========================== destructor ===================
CqRevoluteJoint::~CqRevoluteJoint()
{
	// nothing
}

// ============================= init ======================
void CqRevoluteJoint::init()
{
	_enableMotor	= false;
	_enableLimits	= false;
	_anchorPoint	= QPointF( 0, 0 );
	_initialSpeed	= 0.0;
	_initialAngle	= 0.0;
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
		
		Q_ASSERT( jointDef.body1 );
		Q_ASSERT( jointDef.body2 );
		
		
		jointDef.collideConnected = false;
		
		QPointF a = mapToWorld( _anchorPoint );
		jointDef.anchorPoint = b2Vec2( a.x(), a.y() );
		
		// motor
		jointDef.enableMotor	= _enableMotor;
		jointDef.motorSpeed		= _initialSpeed;
		jointDef.motorTorque	= _maxTorque;
		jointDef.enableLimit	= _enableLimits;
		jointDef.upperAngle		= _upperLimit;
		jointDef.lowerAngle		= _lowerLimit;
		
		b2RevoluteJoint* pJoint = (b2RevoluteJoint*)pWorld->CreateJoint( & jointDef );
		
		// TODO what to do with initial angle?
		
		return pJoint;
	}
	
	qWarning("Tired to create revolute joint without bodies");
	return NULL;
}

// ======================== update pos to physical ================
void CqRevoluteJoint::updatePosToPhysical()
{
	if ( b2joint() )
	{
		b2RevoluteJoint* pJoint = (b2RevoluteJoint*)b2joint();
		b2Vec2 pos = pJoint->GetAnchor1();
		setWorldPos( QPointF(pos.x, pos.y) - _anchorPoint );
	}
}

// ================================ set motor enabled ===========================
void CqRevoluteJoint::setMotorEnabled( bool enabled, double speed, double torque )
{
	_enableMotor	= enabled;
	_maxSpeed		= speed;
	_maxTorque		= torque; // TODO stil ot sure if it is max torque
}

// ================================ set limits enabled ===========================
void CqRevoluteJoint::setLimits( bool limits, double upper, double lower )
{
	_enableLimits	= limits;
	_upperLimit		= upper;
	_lowerLimit		= lower;
}

// ================================= store ========================
void CqRevoluteJoint::store( CqElement& element ) const
{
	CqJoint::store( element );
	
	element.appendPointF( TAG_ANCHOR_POINT, _anchorPoint );
	
	// motor
	element.appendInt( TAG_MOTOR_ENABLED, _enableMotor );
	if ( _enableMotor )
	{
		element.appendDouble( TAG_MAX_SPEED, _maxSpeed );
		element.appendDouble( TAG_MAX_TORQUE, _maxTorque );
	}
	
	// limits
	element.appendInt( TAG_LIMITS_ENABLED, _enableLimits );
	if ( _enableLimits )
	{
		element.appendDouble( TAG_UPPER_LIMIT, _upperLimit );
		element.appendDouble( TAG_LOWER_LIMIT, _lowerLimit );
	}
	
	// store joint state
	b2RevoluteJoint* pJoint = (b2RevoluteJoint*)b2joint();
	if ( pJoint )
	{
		element.appendDouble( TAG_JOINT_SPEED, pJoint->GetJointSpeed() );
		element.appendDouble( TAG_JOINT_ANGLE, pJoint->GetJointAngle() );
	}
	else
	{
		element.appendDouble( TAG_JOINT_SPEED, _initialSpeed );
		element.appendDouble( TAG_JOINT_ANGLE, _initialAngle );
	}
}

// ================================= load ========================
void CqRevoluteJoint::load( const CqElement& element )
{
	CqItem::load( element );

	_anchorPoint = element.readPointF( TAG_ANCHOR_POINT );
	
	// motor
	_enableMotor = bool( element.readInt( TAG_MOTOR_ENABLED ) );
	if ( _enableMotor )
	{
		_maxSpeed = element.readDouble( TAG_MAX_SPEED );
		_maxTorque = element.readDouble( TAG_MAX_TORQUE );
	}
	
	// limits
	_enableLimits = bool( element.readInt( TAG_LIMITS_ENABLED ) );
	if ( _enableLimits )
	{
		_upperLimit = element.readDouble( TAG_UPPER_LIMIT );
		_lowerLimit = element.readDouble( TAG_LOWER_LIMIT );
	}
	
	// physical properties
	// NOTE: assuming joint isn't created yet
	_initialSpeed = element.readDouble( TAG_JOINT_SPEED );
	_initialAngle = element.readDouble( TAG_JOINT_ANGLE );
}

// EOF

