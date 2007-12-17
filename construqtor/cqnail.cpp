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


static const QSizeF SIZE = QSizeF( 0.02, 0.02 );

// ========================= construction ======================
CqNail::CqNail( CqWorld* world  )
	: CqRevoluteJoint(world)
{
	init();
}

// ========================= construction ======================
CqNail::CqNail( QGraphicsItem* parent, CqWorld* world  )
	:CqRevoluteJoint( parent, world )
{
	init();
}

// ========================= destruction ======================
CqNail::~CqNail()
{
	// nope
}

// =========================== init ===========================
void CqNail::init()
{
	setZValue( 2.0 );
}

// ========================= paint ======================
void CqNail::paint
	( QPainter * painter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/)
{
	painter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
}
	
// ========================= bounding rect ======================
QRectF CqNail::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}


// EOF

