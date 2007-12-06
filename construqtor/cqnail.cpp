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
#include "cqnail.h"

// ========================= construction ======================
CqNail::CqNail( CqWorld* world  )
	: CqRevoluteJoint(world)
{
	// nope
}

// ========================= construction ======================
CqNail::CqNail( QGraphicsItem* parent, CqWorld* world  )
	:CqRevoluteJoint( parent, world )
{
	// nope
}

// ========================= destruction ======================
CqNail::~CqNail()
{
	// nope
}

// ========================= paint ======================
void CqNail::paint
	( QPainter * painter
	, const QStyleOptionGraphicsItem * option
	, QWidget * widget)
{
	//painter->drawEllipse( QRectF( pos() - QPointF( 0.01, 0.01 ), QSizeF( 0.02, 0.02 ) ) );
	painter->drawEllipse( QRectF( - QPointF( 0.01, 0.01 ), QSizeF( 0.02, 0.02 ) ) );
}
	
// ========================= bounding rect ======================
QRectF CqNail::boundingRect() const
{
	//return QRectF( pos() - QPointF( 0.2, 0.2 ), QSizeF( 0.4, 0.4 ) );
	return QRectF( - QPointF( 0.2, 0.2 ), QSizeF( 0.4, 0.4 ) );
}


// EOF

