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
#ifndef CQPRISMATICJOINT_H
#define CQPRISMATICJOINT_H

// local
#include "cqjoint.h"

/**
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPrismaticJoint : public CqJoint
{
	Q_OBJECT
public:
	
	// construction / destruction
	CqPrismaticJoint( CqItem* parent = NULL );
	virtual ~CqPrismaticJoint();
	
	// joint
	virtual b2Joint* createJoint(CqWorld* pWorld);

	// properties
	
	/// Sets anchor point (in local coords)
	void setAnchorPoint( const QPointF& point ) { _anchorPoint = point; }
	/// Returns anchor point (in local coords)
	QPointF anchorPoint() const { return _anchorPoint; }
	/// Enables and configures motor
	void setMotorEnabled( bool enabled );
	
	bool motorEnabled() const { return _enableMotor; }
	
	/// Enables and configures motor limits
	void setLimitsEnabled( bool limitsEnabled );
	bool limitsEnabled() const { return _enableLimits; }
	
	void setUpperLimit( double limit );
	double upperLimit() const { return _upperLimit; }
	
	void setLowerLimit( double limit );
	double lowerLimit() const { return _lowerLimit; }
	
	void  setMaxForce( double f );
	double maxForce() const { return _maxForce; }
	
	void setMaxSpeed( double s );
	double maxSpeed() const { return _maxSpeed; }
	
	void setAxis( const QPointF& axis );
	QPointF axis() const { return _axis; }

	// signal from simulation

	virtual void updatePosToPhysical();					///< Updates position and translation to physical
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 

	// state
	double translation() const;	///< Current ternaslation

private:
	
	// methods
	
	void init();
	
	// data
	
	QPointF	_anchorPoint;							///< Anchor point, in local coords
	QPointF	_axis;									///< Joint's axis
		
	// motor specfication
	bool	_enableMotor;							///< Flag - create joint with motor
	bool	_enableLimits;							///< Flag: apply limits to motor
	double	_upperLimit, _lowerLimit;
	
	double	_maxForce;
	double	_maxSpeed;
	
	double	_initialSpeed;
	double	_initialTranslation;
};

#endif // CQPRISMATICJOINT_H

// EOF


