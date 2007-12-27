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

// box2d
#include "b2World.h"

// local
#include "cqsimulation.h"
#include "cqnail.h"
#include "cqphysicalbox.h" 
#include "cqmotorcontroller.h"
#include "cqgroundbody.h"
#include "cqdocument.h"


// constants
static const int SIMULATION_INTERVAL	= 100;	// [ms]
static const double B2D_SPS				= 60.0;	// Box2D simulation steps per second

// XML tags
static const char* ROOT_ELEMENT		= "simulaton";
static const char* TAG_WORLD_RECT	= "worldrectangle";		///< World rectangle
static const char* TAG_GND_ELEMENT	= "ground";				///< Ground elements
static const char* TAG_GRAVITY		= "gravity";

// ===================== constructor =================
CqSimulation::CqSimulation(QObject* parent): QObject(parent)
{
	// init
	init();
	connect( &_simulationTimer, SIGNAL(timeout()), SLOT(simulationTimerTimeout() ) );
}

// ======================== destructoir ==================
CqSimulation::~CqSimulation()
{
	// none
}

// ======================== start ==================
void CqSimulation::start()
{
	assurePhysicalObjectsCreated();
	
	// tell everyone simulation will start
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStarted();
		}
	}
	_simulationTimer.start( SIMULATION_INTERVAL );
	
	emit simulationStarted();
}

// ======================== stop ==================
void CqSimulation::stop()
{
	_simulationTimer.stop();
		
	// tell everyone simulation has stopped
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStopped();
		}
	}

	emit simulationPaused();
}

// ======================== isRunning ==================
bool CqSimulation::isRunning() const
{
	return _simulationTimer.isActive();
}

// ========================== inv time step ================
double CqSimulation::invTimeStep() const
{
	return B2D_SPS;
}

// =========================== timer timeout =============
void CqSimulation::simulationTimerTimeout()
{
	// physical world simulation step
	_pPhysicalWorld->Step( 1.0/B2D_SPS, B2D_SPS * SIMULATION_INTERVAL / 1000.0 );
	
	// update all shapes
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody && pBody->physicalParent() == NULL ) // only top-level items
		{
			pBody->simulationStep();
		}
	}
	
	emit simulationStep();
}

// ============================== init =============================
void CqSimulation::init()
{
	_worldRect	= QRect( -250, -100, 500, 200 ); // 500x200 m
	_gravity	= QPointF( 0.0, -10.0 );
	
	createWorld();
	initScene();
}

// =========================== create world =============
void CqSimulation::createWorld()
{
	
	// world size spec
	b2AABB worldAABB;
	worldAABB.minVertex.Set( _worldRect.left(), _worldRect.top() );
	worldAABB.maxVertex.Set( _worldRect.right(), _worldRect.bottom() );
	
	// gravity
	b2Vec2 gravity( _gravity.x(), _gravity.y() );
	
	// create world
	_pPhysicalWorld = new CqWorld( worldAABB, gravity, true /* do sleep*/, this );
	
	_scene.setSceneRect( _worldRect );
	
	stop();
}

// ============================== init scene =============================
void CqSimulation::initScene()
{
	CqGroundBody *pGround = CqGroundBody::randomGround( this, 0.5 );
	addGroundItem( pGround );
}

// =========================== assure objects created =============
void CqSimulation::assurePhysicalObjectsCreated()
{
	// TODO suboptimal - maintain separate list of bodyless-objects
	QList< QGraphicsItem* > items = _scene.items();
	
	// first - create bodies
	foreach( QGraphicsItem* pItem, items )
	{
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		
		if ( pBody )
		{
			pBody->assureBodyCreated();
		}
	}
	
	// second - create joints
	foreach( QGraphicsItem* pItem, items )
	{
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		
		if ( pJoint )
		{
			pJoint->assureJointCreated();
		}
	}
}

// =============================== add item =========================
void CqSimulation::addItem( CqItem* pItem )
{
	Q_ASSERT( pItem );
	
	// take ownership
	pItem->setParent( this );
	
	_scene.addItem( dynamic_cast<QGraphicsItem*>( pItem ) ); // TODO wild try
	
	pItem->setSimulation( this );
	pItem->setWorld( _pPhysicalWorld );
	if ( isRunning() )
	{
		// make sure it is created
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		if ( pBody )
		{
			pBody->assureBodyCreated();
		}
	
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		if ( pJoint )
		{
			pJoint->assureJointCreated();
		}
	}
}

// ============================= can be selected ? ==================
bool CqSimulation::canBeSelected( const CqItem* pItem ) const
{
	Q_ASSERT( pItem );
	
	// TODO 'selectable' are here
	return true;
}

