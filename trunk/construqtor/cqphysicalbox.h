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
#ifndef CQPHYSICALBOX_H
#define CQPHYSICALBOX_H

// local
#include "cqphysicalbody.h"


/**
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPhysicalBox : public CqPhysicalBody
{
	Q_OBJECT
public:
	
	// construction / destruction
	CqPhysicalBox(QGraphicsItem* parent, CqWorld* world = NULL);
	CqPhysicalBox(CqWorld* world = NULL);
	virtual ~CqPhysicalBox();

	
	// properties
	void setSize( const QSizeF& size );		///< Sets/ changes size
	QSizeF size() const { return _size; };	///< Returns size

	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;
	virtual CqPhysicalBody* bodyHere( const QPointF& /*worldPoint*/ ) { return this; }
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 
	
protected:

	// reimplementables
	
    virtual QList< b2ShapeDef* > createShape();

private:

	// methods
	void init();		///< Initializes instance

	// data
	
	QSizeF	_size;		///< box's size

};

#endif	// CQPHYSICALBOX_H

// EOF


