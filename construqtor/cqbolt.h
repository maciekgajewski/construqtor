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
#ifndef CQBOLT_H
#define CQBOLT_H

// local
#include "cqfragilerevolutejoint.h"
#include "cqphysicalbody.h"

/**
	Bolt is a revolute jont, similar to nail, but disallowing rotations.
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqBolt : public CqFragileRevoluteJoint
{
	Q_OBJECT
public:
	// construction / destruction
	CqBolt(CqWorld* world = NULL );
	CqBolt(QGraphicsItem* parent, CqWorld* world = NULL );
	virtual ~CqBolt();

	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
	virtual QRectF boundingRect() const;
	
protected:

	virtual void broken();							//!< called when joint is breaked
	
private:

	void init();
	
	/// A 'broken bolt' object class. Its a non-selectable object wich represents broken bolt
	class BrokenBolt : public CqPhysicalBody
	{
	public:
	
		BrokenBolt( CqWorld* world = NULL ) : CqPhysicalBody( world ){ init(); }
		virtual ~BrokenBolt(){}
		// operations 
		virtual void paint
			( QPainter * painter
			, const QStyleOptionGraphicsItem * option
			, QWidget * widget = 0 );
			
		virtual QRectF boundingRect() const;
	protected:
	
		virtual QList<b2ShapeDef*> createShape();
		
	private:
	
		void init();
		
	};
};

#endif // CQBOLT_H

// EOF


