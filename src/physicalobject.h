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
#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <QObject>
#include <QPolygonF>

class b2Body;
class b2BodyDef;
class b2ShapeDef;
class b2PolyDef;

#include "material.h"
class World;

/**
	Qt wrapper around Box2D's body.
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class PhysicalObject : public QObject
{
	Q_OBJECT
public:
	PhysicalObject( const QPolygonF& shape, World* pWorld, const Material& material );
	PhysicalObject( const PhysicalObject& t );
	~PhysicalObject();
	
	PhysicalObject& operator=( const PhysicalObject& t );

	// properties
	QPolygonF outline() const;
	b2Body* b2body() { return _pBody; }
	const b2Body* b2body() const { return _pBody; }
	
	const Material& material() const { return _material; }
	
	// operations
	void detach();		///< detaches from box2d body

private:

	/// Created b2d object
	void createBody( const QPointF& position );

	/// Creates shape for body based on polygon
	static QList<b2ShapeDef*> createShape( QPolygonF shape );
	/// Creates polygonal b2ShapeDef, based on three points
	static b2PolyDef* createTriangleB2Shape( const QPointF& a, const QPointF& b, const QPointF& c );
	/// Calculates cross product of two vectors
	static double product( const QPointF& a, const QPointF& b );
	
	
	void destroyBody();				///< Destroys associated body
	
	QPolygonF	_outline;			///< Object's visual shape
	b2Body*		_pBody;				///< box2d body reference
	World*		_pWorld;			///< Parent world
	Material	_material;			///< Material
	
};

#endif
