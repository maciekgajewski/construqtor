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

static const double DEFAULT_MAX_SPEED	= 0.5;		// [m/s] // TODO i'm not sure if it is m/s. look like dm/s
static const double DEFAULT_MAX_FORCE	= 10000.0;	// [N]

// NOTE s about these values:
// - speed 0.5 is fast
// - force of 1000N is small, barely enough to lift 2m girder

// tags
static const char* TAG_MOTOR	= "motor";
static const char* TAG_BARREL	= "barrel";
static const char* TAG_PISTON	= "piston";

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
	_pBarrel->setMaterial( CqMaterial::steel() );
	_pBarrel->setCollisionGroup( CollisionCylinder );
	
	// init piston
	_pPiston->setBrush( Qt::lightGray );
	_pPiston->setMaterial( CqMaterial::steel() );
	_pPiston->setZValue( 1.0999 ); // so hopefully nothing ever gets between barrel and piston
	_pPiston->setCollisionGroup( CollisionCylinder );
	
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
	connect( pSimulation, SIGNAL(calculationStep()), &_controller, SLOT(calculationStep()) );
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
	
	return _pMotor->maxSpeed();
}

// ===========================================================================
CqPhysicalBody* CqHydraulicCylinder::bodyHere( const QPointF& worldPoint )
{
	Q_ASSERT( _pBarrel && _pPiston );
	// check - if this is rightmost 10% of barrel
	QPointF barrelPoint = _pBarrel->mapFromWorld( worldPoint );
	QPointF pistonPoint = _pPiston->mapFromWorld( worldPoint );
	
	// barrel if barrel
	if ( _pBarrel->contains( barrelPoint ) )
	{
		return _pBarrel;
	}
	
	// piston otherwise
	if ( _pPiston->contains( pistonPoint ) )
	{
		return _pPiston;
	}
	
	return NULL;
}

// ===========================================================================
void CqHydraulicCylinder::load( const CqElement& element )
{
	delete _pMotor;
	delete _pBarrel;
	delete _pPiston;
	
	CqCompoundItem::load( element );
	
	_pMotor		= dynamic_cast< CqHydraulicCylinderMotor* >( element.readItemPointer( TAG_MOTOR ) );
	_pBarrel	= dynamic_cast< CqGirder* >( element.readItemPointer( TAG_BARREL ) );
	_pPiston	= dynamic_cast< CqGirder* >( element.readItemPointer( TAG_PISTON ) );
	
	// test
	if ( ! _pBarrel || ! _pPiston || ! _pMotor )
	{
		qFatal("cylinder elements not created properly");
	}
	
	_controller.setJoint( _pMotor );
	
	_pBarrel->setBrush( Qt::darkGray );
	_pPiston->setBrush( Qt::lightGray );
	
}

// ===========================================================================
void CqHydraulicCylinder::store( CqElement& element ) const
{
	CqCompoundItem::store( element );
	 
	 element.appendItemPointer( TAG_MOTOR, _pMotor );
	 element.appendItemPointer( TAG_BARREL, _pBarrel );
	 element.appendItemPointer( TAG_PISTON, _pPiston );
}

// ===============================================================
QString CqHydraulicCylinder::description() const
{
	QString d = QString("Cylinder, %2 %3cm, stroke %4cm, force: %5N, speed %6m/s")
		.arg( QChar( 0x00F8 ) )	// phui
		.arg( 100*diameter() )	// barrel diameter
		.arg( _length * 80 )	// stroke
		.arg( maxForce() )		// max force
		.arg( maxSpeed() )		// max speed
		;
	return d;
}

// EOF


