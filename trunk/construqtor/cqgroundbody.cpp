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
#include "cqsimulation.h"
#include "cqgroundbody.h"

// =============================== constructor =======================
CqGroundBody::CqGroundBody(QGraphicsItem* parent, CqWorld* world)
	: CqPhysicalBody(parent, world)
{
	init();
}

// =============================== constructor =======================
CqGroundBody::CqGroundBody(CqWorld* world)
	: CqPhysicalBody(world)
{
	init();
}

// ================================= init =============================
void CqGroundBody::init()
{
	setMaterial( CqMaterial( 0, 0.9, 0.2 ) );
	setBrush( Qt::darkGreen );
}

// =============================== destructor =======================
CqGroundBody::~CqGroundBody()
{
	// nope
}

// ================================== height ========================
/// Calculates y for specified x.
double CqGroundBody::height( double x )
{
	// TODO
	return 0;
}

// ================================= set heightmap =====================
/// Sets height map
void CqGroundBody::setHeightmap( const QPolygonF& heightMap )
{
	_heightmap = heightMap;
}

// =============================== random ground =======================
/// Creates random ground for specified simulation
CqGroundBody* CqGroundBody::randomGround( CqSimulation* pSimulation, double maxSlope )
{
	Q_ASSERT( pSimulation );
	
	// cache
	double worldHeight = pSimulation->worldRect().height();
	double worldWidth = pSimulation->worldRect().width();
	
	// params / constants
	double MAX_HEIGHT = worldHeight * 0.9; // fit between 10% and 90%
	double MIN_HEIGHT = worldHeight * 0.1;
	
	const double SAFEAREA_WIDTH = 30.0;	// 30m
	
	const double MAX_SLICE_WIDTH = 10.0;
	const double MIN_SLICE_WIDTH = 1.0;
	
	double MAX_SLOPE = maxSlope; // difficulty level. max slope is MAX_SLOPE * (1+SLOPE_VARIATION)
	const double SLOPE_GAIN = 0.02;	// controlelr gain: slope/meter difference
	double SLOPE_VARIATION = maxSlope * 0.3;
	
	// first - get random target heights at constant intervals
	const int SLICES = 10; // 10 slices
	double targets[ SLICES ];
	
	for ( int i = 0; i < SLICES; i++ )
	{
		targets[ i ] = MIN_HEIGHT + ( MAX_HEIGHT - MIN_HEIGHT ) * double(qrand()) / RAND_MAX;
	}
	
	// now - randomize slices
	QPolygonF points;
	
	// intial height
	double height = MIN_HEIGHT + ( MAX_HEIGHT - MIN_HEIGHT ) * double(qrand()) / RAND_MAX;
	
	// first points - left safe area
	points.append( QPointF( 0, height ) );
	points.append( QPointF( SAFEAREA_WIDTH, height ) );
	
	// now - randomize points
	double x = SAFEAREA_WIDTH;
	while ( x < ( worldWidth - SAFEAREA_WIDTH - MAX_SLICE_WIDTH ) )
	{
		double sliceWidth = MIN_SLICE_WIDTH + ( MAX_SLICE_WIDTH - MIN_SLICE_WIDTH ) * double(qrand()) / RAND_MAX;
		
		// find out which slice is it
		int slice = qRound( ( x - SAFEAREA_WIDTH ) / (( worldWidth - 2*SAFEAREA_WIDTH ) / SLICES) );
		
		// calculate slope
		double calculatedSlope = SLOPE_GAIN * ( targets[slice] - height );
		
		// apply limit
		if ( calculatedSlope > MAX_SLOPE ) calculatedSlope = MAX_SLOPE;
		if ( calculatedSlope < -MAX_SLOPE ) calculatedSlope = -MAX_SLOPE;
		
		// add randomization
		double slope = calculatedSlope - SLOPE_VARIATION + 2*SLOPE_VARIATION*( double(qrand()) / RAND_MAX );
		
		// calculate new height
		height = height + slope * sliceWidth;
		x += sliceWidth;
		
		points.append( QPointF( x, height ) );
		
	}
	
	// close: right safe area
	points.append( QPointF( worldWidth, height ) );
	
	// create body
	CqGroundBody* pGround = new CqGroundBody();
	points.translate( - worldWidth / 2, - worldHeight / 2 );
	pGround->setHeightmap( points );
	
	return pGround;
}

// =========================== paint ============================
void CqGroundBody::paint
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
	
	// create cached polygin, if none
	if ( _painterPolygon.empty() )
	{
		_painterPolygon = _heightmap;
		// add corners
		_painterPolygon.append( simulation()->worldRect().topRight() );
		_painterPolygon.prepend( simulation()->worldRect().topLeft() );
	}
	
	
	pPainter->drawPolygon( _painterPolygon );
	
	// TODO debug
	//debugDrawCollision( pPainter );
}

// ============================ boundoing rect =====================
QRectF CqGroundBody::boundingRect() const
{
	// create cached polygin, if none
	if ( _painterPolygon.empty() )
	{
		_painterPolygon = _heightmap;
		// add corners
		_painterPolygon.append( simulation()->worldRect().topRight() );
		_painterPolygon.prepend( simulation()->worldRect().topLeft() );
	}
	
	return _painterPolygon.boundingRect();
}

// =========================== create shape ============================
QList<b2ShapeDef*> CqGroundBody::createShape()
{
	const double MIN_PRODUCT = 0.001;
	
	QList<b2ShapeDef*> list;
	
	double bottom = simulation()->worldRect().top(); // note: QRect uses higher-smaller y coords
	
	
	// create two triangles for each point (trapezoid) in height map
	int num = _heightmap.size();
	QPointF prevPoint = _heightmap.first();
	for ( int i = 1; i < num; i++ )
	{
		QPointF point = _heightmap[ i ];
		
		// calculate two points at bottom
		QPointF prevBottomPoint( prevPoint.x(), bottom ); // point at bottom, just below prevPoint
		QPointF bottomPoint( point.x(), bottom ); // point at bottom, just below point
		
		list.append( createTriangleB2Shape( bottomPoint, prevPoint, prevBottomPoint ) );
		list.append( createTriangleB2Shape( bottomPoint, point, prevPoint ) );
		// TODO try CLOCKWISE
		//list.append( createTriangleB2Shape( bottomPoint, prevPoint, point ) );
		//list.append( createTriangleB2Shape( bottomPoint, prevBottomPoint, prevPoint ) );
		
		
		prevPoint = point;
	}
	
	return list;
}

// ========================== create triangle ================
b2PolyDef* CqGroundBody::createTriangleB2Shape( const QPointF& a, const QPointF& b, const QPointF& c )
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
double CqGroundBody::product( const QPointF& a, const QPointF& b )
{
	return a.x()*b.y() - a.y()*b.x();
}

// EOF

