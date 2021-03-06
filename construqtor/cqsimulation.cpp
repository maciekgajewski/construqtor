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
static const char* TAG_EDITABLE_AREA	= "editablearea";
static const char* TAG_TARGET_AREA		= "targetarea";


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
	clear(); // destroty items in civilized way
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
	Q_ASSERT( _pPhysicalWorld );
	
	// physical world simulation step
	int iterations = B2D_SPS * SIMULATION_INTERVAL / 1000.0;
	for( int i = 0; i < iterations; i++)
	{
		_pPhysicalWorld->Step( 1.0/B2D_SPS, 10 ); // NOTE 10: this is experimental param value
		
		// update all items
		QList< QGraphicsItem* > items = _scene.items();
		foreach( QGraphicsItem* pItem, items )
		{
			CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
			
			if ( pCqItem && pCqItem->physicalParent() == NULL ) // only top-level items
			{
					pCqItem->calculationStep();
			}
		}
		emit calculationStep();
	}
	
	// update all items
	QList< QGraphicsItem* > items = _scene.items(); // NOTE uses sepearte list than loop above, as the loop aboce may delete/add some items
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		
		if ( pCqItem && pCqItem->physicalParent() == NULL ) // only top-level items
		{
			pCqItem->simulationStep();
		}
	}
	
	emit simulationStep();
}

// ============================== init =============================
void CqSimulation::init()
{
	_worldRect	= QRect( -250, -100, 500, 200 ); // 500x200 m
	_gravity	= QPointF( 0.0, -10.0 );
	_pEditableAreaItem	= NULL;
	_pTargetAreaItem	= NULL;

	
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
/// Intiialzies scene with some default values
void CqSimulation::initScene()
{
	/* TODO moved out, to game manager
	CqGroundBody *pGround = CqGroundBody::randomGround( this, 0.2 );
	addGroundItem( pGround );
	adjustEditableAreasToGround();
	*/
}

// =====================================================================
void CqSimulation::adjustEditableAreasToGround()
{
	//TODO remove
	/*
	CqGroundBody* pGround = qobject_cast< CqGroundBody* > ( _groundItems.first() );
	if ( pGround )
	{
		// assuming we are going from left to right
		
		double heightAtBegining = pGround->height( _worldRect.left() );
		double heightAtEnd = pGround->height( _worldRect.right() );
		
		QSizeF areaSize = QSizeF( 20.0, 20.0 );	// 20x20 m. keep it in sync with value in CqGroundBody::randomGround
	
		_editableArea	= QRectF( QPointF( _worldRect.left() + 1.0, heightAtBegining - 1.0 ), areaSize );
		_targetArea		= QRectF( QPointF( _worldRect.right() - 1.0 - areaSize.width(), heightAtEnd - 1.0 ), areaSize );
	
		updateAreaItems();
	}
	*/
}

// =====================================================================
void CqSimulation::updateAreaItems()
{
	// delete items, create new ones
	delete _pEditableAreaItem;
	delete _pTargetAreaItem;
	
	_pEditableAreaItem = _scene.addRect
		( _editableArea
		, QPen( QColor(10, 244, 205, 128), 0.15, Qt::DashLine )
		//, QColor( 182, 244, 234, 64 )
		, QBrush() // use clear brush instead of coloured - faster
		);
		
	_pTargetAreaItem = _scene.addRect
		( _targetArea
		, QPen( QColor(80, 252, 149, 128), 0.15, Qt::DashLine )
		//, QColor( 182, 252, 210, 64 )
		, QBrush() // use clear brush instead of coloured - faster
		);
		
	_pEditableAreaItem->setZValue( 20.0 );
	_pTargetAreaItem->setZValue( 20.0 );
}

// =========================== assure objects created =============
void CqSimulation::assurePhysicalObjectsCreated()
{
	Q_ASSERT( _pPhysicalWorld );
	
	// TODO suboptimal - maintain separate list of bodyless-objects
	QList< QGraphicsItem* > items = _scene.items();
	
	// first - create bodies
	foreach( QGraphicsItem* pItem, items )
	{
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		
		if ( pBody )
		{
			pBody->setWorld( _pPhysicalWorld );
			pBody->assureBodyCreated();
		}
	}
	
	// second - create joints
	foreach( QGraphicsItem* pItem, items )
	{
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		
		if ( pJoint )
		{
			pJoint->setWorld( _pPhysicalWorld );
			pJoint->assureJointCreated();
		}
	}
}

// =============================== add item =========================
void CqSimulation::addItem( CqItem* pItem )
{
	Q_ASSERT( pItem );
	
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
bool CqSimulation::canBeMoved( const CqItem* pItem ) const
{
	Q_ASSERT( pItem );
	
	return ! isRunning() && isInEditableArea( pItem->worldPos() );
}

// ============================== can be moved here ? =====================
bool CqSimulation::canBeMovedHere( const CqItem* /*pItem*/, const QPointF& pos ) const
{
	return isInEditableArea( pos ); // TODO check if not under ground
}

// ================================ can be rotated =======================
bool CqSimulation::canBeRotated( const CqItem* pItem ) const
{
	Q_ASSERT( pItem );
	
	return ! isRunning() && isInEditableArea( pItem->worldPos() );
}
// ============================ can add here ? ======================
// Can item be added here now?
bool CqSimulation::canAddHere( const CqItem* /*pItem*/, const QPointF& pos ) const
{
	if ( ! isRunning() && _worldRect.contains( pos ) && isInEditableArea( pos ) )
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
	element.appendRectF( TAG_EDITABLE_AREA, _editableArea );
	element.appendRectF( TAG_TARGET_AREA, _targetArea );
	
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
	_worldRect		= element.readRectF( TAG_WORLD_RECT );
	_gravity		= element.readPointF( TAG_GRAVITY );
	_editableArea	= element.readRectF( TAG_EDITABLE_AREA );
	_targetArea		= element.readRectF( TAG_TARGET_AREA );
	
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
	
	updateAreaItems();
}

// =================================== clear =========================
void CqSimulation::clear()
{
	// TODO why not use _scene.clear() ?
	
	
	_scene.clear();
	
	// destroy all top-level items
	/*
	QList<QGraphicsItem *> sceneItems =  _scene.items();
	QList<QGraphicsItem *> topLevelItems; 
	foreach( QGraphicsItem* pItem, sceneItems )
	{
		if ( ! pItem->parentItem() )
		{
			topLevelItems.append( pItem );
		}
	}
	foreach( QGraphicsItem* pItem, topLevelItems )
	{
		delete pItem;
	}
	*/
	
	
	// destroy world
	delete _pPhysicalWorld;
	_pPhysicalWorld = NULL;
	
	// clear lists
	_controllers.clear();
	_groundItems.clear();
	
	// clear area items (was deleted above)
	_pEditableAreaItem = NULL;
	_pTargetAreaItem = NULL;
}
// =================================== run =========================
/// Runs - synchronously and at full processor speed - specified simulation time.
void CqSimulation::run( double timeSpan )
{
	int steps = timeSpan * invTimeStep();
	
	// prepare
	if ( ! _pPhysicalWorld )
	{
		createWorld();
	}
	
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
	
	// simualate
	for( int i =0; i < steps; i++ )
	{
		simulationTimerTimeout();
	}
	
	// tell everyone simulation has stopped
	items = _scene.items();
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pBody = dynamic_cast<CqItem*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStopped();
		}
	}
	
}

// EOF

