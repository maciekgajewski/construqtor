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
#ifndef CQPALLET_H
#define CQPALLET_H

// local
#include "cqphysicalbody.h"

/**
	Standard pallet (EURO: 800x1200x140), for transporting boxes.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqPallet : public CqPhysicalBody
{
	Q_OBJECT
public:
	CqPallet( CqItem* parent = NULL );
	virtual ~CqPallet();
	
	// body
	virtual QList< b2ShapeDef * > createShape();

	// graphics item
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;
	
private:

	void init();
};

#endif // CQPALLET_H

// EOF


