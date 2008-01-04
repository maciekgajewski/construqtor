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

// box 2d
#include "b2RevoluteJoint.h"

// local
#include "cqrevolutevelocitycontroler.h"
#include "cqsimulation.h"

// ============================ constructor ====================
CqRevoluteVelocityControler::CqRevoluteVelocityControler(QObject* parent)
	: CqMotorController(parent)
{
	init();
}

// ============================ constructor ====================
CqRevoluteVelocityControler::CqRevoluteVelocityControler( CqRevoluteJoint* pJoint, QObject* parent )
	: CqMotorController(parent)
{
	init();
	setJoint( pJoint );
}

// ============================ destructor ======================
CqRevoluteVelocityControler::~CqRevoluteVelocityControler()
{
	// nope
}

// ================================ init ========================
void CqRevoluteVelocityControler::init()
{
	_desiredVelocity = 0.0;
	_pJoint = NULL;
}

// ================================ get current force ===========
double CqRevoluteVelocityControler::getCurrentForce() const
{
	if ( _pJoint && _pJoint->b2joint() )
	{
		b2RevoluteJoint* pRj = (b2RevoluteJoint*)_pJoint->b2joint();
		double torque =  pRj->GetMotorTorque( _pJoint->simulation()->invTimeStep() );
		return torque;
	}

	return 0.0;
}

// ================================= get current value ===========
double CqRevoluteVelocityControler::getCurrentValue() const
{
	if ( _pJoint && _pJoint->b2joint() )
	{
		b2RevoluteJoint* pRj = (b2RevoluteJoint*)_pJoint->b2joint();
		return pRj->GetJointSpeed();
	}

	return 0.0;
}

// ================================= get desired value ===========
double CqRevoluteVelocityControler::getDesiredValue() const
{
	return _desiredVelocity;
}

// =============================== set desired value ==============
void CqRevoluteVelocityControler::setDesiredValue(double value)
{
	_desiredVelocity = value;
	
	if ( _pJoint && _pJoint->b2joint() )
	{
		if ( value >= _valueMin && value <= _valueMax )
		{
			b2RevoluteJoint* pRj = (b2RevoluteJoint*)_pJoint->b2joint();
			pRj->SetMotorSpeed( value );
		}
	}
}

// =============================== setjoint ==============
void CqRevoluteVelocityControler::setJoint( CqRevoluteJoint* pJoint )
{
	Q_ASSERT( pJoint );
	
	_forceMin = - pJoint->maxTorque();
	_forceMax = pJoint->maxTorque();
	_valueMin = - pJoint->maxSpeed();
	_valueMax = pJoint->maxSpeed();
	
	_pJoint = pJoint;
}

// EOF

