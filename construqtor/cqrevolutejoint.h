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
#ifndef CQREVOLUTEJOINT_H
#define CQREVOLUTEJOINT_H

#include "cqjoint.h"

/**
	Revolute joint object.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqRevoluteJoint : public CqJoint
{
public:
	
	// construction destruction
	
	CqRevoluteJoint(QGraphicsItem* parent, CqWorld* world = NULL);
	CqRevoluteJoint(CqWorld* world = NULL);
	virtual ~CqRevoluteJoint();
	
	// properties
	
	/// Sets anchor point (in local coords)
	void setAnchorPoint( const QPointF& point ) { _anchorPoint = point; }
	/// Returns anchor point (in local coords)
	QPointF anchorPoint() const { return _anchorPoint; }
	/// Enables and configures motor
	void setMotorEnabled( bool enabled, double speed, double torque );
	/// Enables and configures motor limits
	void setLimits( bool limits, double upper, double lower );
	
	bool motorEnabled() const { return _enableMotor; }
	
	double maxTorque() const { return _maxTorque; }
	double maxSpeed() const { return _maxSpeed; }

	// signal from simulation

	virtual void updatePosToPhysical();		///< Updates position and rotation to physical

protected:

	// methods
	void init();
	
    virtual b2Joint* createJoint( CqWorld* pWorld );	///< Creates joint
		
		
	QPointF	_anchorPoint;							///< Anchor point, in local coords
		
	// motor specfication
	bool	_enableMotor;							///< Flag - create joint with motor
	bool	_enableLimits;							///< Flag: apply limits to motor
	double	_upperLimit, _lowerLimit;
	
	double	_maxTorque;
	double	_maxSpeed;
	double	_initialSpeed;

};

#endif // CQREVOLUTEJOINT_H

// EOF


