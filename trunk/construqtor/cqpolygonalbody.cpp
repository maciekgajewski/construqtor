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

// Qt
#include <QPainter>

// local
#include "cqpolygontriangulator.h"
#include "cqpolygonalbody.h"

// ============================== constructor ==========================
CqPolygonalBody::CqPolygonalBody( CqWorld* world )
	: CqPhysicalBody( world )
{
	init();
}

// ============================== constructor ==========================
CqPolygonalBody::CqPolygonalBody( QGraphicsItem* parent, CqWorld* world )
	: CqPhysicalBody( parent, world )
{
	init();
}

// ============================== destructor ==========================
CqPolygonalBody::~CqPolygonalBody()
{
	// nope
}

// ================================= init ================================
void CqPolygonalBody::init()
{
	// nope
}

// ================================== set polygon ============================
void CqPolygonalBody::setPolygon( const QPolygonF& ploygon )
{
	_polygon = ploygon;
	recreateBody();
}

// ================================= create shape =========================
QList<b2ShapeDef*> CqPolygonalBody::createShape()
{
	const double MIN_PRODUCT = 0.001;
	
	QList<b2ShapeDef*> list;
	
	CqPolygonTriangulator triangulator;
	QList< QPolygonF > triangles = triangulator.triangulate( _polygon );
	
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
	}
	
	return list;
}

// ============================== paint ===================================
void CqPolygonalBody::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	// simple selection indicator
	QBrush b = brush();
	if ( selected() )
	{
		b = Qt::gray;
	}
	
	pPainter->setPen( pen() );
	pPainter->setBrush( b );
	
	pPainter->drawPolygon( _polygon );
}

// ========================== bounding rect ==============================
QRectF CqPolygonalBody::boundingRect() const
{
	QRectF pbb = _polygon.boundingRect();
	
	// expand rect by:
	double dw = pbb.width() * 0.1;
	double dh = pbb.height() * 0.1;
	
	return QRectF( pbb.left() - dw/2, pbb.top() - dh/2, pbb.width() + dw, pbb.height() + dh );
}

// ========================== create triangle ================
b2PolyDef* CqPolygonalBody::createTriangleB2Shape( const QPointF& a, const QPointF& b, const QPointF& c )
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
double CqPolygonalBody::product( const QPointF& a, const QPointF& b )
{
	return a.x()*b.y() - a.y()*b.x();
}

// EOF

