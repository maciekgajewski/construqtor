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

// box 2d
#include "b2PrismaticJoint.h"

// lcoal
#include "cqprismaticjoint.h"
#include "cqprismatictraslationcontroller.h"
#include "cqsimulation.h"

// param
static const double	GAIN				= 5.0;	///< controller gain. error is scaled to trnalstion tange (0.0-1.0),
												///< resulting speed is then scaled back from 0.0->1.0 to 0.0->maxSpeed
static const double	PRECISION			= 0.01;	///< Desired precision

// ====================================================================
CqPrismaticTraslationController::CqPrismaticTraslationController(QObject* parent)
	: CqMotorController(parent)
{
	// init
}

// ====================================================================
CqPrismaticTraslationController::~CqPrismaticTraslationController()
{
	// nope
}

// ====================================================================
void CqPrismaticTraslationController::init()
{
	_desiredTranslation = 0.0;
	_pJoint = NULL;
}

// ====================================================================
double CqPrismaticTraslationController::getCurrentForce() const
{
	if ( _pJoint && _pJoint->b2joint() )
	{
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)_pJoint->b2joint();
		double force =  pJoint->GetMotorForce( _pJoint->simulation()->invTimeStep() );
		return force;
	}

	return 0.0;
}

// ====================================================================
double CqPrismaticTraslationController::getCurrentValue() const
{
	if ( _pJoint )
	{
		return _pJoint->translation();
	}

	return 0.0;
}

// ====================================================================
double CqPrismaticTraslationController::getDesiredValue() const
{
	return _desiredTranslation;
}

// ====================================================================
void CqPrismaticTraslationController::setDesiredValue( double value )
{
	_desiredTranslation = value;
}

// =============================== setjoint ===========================
void CqPrismaticTraslationController::setJoint( CqPrismaticJoint* pJoint )
{
	Q_ASSERT( pJoint );
	
	_forceMin = - pJoint->maxForce();
	_forceMax = pJoint->maxForce();
	_valueMin = pJoint->lowerLimit();
	_valueMax = pJoint->upperLimit();
	
	// also -set intial desired value at currenrt
	_desiredTranslation = pJoint->translation();
	
	_pJoint = pJoint;
}

// ================================= simulation step ====================
void CqPrismaticTraslationController::calculationStep()
{
	if ( _pJoint )
	{
		b2PrismaticJoint* pJoint = (b2PrismaticJoint*)_pJoint->b2joint();
		if ( pJoint )
		{
			double currentTranslation = _pJoint->translation();
			double range  = qAbs( _valueMax - _valueMin );
			
			// within precision
			if ( qAbs( currentTranslation - _desiredTranslation ) < PRECISION * range )
			{
				pJoint->SetMotorSpeed( 0 );
				// let bodies snooze
			}
			else 
			{
				double error = ( currentTranslation - _desiredTranslation ) / range; // error: 0-1
				double speed = -GAIN * error;
				if ( speed > 1.0 ) speed = 1.0;
				
				pJoint->SetMotorSpeed( _pJoint->maxSpeed() * speed );
				_pJoint->wakeUpBodies();
			}
			
			// TODO remove
			qDebug("desired: %lf, current: %lf", _desiredTranslation, currentTranslation ); // TODO remove
		}
	}
}
