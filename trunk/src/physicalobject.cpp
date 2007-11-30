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

PhysicalObject::PhysicalObject( const QPolygonF& shape, World* pWorld )
 : QObject(pWorld)
{
	Q_ASSERT( pWorld );
	
	_pWorld = pWorld;
	
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
	bodyDef.position.Set( position.x(), position.y() );
	QList< b2ShapeDef* > shapes = createShape( _outline );
	
	qDebug("shape consists of %d triangles", shapes.size() ); // TODO remove
	
	// add shapes to body
	foreach( b2ShapeDef* pShape, shapes )
	{
		// body-wide material definition
		pShape->density = 1.0f;
		pShape->friction = 0.3f;
	
		bodyDef.AddShape( pShape );
	}
	// create body
	_pBody = _pWorld->b2world()->CreateBody(&bodyDef);
	
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
}

// ========================= return object outline in physical space ===
QPolygonF PhysicalObject::outline() const
{
	Q_ASSERT( _pBody );
	// get body pos
	b2Vec2 position = _pBody->GetCenterPosition();
	float32 rotation = _pBody->GetRotation();
	
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
	const double MIN_PRODUCT	= 0.01;
	
#if 0 // depreciated varsion. now using gpc
	// triangulate
	
	
	// step zero - create box based on degenrated (2-vertec) polygons
	if ( shape.size() <= 2 )
	{
		// TODO 
		qWarning("degenrated polygon ignored");
		return list;
	}
	
	// step one - calculate shape direction (clockwise, counterclockwise)
	double 	direction = 0.0; // sum product for all pairs of adjacent vectors (all vertices)
	int 	numVertices	= shape.size();
	qDebug("vertices: %d", numVertices );
	for ( int i =0; i < numVertices; i++ )
	{
		QPointF a = shape[ ( i + numVertices - 1 ) % numVertices ];
		QPointF b = shape[i];
		QPointF c = shape[ (i + 1) % numVertices ];
		
		double p = product( a-b, c-b );;
		direction += p;
	}
	qDebug("direction is: %lf", direction); // TODO remove
	// step 2 - found and cut off ears
	for( int v = 0; shape.size() > 3;  )
	{
		// get current and adjacent vertex
		QPointF a = shape[ ( v + shape.size() - 1 ) % shape.size() ];
		QPointF b = shape[ v ];
		QPointF c = shape[ (v + 1) % shape.size() ];
		
		// find out if it is convex one
		double p = product( a-b, c-b );
		
		// remove non-relvant triangle
		if ( fabs(p) < MIN_PRODUCT )
		{
			shape.remove( v );
			qDebug("irrevelant trianlge removed (p=%.lf)", p);
			continue;
		}
		
		// ignore not-convex vertex
		if ( (p * direction) <= 0.0 )
		{
			qDebug("skipping non-convex vertex %d (p=%lf)", v, p );
			v = ( v + 1 ) % shape.size();
			continue; 
		}
		
		
		// now - make sure that no other point lies within the triangle
		QPolygonF candidateTriangle;
		candidateTriangle.append( a );
		candidateTriangle.append( b );
		candidateTriangle.append( c );
		
		// qery all points except a,b and c
		qDebug("candidate vertex: %d", v );
		for ( int i=0; i < (shape.size()-3); i++ )
		{
			QPointF point = shape[ ( v + 2 + i ) % shape.size() ];
			qDebug(">>> checking point %d...", ( v + 2 + i ) % shape.size() ); // TODO remove
			if ( candidateTriangle.containsPoint( point, Qt::OddEvenFill ) )
			{
				qDebug("candidate triangle contains point, rejected"); // TODO remove
				v = ( v + 1 ) % shape.size();
				continue;
			}
		}
		
		// remove vertex from shape
		shape.remove( v );
		v = v % shape.size(); // resolves error when v was last vertex
		qDebug("vertex removed, shape\'s size is %d", shape.size() ); // TODO remove
		
		qDebug("relevant trianlge found (p=%lf)", p );
		
		if ( p > 0 )
		{
			list.append( createTriangleB2Shape( c, b, a ) );
		}
		else
		{
			list.append( createTriangleB2Shape( a, b, c ) );
		}
	}
	// create last triangle
	QPointF a = shape[ 0 ];
	QPointF b = shape[ 1 ];
	QPointF c = shape[ 2 ];
	
	// find out if it is convex one
	double p = product( a-b, c-b );
		if ( fabs(p) > MIN_PRODUCT )
		{
			qDebug("last triangle is relevant (p=%lf)", p );
			
			if ( p > 0 )
			{
				list.append( createTriangleB2Shape( c, b, a ) );
			}
			else
			{
				list.append( createTriangleB2Shape( a, b, c ) );
			}
		}
	
	// TODO temporary: bounding box:
	/*
	b2BoxDef* pBoxDef = new b2BoxDef();
	
	QRectF boxRect = shape.boundingRect();
	pBoxDef->extents.Set( boxRect.width() / 2.0, boxRect.height() / 2.0 );
	
	// compile result
	list.append( pBoxDef );
	*/
#endif

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


