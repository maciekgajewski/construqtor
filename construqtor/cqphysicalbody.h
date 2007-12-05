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
#ifndef CQPHYSICALBODY_H
#define CQPHYSICALBODY_H

// Qt
#include <QGraphicsItem>
#include <QPointer>

// box2d
#include "b2Shape.h"
class b2Body;

// local
#include "cqworld.h"

/**
	This is a Graphics Scene item which has is physical alter-ego
	as Box2D body.
	
	Body is created and re-created when needed. You can force body re-createion
	(i.e. when shape changes) by calling recreateBody().
	
	The object is responsible for defining it's own shape nad creaint/destroying
	b2Body when neccesary. 
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPhysicalBody : public QGraphicsItem
{
public:
	
	// constrution / destruction
	CqPhysicalBody( CqWorld* world = NULL );
	explicit CqPhysicalBody( QGraphicsItem* parent, CqWorld* world = NULL );
	virtual ~CqPhysicalBody();

	// properties
	void setWorld ( CqWorld* pWorld );		///< Sets world
	void setRotationRadians( double radians );	///< sets rotation in radians
	double rotationRadians() const { return _rotation; } ///< Retuens rotation

	// signals from simulation
	
	void simulationStep();					///< Called after simulation step
	void assureBodyCreated();				///< Makes sure that body was created
	
protected:
	
	// methodd
	
	void recreateBody();					///< Re-creates body
	
	// reimplementables
	
	/// Defines body shapes. Called each time body is created
	/// Shape defiitions should be created with \b new, and will be destroyed with \b delete
	virtual QList<b2ShapeDef*> createShape() = 0;
	
	

private:

	// methods
	
	void init();							///< Initializes instance
	void createBody( CqWorld* pWorld );		///< Creates body
	void destroyBody( CqWorld* pWorld );	///< Destroys body
	

	// data

	QPointer< CqWorld > _pWorld;		///< World object
	b2Body*	_pBody;						///< Body itself
	double	_rotation;					///< Body's rotation [radians]
};

#endif
