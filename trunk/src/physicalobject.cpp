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

#include <QMatrix>

#include "b2Body.h"

#include "world.h"

#include "physicalobject.h"
#include "polygontriangulator.h"

PhysicalObject::PhysicalObject( const QPolygonF& shape, World* pWorld
	, const Material& material )
 : QObject(pWorld)
{
	Q_ASSERT( pWorld );
	
	_pWorld = pWorld;
	_material = material;
	
	// TODO simplified - used boundoing box as box
	QRectF boxRect = shape.boundingRect();
	
	// find position
	double x = boxRect.x() + boxRect.width() / 2.0;
	double y = boxRect.y() + boxRect.height() / 2.0;
	
	// store shape
	_outline = shape;
	_outline.translate( -x, -y ); // make outline position-agnostic
	createBody( QPointF( x, y ) );
}

// =================== copy construtor ===========
void PhysicalObject::createBody( const QPointF& position )
{
	b2BodyDef bodyDef;
	QPolygonF outline = _outline;
	outline.translate( position ); // TEST
	QList< b2ShapeDef* > shapes = createShape( outline );
	
	qDebug("shape consists of %d triangles", shapes.size() ); // TODO remove
	// TODO make sure there is no more than 64 (use defined condstant) triangles
	
	// add shapes to body
	foreach( b2ShapeDef* pShape, shapes )
	{
		// body-wide material definition
		pShape->density		= _material.density;
		pShape->friction	= _material.friction;
		pShape->restitution	= _material.restitution;
	
		bodyDef.AddShape( pShape );
	}
	
	// attach pointer to self to the box2d body
	bodyDef.userData = this;
	
	// create body
	_pBody = _pWorld->b2world()->CreateBody(&bodyDef);
	
	// xorrect outline positoon to center of gravity
	b2Vec2 center = _pBody->GetCenterPosition();
	QPointF centerPoint( center.x, center.y );
	_outline.translate( position - centerPoint );
	
	// release shapes
	foreach( b2ShapeDef* pShape, shapes )
	{
		delete pShape;
	}
	
}

// =================== copy construtor ===========
PhysicalObject::PhysicalObject( const PhysicalObject& t )
	: QObject( t.parent() )
	, _outline( t._outline )
	, _pBody( t._pBody )
	, _material( t._material )
{
}

// ===================== assignment operator ============
PhysicalObject& PhysicalObject::operator=( const PhysicalObject& t )
{
	_outline = t._outline;
	_pBody = t._pBody;
}

// ===================== destructor ===========
PhysicalObject::~PhysicalObject()
{
	destroyBody();
}

// ===================== destroy body ===========
void PhysicalObject::destroyBody()
{
	/* TODO not, not here!
	if ( _pWorld && _pBody )
	{
		_pWorld->b2world()->DestroyBody( _pBody );
		_pBody = NULL;
	}
	*/
}

// ========================= return object outline in physical space ===
QPolygonF PhysicalObject::outline() const
{
	Q_ASSERT( _pBody );
	// get body pos
	b2Vec2 position		= _pBody->GetCenterPosition();
	float32 rotation	= _pBody->GetRotation();
	
	// transform shape
	// TODO suboptimal, use matrix rotation
	QMatrix transformation;
	transformation.translate( position.x, position.y );
	transformation.rotate( rotation * 180.0 / M_PI );
	
	// TODO remove
	/*
	qDebug("object moves to: %.2lf,%.2lf, rotates to: %.2lf"
		, (double)position.x, (double)position.y
		, (double)rotation );
	*/
	
	return transformation.map( _outline );
	
}
// ========================== create shape ==================
/// Creates list box2d shapes from polygon definiton
QList<b2ShapeDef*> PhysicalObject::createShape( QPolygonF shape )
{
	QList<b2ShapeDef*> list;
	// triangles below this product will be discarded as irrelevant to geometry
	const double MIN_PRODUCT	= 0.002;
	
	PolygonTriangulator triangulator;
	QList< QPolygonF > triangles = triangulator.triangulate( shape );
	
	foreach( QPolygonF triangle, triangles )
	{
		// calculate if triangle is clockwise or counterclockwise
		double p = product( triangle[0] - triangle[1], triangle[2] - triangle[1] );
		
		
		if ( p > MIN_PRODUCT )
		{
			list.append( createTriangleB2Shape( triangle[2], triangle[1], triangle[0] ) );
		}
		else if ( p < -MIN_PRODUCT )
		{
			list.append( createTriangleB2Shape( triangle[0], triangle[1], triangle[2] ) );
		}
		else
		{
			qDebug("irrevelant trianlge removed (p=%lf)", p );
		}
	}

	return list;
	
}

// ========================== create triangle ================
b2PolyDef* PhysicalObject::createTriangleB2Shape( const QPointF& a, const QPointF& b, const QPointF& c )
{
	b2PolyDef* pTriangle = new b2PolyDef();
	
	pTriangle->vertexCount = 3;
	pTriangle->vertices[0].Set( a.x(), a.y() );
	pTriangle->vertices[1].Set( b.x(), b.y() );
	pTriangle->vertices[2].Set( c.x(), c.y() );
	
	return pTriangle;
}

// ========================== product ================
/// Calculates Z coordinate of two 3D cross product of two 2D vectors
/// with artifical Z. 
/// In other words: (0 0 z) = (a1 a2 0) x (b1 b2 0)
/// This value is equal to |a||b|sin(o), where 0 i angle btween vectors
double PhysicalObject::product( const QPointF& a, const QPointF& b )
{
	return a.x()*b.y() - a.y()*b.x();
}

// ============================== detach ==================
void PhysicalObject::detach()
{
	_pBody = NULL;
}

