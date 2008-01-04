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
#include "cqgirder.h"
#include "cqhydrauliccylinder.h"
#include "cqitemfactory.h"
#include "cqsimulation.h"

CQ_ADD_TO_FACTORY( CqHydraulicCylinderMotor );
CQ_ADD_TO_FACTORY( CqHydraulicCylinder );

static const double DEFAULT_MAX_SPEED	= 5.0;		// [m/s] // TODO i'm not sure if it is m/s. look like dm/s
static const double DEFAULT_MAX_FORCE	= 1000.0;	// [N]

// ===========================================================================
CqHydraulicCylinder::CqHydraulicCylinder( CqItem* pParent)
	: CqCompoundItem(pParent)
{
	init();
}

// ===========================================================================
CqHydraulicCylinder::~CqHydraulicCylinder()
{
	// nope
}

// ===========================================================================
void CqHydraulicCylinder::init()
{
	_length		= 1.0;
	_diameter	= 0.1;

	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );

	_pBarrel	= new CqGirder( this );
	_pPiston	= new CqGirder( this );
	_pMotor		= new CqHydraulicCylinderMotor( this );
	
	// init barrel
	_pBarrel->setBrush( Qt::darkGray );
	_pBarrel->setZValue( 1.1 );
	
	// init piston
	_pPiston->setBrush( Qt::lightGray );
	_pBarrel->setZValue( 1.999 ); // so hopefully nothing ever gets between barrel and piston
	
	// init motor
	_pMotor->setConnectedBodies( _pBarrel, _pPiston );
	_pMotor->setMotorEnabled( true );
	_pMotor->setMaxForce( DEFAULT_MAX_FORCE );
	_pMotor->setMaxSpeed( DEFAULT_MAX_SPEED );
	_pMotor->setLimitsEnabled( true );
	_pMotor->setLowerLimit( 0.0 ); // always
	_pMotor->setAxis( QPointF( 1.0, 0.0 ) );
	// upper limit depends on geometry
	
	
	setFollowedChild( _pBarrel );
	setupGeometry();
}

// ===========================================================================
void CqHydraulicCylinder::setSimulation( CqSimulation* pSimulation )
{
	Q_ASSERT( pSimulation );
	CqCompoundItem::setSimulation( pSimulation );
	
	pSimulation->addController( &_controller );
	connect( pSimulation, SIGNAL(simulationStep()), &_controller, SLOT(simulationStep()) );
}

// ===========================================================================
void CqHydraulicCylinder::setupGeometry()
{
	Q_ASSERT( _pPiston && _pBarrel && _pMotor );
	
	// TODO let's just assume for now, taht the motor is at 0
	
	// first - barrel
	_pBarrel->setSize( QSizeF( _length * 0.9, _diameter ) );
	_pBarrel->setPhysicalPos( QPointF( 0.0, 0.0 ) );
	_pBarrel->setPhysicalRotation( 0.0 );
	
	// then - piston
	_pPiston->setSize( QSizeF( _length * 0.9, _diameter * 0.9 ) ); // same length as barrel, but bit thinner
	_pPiston->setPhysicalPos( QPointF( 0.1 * _length,  0.0 ) );
	_pPiston->setPhysicalRotation( 0.0 ); 
	
	// finally - motor
	_pMotor->setUpperLimit( 0.8 * _length );
	_pMotor->setAnchorPoint( QPointF( 0.1 * _length,  0.0 ) );
	
	// controller
	_controller.setJoint( _pMotor );
	
	// voila!
}

// ===========================================================================
void CqHydraulicCylinder::setLength( double l )
{
	_length = l;
	setupGeometry();
}

// ===========================================================================
void CqHydraulicCylinder::setDiameter( double d )
{
	_diameter = d;
	setupGeometry();
}

// ===========================================================================
void CqHydraulicCylinder::setMaxForce( double f )
{
	Q_ASSERT( _pMotor );
	
	_pMotor->setMaxForce( f );
}

// ===========================================================================
double CqHydraulicCylinder::maxForce() const
{
	Q_ASSERT( _pMotor );
	
	return _pMotor->maxForce();
}

// ===========================================================================
void CqHydraulicCylinder::setMaxSpeed( double s )
{
	Q_ASSERT( _pMotor );
	
	_pMotor->setMaxSpeed( s );
}

// ===========================================================================
double CqHydraulicCylinder::maxSpeed() const
{
	Q_ASSERT( _pMotor );
	
	_pMotor->maxSpeed();
}


// EOF


