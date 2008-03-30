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
#ifndef CQMOTORCONTROLLER_H
#define CQMOTORCONTROLLER_H

// Qt
#include <QObject>

/**
	Common interface for al motor controlls. Contains generic methods used by 
	GUI to query and set motor controller state.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqMotorController : public QObject
{
Q_OBJECT
public:
	CqMotorController(QObject* parent = NULL);
	virtual ~CqMotorController();
	
	// controlled value
	virtual void setDesiredValue( double value ) = 0;
	virtual double getDesiredValue() const = 0;
	virtual double getCurrentValue() const = 0;
	double getMaxValue() const { return _valueMax; }
	double getMinValue() const { return _valueMin; }
	
	// force / torque
	double getMaxForce() const { return _forceMax; }
	double getMinForce() const { return _forceMin; }
	virtual double getCurrentForce() const = 0;
	
	
protected:

	// static parameters, set by derrived classe
	
	double _forceMax, _forceMin;			///< force limits
	double _valueMax, _valueMin;			///< Value limits

};

#endif // CQMOTORCONTROLLER_H

// EOF


