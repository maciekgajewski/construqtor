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
#include "cqwheelwithengine.h"

// ======================== constructor ===================
CqWheelWithEngine::CqWheelWithEngine( double wheelDiameter )
 : CqItem()
 , _engine( this )
 , _wheel( this )
{
	_wheel.setDiameter( wheelDiameter );
	init();
}

// =========================== destructor ====================
CqWheelWithEngine::~CqWheelWithEngine()
{
	// nothing
}

// ======================== init ===================
void CqWheelWithEngine::init()
{
	// TODO
}

// ======================== set world ===================
void CqWheelWithEngine::setWorld ( CqWorld* pWorld )
{
	_engine.setWorld( pWorld );
	_wheel.setWorld( pWorld );
	_joint.setWorld( pWorld );
}

// ======================== constructor ===================
void CqWheelWithEngine::setRotationRadians( double radians )
{
	// TODO
}

// ======================== constructor ===================
double CqWheelWithEngine::rotationRadians() const
{
	// TODO
	return 0.0;
}

// ======================== constructor ===================
void CqWheelWithEngine::setPhysicalRotation( double radians )
{
	CqItem::setPhysicalRotation( radians );
	// TODO
}

// ======================== constructor ===================
void CqWheelWithEngine::setPhysicalPos( const QPointF& pos )
{
	CqItem::setPhysicalPos( pos );
	// TODO
}

// ======================== constructor ===================
bool CqWheelWithEngine::canBeMoved() const
{
}

// ======================== constructor ===================
bool CqWheelWithEngine::canBeRotated() const
{
	return simulation()->canBeMoved( this );
}

// ======================== constructor ===================
bool CqWheelWithEngine::canConnectHere( const QPointF& scenePoint )
{
	// TODO
	return false;
}

// ======================== constructor ===================
CqPhysicalBody* CqWheelWithEngine::bodyHere( const QPointF& scenePoint )
{
	// TODO
	return false;
}

// EOF




