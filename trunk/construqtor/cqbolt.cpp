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

// box2d
#include "b2Joint.h"

// local
#include "cqbolt.h"
#include "cqsimulation.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqBolt );
CQ_ADD_TO_FACTORY( CqBrokenBolt );

static const QSizeF SIZE = QSizeF( 0.05, 0.05 );

// =================== constructor =======================
CqBolt::CqBolt(CqWorld* world): CqFragileRevoluteJoint(world)
{
	init();
}

// =================== constructor =======================
CqBolt::CqBolt(QGraphicsItem* parent, CqWorld* world): CqFragileRevoluteJoint(parent, world)
{
	init();
}

// ============================ init ======================
void CqBolt::init()
{
	setZValue( 2.0 );
	setLimits( true, 0.0, 0.0 ); // this should block any rotations
	
	setEditorFlags( editorFlags() | Selectable );
	setEditorFlags( editorFlags() & ~Movable );
	setEditorFlags( editorFlags() & ~Rotatable );
	
	// TODO random rotation
	setRotationRadians( qrand() * 2* M_PI / RAND_MAX );
}

// ========================== destructor ============================
CqBolt::~CqBolt()
{
	// nope
}

// ============================ paint ============================
void CqBolt::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	// simple selection indicator
	if ( selected() )
	{
		pPainter->setBrush( Qt::gray );
	}
	
	pPainter->setPen( colorByTemperature( temperature() ) );
	
	pPainter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
	pPainter->drawLine( QPointF( -SIZE.width()/2, 0),  QPointF(SIZE.width()/2, 0 ) ); 
}

// ========================== boundiong rect ======================
QRectF CqBolt::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}

// =============================== broken ========================
void CqBolt::broken()
{
	Q_ASSERT( simulation() );
	
	// create broken bolt
	CqBrokenBolt* pBrokenBolt = new CqBrokenBolt();
	
	pBrokenBolt->setPhysicalPos( worldPos() );
	pBrokenBolt->setPhysicalRotation( worldRotation() );
	
	simulation()->addItem( pBrokenBolt );
	
	// bye bye, cruel world!
	delete this;
}

// ============================== broken bolt: paint ===========================
void CqBrokenBolt::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/  )
{
	pPainter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
	pPainter->drawLine( QPointF( -SIZE.width()/2, 0),  QPointF(SIZE.width()/2, 0 ) ); 
}

// ============================ broken bolt: boundong rect ======================
QRectF CqBrokenBolt::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}

// ================================ broken bolt: init ===============================
void CqBrokenBolt::init()
{
	setName("Broken bolt");
	setEditorFlags( 0 );
	setMaterial( CqMaterial::steel() );
}

// ================================= create shape =================================
QList<b2ShapeDef*> CqBrokenBolt::createShape()
{
	b2CircleDef* pCircle = new b2CircleDef;
	
	pCircle->radius = SIZE.width() / 2.0;
	
	material().copyToShapeDef( pCircle );
	
	QList<b2ShapeDef*> result;
	result.append( pCircle );
	
	return result;
}

// EOF

