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
#include "cqphysicalbox.h"
#include "cqsimulation.h"

// ==================== contructor =======================
CqPhysicalBox::CqPhysicalBox(QGraphicsItem* parent, CqWorld* world): CqPhysicalBody(parent, world)
{
	init();
}

// ==================== contructor =======================
CqPhysicalBox::CqPhysicalBox(CqWorld* world): CqPhysicalBody(world)
{
	init();
}

// ==================== init =======================
void CqPhysicalBox::init()
{
}

// ==================== destructor =======================
CqPhysicalBox::~CqPhysicalBox()
{
}

// ==================== contructors =======================
/// Sets box sie.
/// If called on created body, destroys it. Body will be created when neccesary
void CqPhysicalBox::setSize( const QSizeF& size )
{
	
	if ( size != _size )
	{
		_size = size;
		recreateBody();
	}
}

// ======================== creates shape defiiiton =========
QList< b2ShapeDef* > CqPhysicalBox::createShape()
{
	b2BoxDef* pBox = new b2BoxDef;
	pBox->extents.Set( _size.width() / 2.0, _size.height() / 2.0 );
	
	material().copyToShapeDef( pBox );
	
	QList< b2ShapeDef* > result;
	result.append( pBox );
	
	return result;
}

// ======================== paint ============================
void CqPhysicalBox::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * pOption
	, QWidget * pWidget )
{
	QTransform t;
	t.rotateRadians( rotationRadians() );
	
	QRectF box = QRectF( QPointF( - _size.width()/2.0, - _size.height()/2.0), _size );
	QPolygonF rotatedBox = t.map( QPolygonF( box ) );
	
	
	// simple selection indicator
	QBrush b = brush();
	if ( isSelected() )
	{
		b = Qt::gray;
	}
	
	pPainter->setPen( pen() );
	pPainter->setBrush( b );
	pPainter->drawPolygon( rotatedBox );
}


// ======================== bounding rect  ==================
QRectF CqPhysicalBox::boundingRect() const
{
	QTransform t;
	t.rotateRadians( rotationRadians() );
	
	QRectF box = QRectF
		( QPointF
			( - _size.width()/2.0
			, - _size.height()/2.0
			)
		, _size
		);
	QPolygonF rotatedBox = t.map( QPolygonF( box ) );
	
	return rotatedBox.boundingRect();
}

// ======================== can be selected =============
bool CqPhysicalBox::canBeSelected() const
{
	// of course, id simulation permits
	return simulation()->canBeSelected( this );
}

// ========================= can be moved ================
bool CqPhysicalBox::canBeMoved() const
{
	// if there is no joins, then yes
	return _joints.empty()  && simulation()->canBeMoved( this );
}

// EOF

