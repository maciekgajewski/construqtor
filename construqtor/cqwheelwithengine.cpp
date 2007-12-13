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
#include "cqsimulation.h"
#include "cqwheelwithengine.h"

// ======================== constructor ===================
CqWheelWithEngine::CqWheelWithEngine( double wheelDiameter )
 : CqItem()
 , _engine( this )
 , _wheel( this )
 , _motor( this )
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
	// TODO move to CqItem
	_engine.setWorld( pWorld );
	_wheel.setWorld( pWorld );
	_motor.setWorld( pWorld );
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
bool CqWheelWithEngine::canConnectHere( const QPointF& /*scenePoint*/ )
{
	// TODO
	return false;
}

// ======================== constructor ===================
CqPhysicalBody* CqWheelWithEngine::bodyHere( const QPointF& /*scenePoint*/ )
{
	// TODO
	return false;
}

// ======================== Engine : constructor ============
CqWheelWithEngine::Engine::Engine( CqWheelWithEngine* parent )
	: CqPhysicalBox( parent )
{
	// TODO init
}

// ======================== Engine : paint ============
void CqWheelWithEngine::Engine::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * option
	, QWidget * /*widget*/ )
{
	// TODO
	CqPhysicalBox::paint( pPainter, option );
}

// ======================== Wheel : constructor ============
CqWheelWithEngine::Wheel::Wheel( CqWheelWithEngine* parent )
	: CqPhysicalDisk( parent )
{
	// TODO
}

// ======================== Wheel : paint ============
void CqWheelWithEngine::Wheel::paint
	( QPainter * pPaint
	, const QStyleOptionGraphicsItem * option
	, QWidget * widget )
{
	CqPhysicalDisk::paint( pPaint, option, widget );
}

// ======================== Motor : constructor ============
CqWheelWithEngine::Motor::Motor( CqWheelWithEngine* parent )
	: CqRevoluteJoint( parent )
{
	// TODO
}

// ======================== Motor : paint ============
void CqWheelWithEngine::Motor::paint
	( QPainter * painter
	, const QStyleOptionGraphicsItem * option
	, QWidget * widget )
{
	// TODO temportary
}
	
// ===================== Motor : rect ==================
QRectF CqWheelWithEngine::Motor::boundingRect() const
{
	// TODO temporary
	return QRectF( -0.05, -0.05, 0.10, 0.10 );
}

// EOF




