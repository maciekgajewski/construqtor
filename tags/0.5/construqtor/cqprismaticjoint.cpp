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
#include "cqprismaticjoint.h"
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
static const char* TAG_AXIS				= "axis";

// ==========================================================================
CqPrismaticJoint::CqPrismaticJoint( CqItem* parent )
	: CqJoint( parent )
{
	init();
}

// ==========================================================================
CqPrismaticJoint::~CqPrismaticJoint()
{
	// nope
}

// ==========================================================================
void CqPrismaticJoint::init()
{
	_enableMotor		= false;
	_enableLimits		= false;
	_anchorPoint		= QPointF( 0, 0 );
	_initialSpeed		= 0.0;
	_initialTranslation	= 0.0;
	
	_lowerLimit			= 0.0;
	_upperLimit			= 1.0;
	
	_maxSpeed			= 1.0;
	_maxForce			= 1000.0;
}

// ==========================================================================
b2Joint* CqPrismaticJoint::createJoint(CqWorld* pWorld)
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
		
		// calculate axis in world coordinates
		QTransform t;
		t.rotateRadians( worldRotation() );
		QPointF worldAxis = t.map( _axis );
		qDebug("world rotation: %lf", worldRotation() ); // TODO remove
		
		// motor
		jointDef.enableMotor		= _enableMotor;
		jointDef.motorSpeed			= _maxSpeed;
		jointDef.motorForce			= _maxForce;
		jointDef.enableLimit		= _enableLimits;
		// NOTE: as there is no generic way to specifi intial trnaslation, i'll shift limits by intial translation
		jointDef.upperTranslation	= _upperLimit - _initialTranslation;
		jointDef.lowerTranslation	= _lowerLimit - _initialTranslation;
		jointDef.axis.Set( worldAxis.x(), worldAxis.y() );
		
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)pWorld->CreateJoint( & jointDef );
		
		// apply initial trnaslation
		// TODO no! the cake is a lie!
		
		return pJoint;
	}
	
	qWarning("Tired to create prismatic joint without bodies");
	return NULL;
}
	
// ==========================================================================
void CqPrismaticJoint::updatePosToPhysical()
{
	if ( b2joint() )
	{
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)b2joint();
		b2Vec2 pos = pJoint->GetAnchor1();
		setWorldPos( QPointF(pos.x, pos.y) - _anchorPoint );
	}
}

// ================================ set motor enabled ===========================
void CqPrismaticJoint::setMotorEnabled( bool enabled )
{
	_enableMotor	= enabled;
	recreateJoint();
}

// ================================ set limits enabled ===========================
void CqPrismaticJoint::setLimitsEnabled( bool limits  )
{
	_enableLimits	= limits;
	recreateJoint();
}

// ==========================================================================
void CqPrismaticJoint::store( CqElement& element ) const
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
	
	// axis
	element.appendPointF( TAG_AXIS, _axis );
	
	// store joint state
	b2PrismaticJoint* pJoint = (b2PrismaticJoint*)b2joint();
	if ( pJoint )
	{
		element.appendDouble( TAG_JOINT_SPEED, pJoint->GetJointSpeed() );
		element.appendDouble( TAG_JOINT_TRANSLATION, translation() );
	}
	else
	{
		element.appendDouble( TAG_JOINT_SPEED, _initialSpeed );
		element.appendDouble( TAG_JOINT_TRANSLATION, _initialTranslation );
	}
}

// ==========================================================================
void CqPrismaticJoint::load( const CqElement& element )
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
	
	// axis
	_axis = element.readPointF( TAG_AXIS );
	
	// physical properties
	// NOTE: assuming joint isn't created yet
	_initialSpeed = element.readDouble( TAG_JOINT_SPEED );
	_initialTranslation = element.readDouble( TAG_JOINT_TRANSLATION );
}

// =====================================================================
void CqPrismaticJoint::setMaxForce( double f )
{
	_maxForce = f;
	recreateJoint();
}

// =====================================================================
void CqPrismaticJoint::setMaxSpeed( double s )
{
	_maxSpeed = s;
	recreateJoint();
}

// =====================================================================
void CqPrismaticJoint::setUpperLimit( double limit )
{
	_upperLimit = limit;
	recreateJoint();
}

// =====================================================================
void CqPrismaticJoint::setLowerLimit( double limit )
{
	_lowerLimit = limit;
	recreateJoint();
}

// =====================================================================
void CqPrismaticJoint::setAxis( const QPointF& axis )
{
	_axis = axis;
	recreateJoint();
}

// =====================================================================
double CqPrismaticJoint::translation() const
{
	b2PrismaticJoint* pJoint = (b2PrismaticJoint*)b2joint(); // dynamic_cast?
	
	if ( pJoint )
	{
		return pJoint->GetJointTranslation() + _initialTranslation;
	}
	
	return _initialTranslation;
}

// EOF



