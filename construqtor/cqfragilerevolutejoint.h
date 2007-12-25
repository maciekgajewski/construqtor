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
#ifndef CQFRAGILEREVOLUTEJOINT_H
#define CQFRAGILEREVOLUTEJOINT_H

// local
#include "cqrevolutejoint.h"

/**
	This is revolute jpint that breaks under hevy load.
	It works this way: the joint has some force and torque which it tolerates w/o harm.
	When any of these values exceeds tolerance, the joint 'heats'. Heat is measured in 
	as value between 0.0 and 1.0. The higher force, the faster joint hetas. When heat reaches 1.0,
	joint is breaked, virtual method breaked() is called.
	Each simulation step joint cools down at soecified rate.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqFragileRevoluteJoint: public CqRevoluteJoint
{
	Q_OBJECT
public:

	// construction / destruction
	CqFragileRevoluteJoint(QGraphicsItem* parent, CqWorld* world = NULL);
	CqFragileRevoluteJoint(CqWorld* world = NULL);
	virtual ~CqFragileRevoluteJoint();
	
	// info from simulatiom
	virtual void simulationStep();						///< Called after simulation step
	
	// properties
	double temperature() const { return _temperature; }
	
	void setToleratedTorque( double t ) { _toleratedTorque = t; }
	double toleratedTorque() const { return _toleratedTorque; }
	
	void setToleratedForce( double f ) { _tolaretedForce = f; }
	double toleratedForce() const { return _tolaretedForce; }
	
	void setHeatingByTorque( double h ) { _heatingByTorque = h; }
	double heatingByTorque() const { return _heatingByTorque; }
	
	void setHeatingByForce( double h ) { _heatingByForce = h; }
	double heatingByForce() const { return _heatingByForce; }
	
	void setCooling( double c ) { _cooling = c; }
	double cooling() const { return _cooling; }
	
	// utilities
	
	static QColor colorByTemperature( double t );		///< Returns color beetween black and red, according to temp
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 


protected:

	virtual void broken() = 0;		//!< called when joint is breaked
	
private:

	// methods
	
	void init();
	
	// state
	double _temperature;			///< Current temperature (0-1)
	
	
	// params
	double _tolaretedForce;			///< Toleraded force value
	double _toleratedTorque;		///< Tolerated torque value
	
	double _heatingByTorque;		///< Heating by torque unit in one second
	double _heatingByForce;			///< Heating by torque unit in one second
	
	double _cooling;				///< Cooling in second.


};

#endif // CQFRAGILEREVOLUTEJOINT_H

// EOF


