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
#include <QFile>

// local
#include "cqsimulation.h"
#include "cqwheelwithengine.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqWheelWithEngine );
CQ_ADD_TO_FACTORY( CqWheelWithEngineMotor );

// tags
static const char* TAG_ENGINE	= "engine";
static const char* TAG_WHEEL	= "wheel";
static const char* TAG_MOTOR	= "motor";


// constants
static const double MAX_SPEED	= 20.0;		// max speed [rad/s?] 
static const double MAX_TORQUE	= 1000.0;	// max torque [Nm?]
static const CqMaterial ENGINE_MATERIAL( 400.0, 0.2, 0.1 ); // dens, friq, restit

// ======================== constructor ===================
CqWheelWithEngine::CqWheelWithEngine( CqItem* parent )
 : CqCompoundItem( parent )
{
	init();
}

// =========================== destructor ====================
CqWheelWithEngine::~CqWheelWithEngine()
{
	delete _pWheel;
	delete _pEngine;
	delete _pMotor;
}

// ======================== init ===================
void CqWheelWithEngine::init()
{
	_wheelDiameter = 1.0;
	// init wheel
	_pWheel = new CqWheel();
	_pWheel->setDiameter( _wheelDiameter );
	_pWheel->setEditorFlags( _pWheel->editorFlags() & ~Selectable );
	_pWheel->setZValue( 0.7 );
	// some aesthetic
	QFile f( "graphics/tire80.svg" );	
	if ( f.open( QIODevice::ReadOnly ) )
	{
		_pWheel->setSvgAppearance( f.readAll() );
	}
	
	// init engine
	_pEngine = new CqGirder();
	_pEngine->setSize( QSizeF( _wheelDiameter / 2, _wheelDiameter ) );
	_pEngine->setPos( 0, _wheelDiameter * 0.4 );
	_pEngine->setMaterial( ENGINE_MATERIAL );
	_pEngine->setZValue( 0.5 );
	_pEngine->setName("Engine");
	_pEngine->setBrush( QColor( 0x80, 0x80, 0x80, 0x80 ) ); // semitransparent gray
	
	// init joint
	_pMotor = new CqWheelWithEngineMotor();
	_pMotor->setAnchorPoint( QPointF(0,0) );
	_pMotor->setConnectedBodies( _pWheel,  _pEngine );
	_pMotor->setMotorEnabled( true, MAX_SPEED, MAX_TORQUE );
	_pMotor->setZValue( 0.9 );
	
	addChild( _pWheel );
	addChild( _pMotor );
	addChild( _pEngine );
	
	setFollowedChild( _pMotor );
	
	// flags
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
	
	// controller
	_controller.setJoint( _pMotor );
	
	// init this
	setName( "Wheel with engine" );
}

// ======================== constructor ===================
CqPhysicalBody* CqWheelWithEngine::bodyHere( const QPointF& worldPoint )
{
	QPointF engineCoords = _pEngine->mapFromWorld( worldPoint );
	if ( _pEngine->contains( engineCoords ) )
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
	
	return contains;
}

// ======================== Motor : constructor ============
CqWheelWithEngineMotor::CqWheelWithEngineMotor( CqWheelWithEngine* parent )
	: CqRevoluteJoint( parent )
{
	setName( "Motor" );
}

// ======================== Motor : paint ============
void CqWheelWithEngineMotor::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * /*option*/
	, QWidget * /*widget*/ )
{
	pPainter->drawEllipse( boundingRect() );
}
	
// ===================== Motor : rect ==================
QRectF CqWheelWithEngineMotor::boundingRect() const
{
	// TODO temporary
	return QRectF( -0.05, -0.05, 0.10, 0.10 );
}

// ======================= set simualtion ========================
void CqWheelWithEngine::setSimulation( CqSimulation* pSimulation )
{
	Q_ASSERT( pSimulation );
	CqCompoundItem::setSimulation( pSimulation );
	
	pSimulation->addController( &_controller );
}

// =========================== load =================================
void CqWheelWithEngine::load( const CqElement& element )
{
	CqCompoundItem::load( element );
	
	// delete currently held items
	delete _pMotor; // TODO - important: delete joint before deleting bodies
	delete _pEngine;
	delete _pWheel;
	
	// new objects are already created, we only need to extract their pointers
	_pEngine	= dynamic_cast<CqGirder*>( element.readItemPointer( TAG_ENGINE ) );
	_pWheel		= dynamic_cast<CqWheel*>( element.readItemPointer( TAG_WHEEL ) );
	_pMotor		= dynamic_cast<CqWheelWithEngineMotor*>(  element.readItemPointer( TAG_MOTOR ) );
	
	// test
	if ( ! _pEngine || ! _pWheel || ! _pMotor )
	{
		qFatal("wheel w/enigne elements not created properly");
	}
	
	// freshen the controller pointer
	_controller.setJoint( _pMotor );
	
	// TODO this should be loaded
	_pEngine->setBrush( QColor( 0x80, 0x80, 0x80, 0x80 ) ); // semitransparent gray
}

// ===============================================================
QString CqWheelWithEngine::description() const
{
	Q_ASSERT( _pMotor );
	
	QString d = QString("Wheel with engine, wheel %1%2cm, motor torque: %3NM, RPM: %4")
		.arg( QChar( 0x00F8 ) )			// phi
		.arg( 100*_pWheel->diameter() )	// wheel diameter [cm]
		.arg( _pMotor->maxTorque() )	// motor torque [NM]
		.arg( _pMotor->maxSpeed() / ( 2 * M_PI ) * 60 ) // motor speed [rad/s] -> [RPM]
		;
	return d;
}

// =========================== store =================================
void CqWheelWithEngine::store( CqElement& element ) const
{
	CqCompoundItem::store( element );
	
	// remember which object is what
	element.appendItemPointer( TAG_ENGINE, _pEngine );
	element.appendItemPointer( TAG_WHEEL, _pWheel );
	element.appendItemPointer( TAG_MOTOR, _pMotor );

}

// ======================================================================
void CqWheelWithEngine::setWheelDiameter( double diameter )
{
	_wheelDiameter = diameter;
	if ( _pWheel )
	{
		_pWheel->setDiameter( diameter );
	}
}

// EOF




