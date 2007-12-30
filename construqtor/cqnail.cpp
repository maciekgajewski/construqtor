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
#include "cqnail.h"
#include "cqsimulation.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqNail );
CQ_ADD_TO_FACTORY( CqBrokenNail );

static const QSizeF SIZE = QSizeF( 0.02, 0.02 );

// ========================= construction ======================
CqNail::CqNail( CqWorld* world  )
	: CqFragileRevoluteJoint(world)
{
	init();
}

// ========================= construction ======================
CqNail::CqNail( QGraphicsItem* parent, CqWorld* world  )
	:CqFragileRevoluteJoint( parent, world )
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
	setEditorFlags( editorFlags() | Selectable );
	setEditorFlags( editorFlags() & ~Movable );
	setEditorFlags( editorFlags() & ~Rotatable );
}

// ========================= paint ======================
void CqNail::paint
	( QPainter * painter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/)
{
	// simple selection indicator
	if ( selected() )
	{
		painter->setBrush( Qt::gray );
	}
	
	painter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
}
	
// ========================= bounding rect ======================
QRectF CqNail::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}
// ============================== broken nail: paint ===========================
void CqBrokenNail::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/  )
{
	// simple selection indicator
	if ( selected() )
	{
		pPainter->setBrush( Qt::gray );
	}
	pPainter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
}

// ============================ broken bolt: boundong rect ======================
QRectF CqBrokenNail::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}

// ================================ broken nail: init ===============================
void CqBrokenNail::init()
{
	setName("Broken nail");
	setEditorFlags( 0 );
	setMaterial( CqMaterial::steel() );
}

// ================================= broken nail: create shape =================================
QList<b2ShapeDef*> CqBrokenNail::createShape()
{
	b2CircleDef* pCircle = new b2CircleDef;
	
	pCircle->radius = SIZE.width() / 2.0;
	
	material().copyToShapeDef( pCircle );
	
	QList<b2ShapeDef*> result;
	result.append( pCircle );
	
	return result;
}

// =============================== broken ========================
void CqNail::broken()
{
	Q_ASSERT( simulation() );
	
	// create broken nail
	CqBrokenNail* pBrokenNail = new CqBrokenNail();
	
	pBrokenNail->setPhysicalPos( worldPos() );
	pBrokenNail->setPhysicalRotation( worldRotation() );
	
	simulation()->addItem( pBrokenNail );
	
	// bye bye, cruel world!
	delete this;
}



// EOF

