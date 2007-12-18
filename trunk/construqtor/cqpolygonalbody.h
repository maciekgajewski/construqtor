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
#ifndef CQPOLYGONALBODY_H
#define CQPOLYGONALBODY_H

// Qt
#include <PolygonF>

// local
#include "cqphysicalbody.h"

/**
	This is a physical body with shape defined by polygon.
	Polygon is spilt into triangles using GPC library.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPolygonalBody : public CqPhysicalBody
{
public:
	// constrution / destruction
	CqPolygonalBody( CqWorld* world = NULL );
	explicit CqPolygonalBody( QGraphicsItem* parent, CqWorld* world = NULL );
	virtual ~CqPolygonalBody();
	
	// shape definition
	void setShape( const QPolygonF& ploygon );			///< Sets shape
	QPolygonF shape() const { return _polygon; }		///< Returns shape
	
	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;
	virtual CqPhysicalBody* bodyHere( const QPointF& /*worldPoint*/ ) { return this; }
	
protected:
	
	virtual QList<b2ShapeDef*> createShape();			///< Creates body shape
	
private:	

	// methods
	
	void init();

	// data
	
	QPolygonF	_polygon;		///< shape, as polygon
};

#endif
