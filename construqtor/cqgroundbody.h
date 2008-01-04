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
#ifndef CQGROUNDBODY_H
#define CQGROUNDBODY_H

// Qt
#include <QPolygonF>

// local
#include "cqphysicalbody.h"

/**
	This is the ground body. Its just a polygonl body with 'groundish' default setting, which
	is configured by, and holds heightmap. It also calculates groud height at specified position.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqGroundBody : public CqPhysicalBody
{
	Q_OBJECT
public:
	
	explicit CqGroundBody( CqItem* parent = NULL);
	virtual ~CqGroundBody();
	
	double height( double x );							///< Calculates ground height at specified x
	
	void setHeightmap( const QPolygonF& heightMap );	///< Sets heightmap
	
	/// Creates random ground for specified simulation
	static CqGroundBody* randomGround( CqSimulation* pSimulation, double maxSlope );
	
	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
	virtual QRectF boundingRect() const;
	virtual bool contains( const QPointF& pos ) const;
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 

protected:
	
	virtual QList<b2ShapeDef*> createShape();			///< Creates body shape

private:

	// methods
	
	void init();
	/// Creates polygonal b2ShapeDef, based on three points
	static b2PolyDef* createTriangleB2Shape( const QPointF& a, const QPointF& b, const QPointF& c );
	/// Calculates cross product of two vectors
	static double product( const QPointF& a, const QPointF& b );

	// data
	
	QPolygonF	_heightmap;					///< Heightmap - poins of ground surface
	mutable QPolygonF	_painterPolygon;	///< Cache: painted polygon
};

#endif // CQGROUNDBODY_H

// EOF


