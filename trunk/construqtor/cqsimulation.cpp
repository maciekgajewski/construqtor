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


// box2d
#include "b2World.h"

// local
#include "cqsimulation.h"
#include "cqphysicalbox.h"

// constants
static const int SIMULATION_INTERVAL	= 100;	// [ms]
static const double B2D_SPS				= 30.0;	// Box2D simulation steps per second

// ===================== constructor =================
CqSimulation::CqSimulation(QObject* parent): QObject(parent)
{
	// init
	InitWorld();
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
	_simulationTimer.start( SIMULATION_INTERVAL );
}

// ======================== stop ==================
void CqSimulation::stop()
{
	_simulationTimer.stop();
}

// ======================== isRunning ==================
bool CqSimulation::isRunning() const
{
	return _simulationTimer.isActive();
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
		CqPhysicalBody* pBody = qgraphicsitem_cast<CqPhysicalBody*>( pItem );
		
		if ( pBody )
		{
			pBody->simulationStep();
		}
	}
}

// =========================== timer timeout =============
void CqSimulation::InitWorld()
{
	// TODO define outside
	
	// world size spec
	b2AABB worldAABB;
	worldAABB.minVertex.Set(-10.0, -10.0);
	worldAABB.maxVertex.Set(10.0, 10.0);
	
	// gravity
	b2Vec2 gravity(0.0, -10.0);
	
	// create world
	_pPhysicalWorld = new CqWorld( worldAABB, gravity, true /* do sleep*/, this );
	
	// add sample objects
	CqPhysicalBox* pBox = new CqPhysicalBox( NULL, _pPhysicalWorld );
	pBox->setSize( QSizeF( 2.0, 0.3) );
	_scene.addItem( pBox );
	
	CqPhysicalBox* pBox2 = new CqPhysicalBox( NULL, _pPhysicalWorld );
	pBox2->setSize( QSizeF( 3.0, 0.2 ) );
	pBox2->setPos( 1.0, 1.0 );
	_scene.addItem( pBox2 );
	
	// set scene transform to display things right
}

// =========================== assure objects created =============
void CqSimulation::assurePhysicalObjectsCreated()
{
	// first - create bodies
	// TODO suboptimal - maintain separate list of bodyless-objects
	QList< QGraphicsItem* > items = _scene.items();
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqPhysicalBody* pBody = qgraphicsitem_cast<CqPhysicalBody*>( pItem );
		
		if ( pBody )
		{
			pBody->assureBodyCreated();
		}
	}
}

