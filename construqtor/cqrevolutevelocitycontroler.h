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
#ifndef CQREVOLUTEVELOCITYCONTROLER_H
#define CQREVOLUTEVELOCITYCONTROLER_H

// local
#include "cqmotorcontroller.h"
#include "cqrevolutejoint.h"

/**
	Revolute joint motor controller. Controls motor velocity
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqRevoluteVelocityControler : public CqMotorController
{
Q_OBJECT
public:
	CqRevoluteVelocityControler( QObject* parent = NULL );
	CqRevoluteVelocityControler( CqRevoluteJoint* pJoint, QObject* parent = NULL );
	virtual ~CqRevoluteVelocityControler();
	
	virtual double getCurrentForce() const;
	virtual double getCurrentValue() const;
	virtual double getDesiredValue() const;
	virtual void setDesiredValue(double value);
	
	void setJoint( CqRevoluteJoint* pJoint );
	CqRevoluteJoint* joint() const { return _pJoint; }

private:

	void init();

	CqRevoluteJoint*	_pJoint;
	double				_desiredVelocity;
};

#endif // CQREVOLUTEVELOCITYCONTROLER_H

// EOF


