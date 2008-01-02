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

// Box2D
#include "b2PrismaticJoint.h"

// local
#include "cqprysmaticjoint.h"
#include "cqphysicalbody.h"
#include "cqworld.h"

// XML tags
static const char* TAG_ANCHOR_POINT		= "anchor";
static const char* TAG_MOTOR_ENABLED	= "motor";
static const char* TAG_MAX_SPEED		= "maxspeed";
static const char* TAG_MAX_FORCE		= "maxforce";
static const char* TAG_LIMITS_ENABLED	= "limits";
static const char* TAG_UPPER_LIMIT		= "upperlimit";
static const char* TAG_LOWER_LIMIT		= "lowerlimit";
static const char* TAG_JOINT_SPEED		= "jointspeed";
static const char* TAG_JOINT_TRANSLATION	= "jointtranslation";

// ==========================================================================
CqPrysmaticJoint::CqPrysmaticJoint(QGraphicsItem* parent, CqWorld* world)
	: CqJoint(parent, world)
{
	init();
}

// ==========================================================================
CqPrysmaticJoint::CqPrysmaticJoint(CqWorld* world)
	: CqJoint( world)
{
	init();
}

// ==========================================================================
CqPrysmaticJoint::~CqPrysmaticJoint()
{
	// nope
}

// ==========================================================================
void CqPrysmaticJoint::init()
{
	_enableMotor		= false;
	_enableLimits		= false;
	_anchorPoint		= QPointF( 0, 0 );
	_initialSpeed		= 0.0;
	_initialTranslation	= 0.0;
}

// ==========================================================================
b2Joint* CqPrysmaticJoint::createJoint(CqWorld* pWorld)
{
	Q_ASSERT( pWorld );
	
	if ( body1() && body2() )
	{
		b2PrismaticJointDef jointDef;
		jointDef.body1 = body1()->b2body();
		jointDef.body2 = body2()->b2body();
		
		Q_ASSERT( jointDef.body1 );
		Q_ASSERT( jointDef.body2 );
		
		
		jointDef.collideConnected = false;
		
		QPointF a = mapToWorld( _anchorPoint );
		jointDef.anchorPoint = b2Vec2( a.x(), a.y() );
		
		// motor
		jointDef.enableMotor		= _enableMotor;
		jointDef.motorSpeed			= _initialSpeed;
		jointDef.motorForce			= _maxForce;
		jointDef.enableLimit		= _enableLimits;
		jointDef.upperTranslation	= _upperLimit;
		jointDef.lowerTranslation	= _lowerLimit;
		
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)pWorld->CreateJoint( & jointDef );
		
		// TODO what to do with initial translation?
		
		return pJoint;
	}
	
	qWarning("Tired to create prismatic joint without bodies");
	return NULL;
}
	
// ==========================================================================
void CqPrysmaticJoint::updatePosToPhysical()
{
	if ( b2joint() )
	{
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)b2joint();
		b2Vec2 pos = pJoint->GetAnchor1();
		setWorldPos( QPointF(pos.x, pos.y) - _anchorPoint );
	}
}

// ================================ set motor enabled ===========================
void CqPrysmaticJoint::setMotorEnabled( bool enabled, double speed, double force )
{
	_enableMotor	= enabled;
	_maxSpeed		= speed;
	_maxForce		= force; // TODO stil ot sure if it is max torque
}

// ================================ set limits enabled ===========================
void CqPrysmaticJoint::setLimits( bool limits, double upper, double lower )
{
	_enableLimits	= limits;
	_upperLimit		= upper;
	_lowerLimit		= lower;
}

// ==========================================================================
void CqPrysmaticJoint::store( CqElement& element ) const
{
	CqJoint::store( element );
	
	element.appendPointF( TAG_ANCHOR_POINT, _anchorPoint );
	
	// motor
	element.appendInt( TAG_MOTOR_ENABLED, _enableMotor );
	if ( _enableMotor )
	{
		element.appendDouble( TAG_MAX_SPEED, _maxSpeed );
		element.appendDouble( TAG_MAX_FORCE, _maxForce );
	}
	
	// limits
	element.appendInt( TAG_LIMITS_ENABLED, _enableLimits );
	if ( _enableLimits )
	{
		element.appendDouble( TAG_UPPER_LIMIT, _upperLimit );
		element.appendDouble( TAG_LOWER_LIMIT, _lowerLimit );
	}
	
	// store joint state
	b2PrismaticJoint* pJoint = (b2PrismaticJoint*)b2joint();
	if ( pJoint )
	{
		element.appendDouble( TAG_JOINT_SPEED, pJoint->GetJointSpeed() );
		element.appendDouble( TAG_JOINT_TRANSLATION, pJoint->GetJointTranslation() );
	}
	else
	{
		element.appendDouble( TAG_JOINT_SPEED, _initialSpeed );
		element.appendDouble( TAG_JOINT_TRANSLATION, _initialTranslation );
	}
}

// ==========================================================================
void CqPrysmaticJoint::load( const CqElement& element )
{
	CqJoint::load( element );

	_anchorPoint = element.readPointF( TAG_ANCHOR_POINT );
	
	// motor
	_enableMotor = bool( element.readInt( TAG_MOTOR_ENABLED ) );
	if ( _enableMotor )
	{
		_maxSpeed = element.readDouble( TAG_MAX_SPEED );
		_maxForce = element.readDouble( TAG_MAX_FORCE );
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
	_initialTranslation = element.readDouble( TAG_JOINT_TRANSLATION );
}

// EOF



