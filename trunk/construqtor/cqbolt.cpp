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

// box2d
#include "b2Joint.h"

// local
#include "cqbolt.h"
#include "cqsimulation.h"

static const QSizeF SIZE = QSizeF( 0.05, 0.05 );

// =================== constructor =======================
CqBolt::CqBolt(CqWorld* world): CqRevoluteJoint(world)
{
	init();
}

// =================== constructor =======================
CqBolt::CqBolt(QGraphicsItem* parent, CqWorld* world): CqRevoluteJoint(parent, world)
{
	init();
}

// ============================ init ======================
void CqBolt::init()
{
	setZValue( 2.0 );
	setLimits( true, 0.0, 0.0 ); // this should block any rotations
	
	// TODO random rotation
}

// =================== destructor =======================
CqBolt::~CqBolt()
{
	// nope
}

// ====================== paint =================
void CqBolt::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	pPainter->drawEllipse( QRectF( - QPointF( SIZE.width(), SIZE.height() ) / 2, SIZE ) );
	pPainter->drawLine( QPointF( -SIZE.width()/2, 0),  QPointF(SIZE.width()/2, 0 ) ); 
}
	
// ================= boundiong rect =============
QRectF CqBolt::boundingRect() const
{
	QSizeF bbs = SIZE *1.1;
	return QRectF( - QPointF( bbs.width(), bbs.height() ) / 2, bbs );
}

// ====================== simualtion step =================
void CqBolt::simulationStep()
{
	CqRevoluteJoint::simulationStep();
	/*
	if ( b2joint() )
	{
		double its = simulation()->invTimeStep();
		b2Vec2 rf = b2joint()->GetReactionForce( its );
		double rt = b2joint()->GetReactionTorque( its );
		
		// TODO modify temperature here. Move code to CqRevoluteJoint
	}
	*/
}

// EOF

