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
#ifndef CQPOLYGONALBODY_H
#define CQPOLYGONALBODY_H

// Qt
#include <QPolygonF>

// local
#include "cqphysicalbody.h"

/**
	This is a physical body with shape defined by polygon.
	Polygon is spilt into triangles using GPC library.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPolygonalBody : public CqPhysicalBody
{
	Q_OBJECT
	Q_PROPERTY( bool connectable READ connectable WRITE setConnectable DESIGNABLE true );
	
public:
	// constrution / destruction
	explicit CqPolygonalBody( CqItem* parent = NULL );
	virtual ~CqPolygonalBody();
	
	// shape definition
	void setPolygon( const QPolygonF& ploygon );		///< Sets shape
	QPolygonF polygon() const { return _polygon; }		///< Returns shape
	
	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;
	virtual CqPhysicalBody* bodyHere( const QPointF& /*worldPoint*/ ) { return this; }
	virtual bool canConnectHere( const QPointF& /*worldPoint*/ ) { return _connectable; }
	virtual bool canBeMoved() const;
	virtual bool canBeRotated() const;
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 
	
	// properties
	void setConnectable( bool c ) { _connectable = c; }
	bool connectable() const { return _connectable; }

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
	
	QPolygonF	_polygon;		///< shape, as polygon
	bool		_connectable;	///< If entire body is connectable
	
};

#endif // CQPOLYGONALBODY_H

// EOF
