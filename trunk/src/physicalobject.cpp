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

#include <QMatrix>

#include "b2Body.h"

#include "world.h"

#include "physicalobject.h"

PhysicalObject::PhysicalObject( const QPolygonF& shape, World* pWorld )
 : QObject(pWorld)
{
	Q_ASSERT( pWorld );
	
	// TODO simplified - used boundoing box as box
	QRectF boxRect = shape.boundingRect();
	
	// find position
	double x = boxRect.x() + boxRect.width() / 2.0;
	double y = boxRect.y() + boxRect.height() / 2.0;
	
	// store shape
	_outline = shape;
	_outline.translate( -x, -y ); // make outline position-agnostic
	
	
	// create body
	b2BoxDef boxDef;
	
	boxDef.extents.Set( boxRect.width() / 2.0, boxRect.height() / 2.0 );
	boxDef.density = 1.0f;
	boxDef.friction = 0.3f;
	
	b2BodyDef bodyDef;
	bodyDef.position.Set( x, y );
	bodyDef.AddShape(&boxDef);
	_pBody = pWorld->b2world()->CreateBody(&bodyDef);

}

// =================== copy construtor ===========
PhysicalObject::PhysicalObject( const PhysicalObject& t )
	: QObject( t.parent() )
	, _outline( t._outline )
	, _pBody( t._pBody )
{
}

// ===================== assignment operator ============
PhysicalObject& PhysicalObject::operator=( const PhysicalObject& t )
{
	_outline = t._outline;
	_pBody = t._pBody;
}

// ===================== destructor ===========
PhysicalObject::~PhysicalObject()
{
}

// ========================= return object outline in physical space ===
QPolygonF PhysicalObject::outline() const
{
	Q_ASSERT( _pBody );
	// get body pos
	b2Vec2 position = _pBody->GetCenterPosition();
	float32 rotation = _pBody->GetRotation();
	
	// transform shape
	// TODO suboptimal, use matrix rotation
	QMatrix transformation;
	transformation.translate( position.x, position.y );
	transformation.rotate( rotation * 180.0 / M_PI );
	
	// TODO remove
	/*
	qDebug("object moves to: %.2lf,%.2lf, rotates to: %.2lf"
		, (double)position.x, (double)position.y
		, (double)rotation );
	*/
	
	return transformation.map( _outline );
	
}


