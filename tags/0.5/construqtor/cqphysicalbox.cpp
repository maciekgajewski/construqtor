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
// Qt
#include <QPainter>


// local
#include "cqphysicalbox.h"

// XML tags
static const char* TAG_BOX_SIZE	= "boxsize";

// ==================== contructor =======================
CqPhysicalBox::CqPhysicalBox( CqItem* parent )
	: CqPhysicalBody( parent )
{
	init();
}

// ==================== init =======================
void CqPhysicalBox::init()
{
	// nothing to intilaize
}

// ==================== destructor =======================
CqPhysicalBox::~CqPhysicalBox()
{
	// nothing
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
	, const QStyleOptionGraphicsItem * /*pOption*/
	, QWidget * /*pWidget*/ )
{
	QRectF box = QRectF( QPointF( - _size.width()/2.0, - _size.height()/2.0), _size );
	
	// simple selection indicator
	QBrush b = brush();
	if ( selected() )
	{
		b = Qt::gray;
	}
	
	pPainter->setPen( pen() );
	pPainter->setBrush( b );
	pPainter->drawRect( box );
}


// ======================== bounding rect  ==================
QRectF CqPhysicalBox::boundingRect() const
{
	QSizeF bbs = _size * 1.1; // enlarge size a bit
	return QRectF
		( QPointF
			( - _size.width()/2.0
			, - _size.height()/2.0
			)
		, _size
		);
	
}

// =========================================================
void CqPhysicalBox::store( CqElement& element ) const
{
	CqPhysicalBody::store( element );
	
	element.appendSizeF( TAG_BOX_SIZE, _size );
}

// =========================================================
void CqPhysicalBox::load( const CqElement& element )
{
	CqPhysicalBody::load( element );
	
	_size = element.readSizeF( TAG_BOX_SIZE );
}


// EOF