// ============================== can be moved ? =====================
bool CqSimulation::canBeMoved( const CqItem* /*pItem*/ ) const
{
	// TODO 'movable' area here
	return ! isRunning();
}

// ============================== can be moved here ? =====================
bool CqSimulation::canBeMovedHere( const CqItem* /*pItem*/, const QPointF& /*pos*/ ) const
{
	// TODO editable region here
	return true;
}

// ================================ can be rotated =======================
bool CqSimulation::canBeRotated( const CqItem* /*pItem*/ ) const
{
	// TODO 'movable' area here
	return ! isRunning();
}
// ============================ can add here ? ======================
// Can item be added here now?
bool CqSimulation::canAddHere( const CqItem* /*pItem*/, const QPointF& pos ) const
{
	// TODO 'edit' area here
	if ( ! isRunning() && _worldRect.contains( pos ) )
	{
		// make sure it is not inside any ground body
		foreach( CqItem* pGround, _groundItems )
		{
			if ( pGround->contains( pos ) )
			{
				return false;
			}
			
			return true;
		}
	}
	
	return false;
}

// ============================== cotroller destroyed ==================
void CqSimulation::controllerDestroyed( QObject* p )
{
	_controllers.removeAll( qobject_cast<CqMotorController*>(p) );
}

// =============================== add controller ========================
void CqSimulation::addController( CqMotorController* pController )
{
	Q_ASSERT( pController );
	
	if ( ! _controllers.contains( pController ) )
	{
		_controllers.append( pController );
		connect( pController, SIGNAL(destroyed(QObject*)), SLOT(controllerDestroyed(QObject*)) );
	
		emit motorControllerCreated( pController );
	}
}

// =============================== add ground item ====================
/// Like addsItem(), buts stores item on ground item's list. Does not change the item
void CqSimulation::addGroundItem( CqItem* pItem )
{
	_groundItems.append( pItem );
	addItem( pItem );
}

// ============================= load from XML ======================
void CqSimulation::loadFromXml( const QString& fileName )
{
	CqDocument doc;
	
	doc.loadFromFile( fileName );
	
	CqElement simulation = doc.readElement( ROOT_ELEMENT );
	if ( ! simulation.isNull() )
	{
		load( simulation );
	}
	else
	{
		qWarning("No root element with tag %s", ROOT_ELEMENT );
	}
}

// ============================= save to XML =======================
void CqSimulation::saveToXml( const QString& fileName ) const
{
	CqDocument doc;
	CqElement element = doc.createElement();
	
	store( element );
	doc.appenElement( ROOT_ELEMENT, element );
	
	doc.saveToFile( fileName );
}

// ================================ store ==============================
void CqSimulation::store( CqElement& element ) const
{
	// store discrete items first
	element.appendRectF( TAG_WORLD_RECT, _worldRect );
	element.appendPointF( TAG_GRAVITY, _gravity );
	
	// ground elements
	foreach( CqItem* pGround, _groundItems )
	{
		element.appendItemPointer( TAG_GND_ELEMENT, pGround );
	}
	
	// all CqItem-derrived, top-level items
	QList< QGraphicsItem* > items = _scene.items();
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pCqIem = dynamic_cast<CqItem*>( pItem );
		
		if ( pCqIem && ! pCqIem->physicalParent() )
		{
			element.appendItem( pCqIem );
		}
	}
	
}

// ================================ load =============================
void CqSimulation::load( const CqElement& element )
{
	clear();
	
	// load discrete elements
	_worldRect = element.readRectF( TAG_WORLD_RECT );
	_gravity = element.readPointF( TAG_GRAVITY );
	
	// now - create world
	createWorld();
	
	// populate world with items
	forever
	{
		CqItem* pItem = element.readItem();
		if ( ! pItem )
		{
			break;
		}
		
		addItem( pItem );
	}
	
	// read ground intems
	forever
	{
		CqItem* pItem = element.readItemPointer(TAG_GND_ELEMENT);
		if ( ! pItem )
		{
			break;
		}
	
		_groundItems.append( pItem );
	}
}

// =================================== clear =========================
void CqSimulation::clear()
{
	// destroy all scene iterms
	QList<QGraphicsItem *> sceneItems =  _scene.items();
	foreach( QGraphicsItem* pItem, sceneItems )
	{
		delete pItem;
	}
	
	// destroy world
	delete _pPhysicalWorld;
	_pPhysicalWorld = NULL;
	
	// clear lists
	_controllers.clear();
	_groundItems.clear();
}

// EOF

