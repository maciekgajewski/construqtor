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
#include <QPointer>
#include <QPen>
#include <QBrush>

// box2d
#include "b2Shape.h"
class b2Body;

// local
#include "cqitem.h"
#include "cqworld.h"
#include "cqmaterial.h"
class CqJoint;

/**
	This is a Graphics Scene item which has is physical alter-ego
	as Box2D body.
	
	Body is created and re-created when needed. You can force body re-createion
	(i.e. when shape changes) by calling recreateBody().
	
	The object is responsible for defining it's own shape nad creaint/destroying
	b2Body when neccesary. 
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPhysicalBody : public CqItem
{
public:
	
	// constrution / destruction
	CqPhysicalBody( CqWorld* world = NULL );
	explicit CqPhysicalBody( QGraphicsItem* parent, CqWorld* world = NULL );
	virtual ~CqPhysicalBody();

	// properties
	CqMaterial material() { return _material; }
	void setMaterial( const CqMaterial& material ) {  _material = material; }
	
	void setPen( const QPen& pen ) { _pen = pen; }
	QPen pen() const { return _pen; }
	
	void setBrush( const QBrush& brush ) { _brush = brush; }
	QBrush brush() const { return _brush; }

	b2Body* b2body() { return _pBody; }
	const b2Body* b2body() const { return _pBody; }

	virtual int type() const;	///< RTTI
	
	// operations
	void breakAllJoints();								///< Destroys all joints attached
	
	// signals from simulation
	
	void simulationStep();					///< Called after simulation step
	void assureBodyCreated();				///< Makes sure that body was created
	virtual void updatePhysicalPos();		///< Updates body pos to item positon/rotation
	
	// signals from joint
	void addJoint( CqJoint* pJoint );		///< Info: you have new joint
	void removeJoint( CqJoint* pJoint );	///< Info: joint was removed
	
	
protected:
	
	// methodd
	
	void recreateBody();					///< Re-creates body
	
	// reimplementables
	
	/// Defines body shapes. Called each time body is created
	/// Shape defiitions should be created with \b new, and will be destroyed with \b delete
	virtual QList<b2ShapeDef*> createShape() = 0;
	
	// data
	
	QList<CqJoint*>	_joints;		///< Joints within this body

private:

	// methods
	
	void init();							///< Initializes instance
	void createBody( CqWorld* pWorld );		///< Creates body
	void destroyBody( CqWorld* pWorld );	///< Destroys body
	

	// data

	QPointer< CqWorld > _pWorld;		///< World object
	b2Body*	_pBody;						///< Body itself
	CqMaterial	_material;				///< Material used
	
	QBrush		_brush;					///< Brush used to paint item
	QPen		_pen;					///< Pen used to paint ite,
};

#endif
