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
#ifndef CQHYDRAULICCYLINDER_H
#define CQHYDRAULICCYLINDER_H

// local
#include "cqcompounditem.h"
#include "cqprismaticjoint.h"
#include "cqprismatictraslationcontroller.h"
class CqGirder;

/// \internal
class CqHydraulicCylinderMotor : public CqPrismaticJoint
{
	Q_OBJECT
public:
	explicit CqHydraulicCylinderMotor( CqItem* parent = NULL ) : CqPrismaticJoint( parent ) {}
	virtual ~CqHydraulicCylinderMotor(){};
	
	virtual void paint
		( QPainter * /*painter*/
		, const QStyleOptionGraphicsItem * /*option*/
		, QWidget * /*widget = 0*/ )
	{
		// no painting
	}
		
    virtual QRectF boundingRect() const
	{
    	return QRectF(); // no bounding rect
	}
};


/**
	Hydraulic cylinder - two long bodies connected together with prismatic joint,
	translating along longitudinal axis.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqHydraulicCylinder : public CqCompoundItem
{
	Q_OBJECT
	
	Q_PROPERTY( double length READ length WRITE setLength DESIGNABLE true );
	Q_PROPERTY( double diameter READ diameter WRITE setDiameter DESIGNABLE true );
	Q_PROPERTY( double maxForce READ maxForce WRITE setMaxForce DESIGNABLE true );
	Q_PROPERTY( double maxSpeed READ maxSpeed WRITE setMaxSpeed DESIGNABLE true );
public:

	// construction / destruction
	CqHydraulicCylinder( CqItem* pParent = NULL );
	virtual ~CqHydraulicCylinder();

	// CqItem
	
	/// Extends base implementation by adding controller to simulation
	virtual void setSimulation( CqSimulation* pSimulation );
	
	/// Returns sub-body after point, for conection purposes
	virtual CqPhysicalBody* bodyHere( const QPointF& worldPoint );
	virtual bool canConnectHere( const QPointF& worldPoint ) { return bodyHere( worldPoint ) != NULL; }
	
	// properties
	
	void setLength( double l );
	double length() const { return _length; }
	
	void setDiameter( double d );
	double diameter() const { return _diameter; }
	
	void setMaxForce( double f );
	double maxForce() const;
	
	void setMaxSpeed( double s );
	double maxSpeed() const;
	
	virtual QString description() const;
	
	// i/o
	virtual void load( const CqElement& element );
	virtual void store( CqElement& element ) const;

private:

	// methods
	
	void init();
	void setupGeometry();				///< set's up relative child geometry

	// data
	
	CqGirder*			_pBarrel;
	CqGirder*			_pPiston;
	CqHydraulicCylinderMotor*	_pMotor;
	
	double				_length;		///< cylinder length (when shortest. longest is 180% of this)
	double				_diameter;		///< Barrel diameter 
	
	CqPrismaticTraslationController _controller;	///< Motor controller
	
};
#endif // CQHYDRAULICCYLINDER_H

// EOF


