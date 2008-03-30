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

#include "b2Body.h"

#include "world.h"

// ======================== constructior ===============
World::World(QObject *parent)
 : QObject(parent)
{
	// TODO pass world size, gravity and ground size in params
	
	// world size spec
	b2AABB worldAABB;
	worldAABB.minVertex.Set(-100.0, -100.0);
	worldAABB.maxVertex.Set(100.0, 100.0);
	
	// gravity
	b2Vec2 gravity(0.0, -10.0);
	
	// create world
	_pWorld = new b2World( worldAABB, gravity, true /* do sleep*/ );
	
	// create ground
	b2BoxDef groundBoxDef;
	groundBoxDef.extents.Set(100.0, 5.0);
	groundBoxDef.density = 0.0;
	
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0, -5.0);
	groundBodyDef.AddShape(&groundBoxDef);
	
	b2Body* pGround = _pWorld->CreateBody(&groundBodyDef);
	
	// animate timer
	connect( &animateTimer, SIGNAL(timeout()), SLOT(animate() ) );
}

// ======================== destructor ===============
World::~World()
{
}

// ========================= add object =================
void World::addObject( const QPolygonF& outline, const Material& material  )
{
	// step one: simplify outline
	QPolygonF simplified = simplifyPolygon( outline );
	
	PhysicalObject object( simplified, this, material );
	_objects.append( object );
}

// ================== simplify polygon ===========
// TODO remove the method from here
QPolygonF World::simplifyPolygon( const QPolygonF& input )
{
	// params: max acceptable angle
	double MIN_DISTANCE	= 0.05;	// min distance between ppoints - in pixels
	
	
	// stupid copy of simple polygon 
	if ( input.size() < 4 )
	{
		return input;
	}
	
	// do some simplification - simple suboptimal algoritmh
	QPolygonF output;
	
	// first point goes always
	output.append( input[ 0 ] );
	
	// second poitn is first candidate 
	QPointF candidate = input[ 1 ];
	
	// calculate distance and angle
	double angle = atan2( candidate.y() - output.last().y(), 
						candidate.x() - output.last().x() );
	// may be reduced due to distance?
	// seek for next candidates
	for( int i = 2; i < input.size() - 1 ; i++ )
	{
		double dy = input[i].y() - output.last().y();
		double dx = input[i].x() - output.last().x();
		double distance = sqrt( dx*dx + dy*dy );
		double currentAngle = atan2( input[i].y() - output.last().y(),
									input[i].x() - output.last().x() );
	
		if 	(
				( distance  < MIN_DISTANCE )
			||	( ( distance * fabs( angle - currentAngle ) ) < MIN_DISTANCE )
			)
		{
			// ok, reduced
			candidate = input[i];
		}
		else
		{
			// no, it's new point
			output.append( candidate );
			candidate = input[i];
			angle = atan2( candidate.y() - output.last().y(), 
						candidate.x() - output.last().x() );
		}
	}
	// last point always comes in
	output.append( input.last() );
	
	// no simplification
	return output;
}

// ======================== animate timer ====================
// TODO should be moved outside
void World::animate()
{
	if ( _pWorld )
	{
		_pWorld->Step( 1.0/30.0, 3 );
	}
	
	emit animated();
}

// ======================== start simulation ==========
void World::startSimulation()
{
	animateTimer.start( 100 );
}

// ======================== stop simulation ==========
void World::stopSimulation()
{
	animateTimer.stop();
}

// ======================== is started ==========
bool World::isSimulationStarted() const
{
	return animateTimer.isActive();
}

// ======================== destroy bodies at point ==========
void World::destroyBodiesAtPoint( const QPointF& point )
{
	// get shapes under point
	b2AABB aabb;
	double prec = 0.01; // one dm precision
	
	aabb.minVertex.Set( point.x() - prec, point.y() - prec );
	aabb.maxVertex.Set( point.x() + prec, point.y() + prec );
	
	const int bufferSize = 10;
	b2Shape *buffer[bufferSize];
	
	int count = _pWorld->Query(aabb, buffer, bufferSize);
	
	// build list of objects
	QList< b2Body* > bodies;
	for (int i = 0; i < count; i++)
	{
		b2Body* pBody = buffer[ i ]->GetBody();
		if ( ! bodies.contains( pBody ) )
		{
			bodies.append( pBody );
		}
	}
	
	// remove appropriate physical objects from list
	for ( QList< PhysicalObject >::iterator it = _objects.begin(); it != _objects.end(); )
	{
		// should be removed?
		b2Body* pBody = (*it).b2body();
		if ( bodies.contains( pBody ) )
		{
			bodies.removeAll( pBody );
			it = _objects.erase( it );
			_pWorld->DestroyBody( pBody );
		}
		else
		{
			++it;
		}
	}
	
}



// EOF
