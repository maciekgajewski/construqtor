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
#ifndef WORLDQT_H
#define WORLDQT_H

#include <QObject>
#include <QTimer>

#include "b2World.h"

#include "physicalobject.h"

/**
	Wrapper around Box2d's world object

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
	
*/
class World : public QObject
{
	Q_OBJECT
public:

	World( QObject *parent = 0 );
	~World();
	
	void addObject( const QPolygonF& outline );		///< Adds object to the world
	
	// tools
	static QPolygonF simplifyPolygon( const QPolygonF& input );
	
	// proeprties
	const QList< PhysicalObject >& objects() const { return _objects; }
	b2World*  b2world(){ return _pWorld; }
	
	
private:

	QList< PhysicalObject > _objects;				///< List of worl'd objects
	
	//. box2d object
	b2World*	_pWorld;							///< Box2D's world
	
	// TODO tmporary, will be moved outside
	QTimer		animateTimer;						///< Animate timer

private slots:

	void animate();								///< Aimate timer timeout

signals:

	void animated();								///< World was just animated
};

#endif // WORLDQT_H

// EOF
