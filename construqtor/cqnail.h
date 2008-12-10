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
#ifndef CQNAIL_H
#define CQNAIL_H

// local
#include "cqfragilerevolutejoint.h"
#include "cqphysicalbody.h"


/**
	Nail is a simplest revolition joint: no motors, no limits.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqNail : public CqFragileRevoluteJoint
{
	Q_OBJECT
public:

	// construction / destruction
	explicit CqNail( CqItem* parent = NULL );
	virtual ~CqNail();
	
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
};

/// A 'broken nail' object class. Its a non-selectable object wich represents broken nails
///\internal
class CqBrokenNail : public CqPhysicalBody
{
	Q_OBJECT
public:

	CqBrokenNail( CqItem* parent = NULL ) : CqPhysicalBody( parent ){ init(); }
	virtual ~CqBrokenNail(){}
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


#endif // CQNAIL_H

// EOF


