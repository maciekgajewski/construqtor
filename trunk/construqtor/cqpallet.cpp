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
#include "cqitemfactory.h"
#include "cqpallet.h"

CQ_ADD_TO_FACTORY( CqPallet );

// constants
static const double PALETT_WIDTH	= 1.2;	// 1200
static const double PALETT_HEIGHT	= 0.144;	// 144 mm
static const double PALETT_BOARD_THICKNESS	= 0.022;	// 22 mm
static const double BLOCK_WIDTH		= 0.1;	// 100m


// =================================================================
CqPallet::CqPallet(CqItem* parent): CqPhysicalBody(parent)
{
	init();
}


// =================================================================
CqPallet::~CqPallet()
{
	// nope
}

// =================================================================
void CqPallet::init()
{
	setBrush( QBrush( QColor("#f7b21b") ) );
	//setPen( QPen( QColor( "#f7aa00" ) ) ); // looks ugly
	setPen( QPen( QColor( "#f7b21b" ) ) ); // looks ugly
	setName( "Pallet" );
	// use wood-like material, but with fgreater firction
	CqMaterial m = CqMaterial::wood();
	m.friction = 1.0;
	
	setMaterial( m );
	setZValue( 2.0 );
}

// =================================================================
QList< b2ShapeDef * > CqPallet::createShape()
{
	QList< b2ShapeDef * > result;
	b2BoxDef box;
	
	box.extents.Set( PALETT_WIDTH / 2.0, PALETT_BOARD_THICKNESS / 2.0 );
	material().copyToShapeDef( &box );
	
	box.localPosition.Set( 0.0, ( PALETT_HEIGHT - PALETT_BOARD_THICKNESS ) / 2.0 );
	result.append( new b2BoxDef( box ) );
	
	box.localPosition.Set( 0.0, -( PALETT_HEIGHT - PALETT_BOARD_THICKNESS ) / 2.0 );
	result.append( new b2BoxDef( box ) );
	
	return result;
	
}

// =================================================================
void CqPallet::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	// simple selection indicator
	QBrush b = brush();
	if ( selected() )
	{
		b = Qt::gray;
	}
	
	QColor c = b.color();
	QBrush semitransparent = QBrush( QColor( c.red(), c.green(), c.blue(), 0x80 ) );
	
	pPainter->setPen( pen() );
	pPainter->setBrush( b );
	
	// paint main boards
	pPainter->drawRect( QRectF
		( - PALETT_WIDTH / 2.0,	- PALETT_HEIGHT / 2.0
		, PALETT_WIDTH,	 PALETT_BOARD_THICKNESS ) );
	
	pPainter->drawRect( QRectF
		( - PALETT_WIDTH / 2.0,	PALETT_HEIGHT / 2.0 - PALETT_BOARD_THICKNESS 
		, PALETT_WIDTH,	 PALETT_BOARD_THICKNESS ) );
		
	// paint joining blocks
	pPainter->setBrush( semitransparent );
	
	pPainter->drawRect ( QRectF
		( - PALETT_WIDTH / 2.0,	- PALETT_HEIGHT / 2.0 + PALETT_BOARD_THICKNESS
		, BLOCK_WIDTH,			 PALETT_HEIGHT - PALETT_BOARD_THICKNESS*2.0 ) );
		
	pPainter->drawRect ( QRectF
		( PALETT_WIDTH / 2.0 - BLOCK_WIDTH,	- PALETT_HEIGHT / 2.0 + PALETT_BOARD_THICKNESS
		, BLOCK_WIDTH,			 PALETT_HEIGHT - PALETT_BOARD_THICKNESS*2.0 ) );
	
	pPainter->drawRect ( QRectF
		( - BLOCK_WIDTH / 2.0,	- PALETT_HEIGHT / 2.0 + PALETT_BOARD_THICKNESS
		, BLOCK_WIDTH,			 PALETT_HEIGHT - PALETT_BOARD_THICKNESS*2.0 ) );
	
}
	
// =================================================================
QRectF CqPallet::boundingRect() const
{
	return QRectF( - PALETT_WIDTH / 2.0, - PALETT_HEIGHT / 2.0, PALETT_WIDTH, PALETT_HEIGHT );
}

// EOF
