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
#ifndef CQWORLD_H
#define CQWORLD_H

// qt
#include <QObject>

// box2d
#include "b2World.h"

/**
	Qt Wrapper around b2d World.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqWorld : public QObject, public b2World
{
	Q_OBJECT
public:
	
	// constructor/destructor
	CqWorld( const b2AABB& worldAABB, const b2Vec2& gravity, bool doSleep, QObject* parent = NULL );
	~CqWorld();

private:
};


#endif // CQWORLD_H


// EOF


