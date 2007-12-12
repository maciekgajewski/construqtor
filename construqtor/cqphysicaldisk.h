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
#ifndef CQPHYSICALDISK_H
#define CQPHYSICALDISK_H

// local
#include "cqphysicalbody.h"


/**
	Uniform monolithi physical body with a shape of a disk
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPhysicalDisk : public CqPhysicalBody
{
public:
	
	// construction / destruction
	CqPhysicalDisk(QGraphicsItem* parent, CqWorld* world = NULL);
	CqPhysicalDisk(CqWorld* world = NULL);
	virtual ~CqPhysicalDisk();

	
	// properties
	void setDiameter( double diameter );			///< Sets/ changes diameter
	double diameter() const { return _diameter; };	///< Returns diameter

	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
	
	/// Returns body under poitn - single body everywhere
	virtual CqPhysicalBody* bodyHere( const QPointF& scenePoint ) { return this; }
	
protected:

	// reimplementables
	
    virtual QList< b2ShapeDef* > createShape();

private:

	// methods
	void init();		///< Initializes instance

	// data
	
	double	_diameter;		///< diameter

};

#endif // CQPHYSICALDISK_H

// EOF


