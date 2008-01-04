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
// libc
#include <math.h>

// local
#include "cqstone.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqStone );

// ================================= constructor ======================
CqStone::CqStone( CqItem* parent )
	: CqPolygonalBody( parent )
{
	init();
}

// ===================================== destructor ==================
CqStone::~CqStone()
{
	// nope
}

// ===================================== init ==================
void CqStone::init()
{
	setBrush( Qt::darkGray );
	setZValue( 0.1 ); // background, TODO once again: manage
	
	// TODO temporary flags
	setEditorFlags( editorFlags() | Movable | Rotatable | Selectable );
	
	// some 'stonish' material
	setMaterial( CqMaterial( 200.0, 0.3, 0.1 ) );
}

// ===================================== random stone  ==================
CqStone* CqStone::createRandomStone( double diameter )
{
	// algorithm:
	// use polar coordinates to create polygon. start with angle=0, 
	// select random distance, then increase angle by random value and select new dstance
	// This should give nice, irregular but consistent shape. Keep values
	// around desired radius
	
	// params
	const double MIN_ANGLE_STEP	= 0.3;
	const double MAX_ANGLE_STEP	= 0.5;
	double MIN_DISTANCE = diameter * 0.2;
	double MAX_DISTANCE = diameter * 0.9;
	double MAX_DISTANCE_DELTA = diameter * 0.1;
	
	// go
	double currentAngle = 0.0;
	QPolygonF result;
	// init ital distance
	double distance = MIN_DISTANCE  + ( MAX_DISTANCE - MIN_DISTANCE ) * double(qrand()) / RAND_MAX;
	
	while( currentAngle < M_PI * 2 )
	{
		result.append( QPointF( distance* cos( currentAngle ), distance * sin( currentAngle ) ) );
		currentAngle += MIN_ANGLE_STEP + ( MAX_ANGLE_STEP - MIN_ANGLE_STEP )* double(qrand()) / RAND_MAX;
		
		// select next distance
		distance += - MAX_DISTANCE_DELTA + 2 * MAX_DISTANCE_DELTA * double(qrand()) / RAND_MAX;
		
		if ( distance > MAX_DISTANCE ) distance = MAX_DISTANCE;
		if ( distance < MIN_DISTANCE ) distance = MIN_DISTANCE;
	}
	
	// now, with polygon ,create stone
	CqStone* pStone = new CqStone();
	
	pStone->setPolygon( result );
	
	return pStone;
}

// EOF


