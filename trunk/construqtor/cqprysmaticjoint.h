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
#ifndef CQPRYSMATICJOINT_H
#define CQPRYSMATICJOINT_H

// local
#include "cqjoint.h"

/**
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPrysmaticJoint : public CqJoint
{
	Q_OBJECT
public:
	
	// construction / destruction
	CqPrysmaticJoint( QGraphicsItem* parent, CqWorld* world = NULL );
	CqPrysmaticJoint( CqWorld* world = NULL );
	virtual ~CqPrysmaticJoint();
	
	// joint
	virtual b2Joint* createJoint(CqWorld* pWorld);

	// properties
	
	/// Sets anchor point (in local coords)
	void setAnchorPoint( const QPointF& point ) { _anchorPoint = point; }
	/// Returns anchor point (in local coords)
	QPointF anchorPoint() const { return _anchorPoint; }
	/// Enables and configures motor
	void setMotorEnabled( bool enabled, double speed, double force );
	/// Enables and configures motor limits
	void setLimits( bool limits, double upper, double lower );
	
	bool motorEnabled() const { return _enableMotor; }
	
	double maxForce() const { return _maxForce; }
	double maxSpeed() const { return _maxSpeed; }

	// signal from simulation

	virtual void updatePosToPhysical();					///< Updates position and translation to physical
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 

private:
	
	// methods
	
	void init();
	
	// data
	
	QPointF	_anchorPoint;							///< Anchor point, in local coords
		
	// motor specfication
	bool	_enableMotor;							///< Flag - create joint with motor
	bool	_enableLimits;							///< Flag: apply limits to motor
	double	_upperLimit, _lowerLimit;
	
	double	_maxForce;
	double	_maxSpeed;
	
	// physica lproperties
	double	_initialSpeed;
	double	_initialTranslation;
};

#endif // CQPRYSMATICJOINT_H

// EOF


