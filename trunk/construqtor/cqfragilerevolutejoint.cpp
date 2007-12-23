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
#include "b2Joint.h"

// local
#include "cqsimulation.h"
#include "cqfragilerevolutejoint.h"

// ================================ constructor ==================
CqFragileRevoluteJoint::CqFragileRevoluteJoint( QGraphicsItem* parent, CqWorld* world )
	: CqRevoluteJoint( parent, world )
{
	init();
}

// ================================ constructor ==================
CqFragileRevoluteJoint::CqFragileRevoluteJoint( CqWorld* world )
	: CqRevoluteJoint( world )
{
	init();
}

// ================================ destructor ==================
CqFragileRevoluteJoint::~CqFragileRevoluteJoint()
{
	// nope
}

// ================================ init =======================
void CqFragileRevoluteJoint::init()
{
	// init state
	_temperature = 0.0;
	
	// set some defaults
	_cooling = 0.1;
	
	_tolaretedForce		= 3000.0;
	_toleratedTorque	= 3000.0;
	
	_heatingByTorque	= 0.0001;
	_heatingByForce		= 0.0001;

}

// ================================= simulation step ==========
void CqFragileRevoluteJoint::simulationStep()
{
	CqRevoluteJoint::simulationStep();
	
	Q_ASSERT( simulation() );
	
	double tempBefore = _temperature;
	
	// calculate force and torque
	double its		= simulation()->invTimeStep();
	double force	= b2joint()->GetReactionForce( its ).Length();
	double torque	= qAbs( b2joint()->GetReactionTorque( its ) );

	// heat
	if ( force > _tolaretedForce )
	{
		_temperature += ( force - _tolaretedForce ) * _heatingByForce / its;
		//qDebug("FORCE"); // TODO remove
	}
	if ( torque > _toleratedTorque )
	{
		_temperature += ( torque - _toleratedTorque ) * _heatingByTorque / its;
		//qDebug("TORQUE"); // TODO remove
	}
	
	// cool
	_temperature -= _cooling / its;
	
	// check limits
	if ( _temperature < 0.0 )
	{
		_temperature = 0.0;
	}
	
	else if ( _temperature >= 1.0 )
	{
		// update last time before possible deletion
		_temperature = 1.0;
		update();
		broken(); // NOTE propably deletes this (which is cool ;) )
		return;
	}

	// update if temp changes significantly
	if ( qAbs( _temperature - tempBefore ) > 0.01 )
	{
		update();
	}
}

// ========================== color bu temperature ======================
/// Returns color beetween black and red, according to temp. Temp shoudk be between 0 and 1
QColor CqFragileRevoluteJoint::colorByTemperature( double t )
{
	return QColor( qMin(255.0, 255.0 * t), 0, 0 );
}

// EOF

