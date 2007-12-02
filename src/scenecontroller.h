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
#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include <QObject>

#include "world.h"

/**
	This is a controller (yikes!) which controlls behavior of game scene.
	It holds the wortld object.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class SceneController : public QObject
{
	Q_OBJECT
public:

	/// Tools (work modes )
	enum Tool
	{
		ToolPaintObject			///< Paint (create) new object
		// TODO other tools here
	};

	// construction / destruction
	SceneController( QObject *parent = 0 );
	~SceneController();
	
	// operation
	void setTool( Tool tool );			///< Sets tool used
	Tool tool() const { return _tool; }	///< Gets current tool
	
	/// World object
	const World* world() const { return &_world; };
	
public slots:
	// inputs from view
	void pointDrawed( const QPointF& point );
	void shapeDrawed( const QPolygonF& polygon );

signals:

	void toolChanged();			///< Signal yo views: tool changed
	
private:

	World	_world;		///< Game world
	Tool	_tool;		///< Current tool
	
};

#endif // SCENECONTROLLER_H


// EOF


