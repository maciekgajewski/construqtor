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
#include "cqphysicaldisk.h"

// ==================== contructor =======================
CqPhysicalDisk::CqPhysicalDisk(QGraphicsItem* parent, CqWorld* world): CqPhysicalBody(parent, world)
{
	init();
}

// ==================== contructor =======================
CqPhysicalDisk::CqPhysicalDisk(CqWorld* world): CqPhysicalBody(world)
{
	init();
}

// ==================== init =======================
void CqPhysicalDisk::init()
{
	_diameter = 0.0;
}

// ==================== destructor =======================
CqPhysicalDisk::~CqPhysicalDisk()
{
	// none
}

// ==================== set diameter =======================
void CqPhysicalDisk::setDiameter( double diameter )
{
	
	if ( diameter != _diameter )
	{
		_diameter = diameter;
		recreateBody();
	}
}

// ======================== creates shape defiiiton =========
QList< b2ShapeDef* > CqPhysicalDisk::createShape()
{
	b2CircleDef* pCircle = new b2CircleDef;
	pCircle->radius = _diameter / 2.0;
	
	material().copyToShapeDef( pCircle );
	
	QList< b2ShapeDef* > result;
	result.append( pCircle );
	
	return result;
}

// ======================== paint ============================
void CqPhysicalDisk::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * pOption
	, QWidget * pWidget )
{
	QTransform t;
	t.rotateRadians( rotationRadians() );
	
	pPainter->setTransform( t, true );
	
	// simple selection indicator
	QBrush b = brush();
	if ( selected() )
	{
		b = Qt::gray;
	}
	
	pPainter->setPen( pen() );
	pPainter->setBrush( b );
	// draw ellipse
	pPainter->drawEllipse( QRectF( -_diameter/2.0, -_diameter/2.0, _diameter, _diameter ) );
	// draw some features on it, to make rotation visible
	pPainter->drawLine( QLineF( _diameter*0.05, 0, _diameter*0.45, 0 ) );
	pPainter->drawLine( QLineF( -_diameter*0.05, 0, -_diameter*0.45, 0 ) );
}


// ======================== bounding rect  ==================
QRectF CqPhysicalDisk::boundingRect() const
{
	double brsize = _diameter * 1.1;
	return QRectF( -brsize/2, -brsize/2, brsize, brsize );
}

// ============================== shape ==============================
QPainterPath CqPhysicalDisk::shape() const
{
	QPainterPath path;
	path.addEllipse( -_diameter/2, -_diameter/2, _diameter, _diameter );
	
	return path;
}


// EOF

