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
 : CqCompoundItem()
{
	_wheelDiameter = wheelDiameter;
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
	// init wheel
	_pWheel = new CqWheel( _wheelDiameter );
	_pWheel->setEditorFlags( _pWheel->editorFlags() & ~Selectable );
	_pWheel->setZValue( 0.7 );
	
	// init engine
	_pEngine = new CqGirder(_wheelDiameter / 2, _wheelDiameter );
	_pEngine->setPos( 0, _wheelDiameter * 0.4 );
	_pEngine->setBrush( QColor( 0x80, 0x80, 0x80, 0x80 ) ); // semitransparent gray
	_pEngine->setMaterial( CqMaterial( 5.0, 0.2, 0.1 ) );
	_pEngine->setZValue( 0.5 );
	
	// init joint
	_pMotor = new Motor();
	_pMotor->setAnchorPoint( QPointF(0,0) );
	_pMotor->setConnectedBodies( _pWheel,  _pEngine );
	_pMotor->setMotorEnabled( true, 1.0, 10.0 );
	_pMotor->setZValue( 0.9 );
	
	addChild( _pWheel );
	addChild( _pMotor );
	addChild( _pEngine );
	
	setFollowedChild( _pMotor );
	
	// flags
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
	
}

// ======================== constructor ===================
CqPhysicalBody* CqWheelWithEngine::bodyHere( const QPointF& worldPoint )
{
	if ( _pEngine->contains( _pEngine->mapFromWorld( worldPoint ) ) )
	{
		return _pEngine;
	}
	
	return NULL;
}

// =========================== can connect here ===================
bool CqWheelWithEngine::canConnectHere( const QPointF& worldPoint )
{
	Q_ASSERT( _pEngine );
	QPointF enginePoint = _pEngine->mapFromWorld( worldPoint );
	bool contains =  _pEngine->contains( enginePoint );
}

// ======================== Motor : constructor ============
CqWheelWithEngine::Motor::Motor( CqWheelWithEngine* parent )
	: CqRevoluteJoint( parent )
{
	// TODO
}

// ======================== Motor : paint ============
void CqWheelWithEngine::Motor::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	pPainter->drawEllipse( boundingRect() );
}
	
// ===================== Motor : rect ==================
QRectF CqWheelWithEngine::Motor::boundingRect() const
{
	// TODO temporary
	return QRectF( -0.05, -0.05, 0.10, 0.10 );
}

// EOF




