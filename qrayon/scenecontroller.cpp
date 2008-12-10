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
#include "scenecontroller.h"

// ========================= constructor ====================
SceneController::SceneController(QObject *parent)
 : QObject(parent)
{
	_tool		= ToolPaintObject;
	_material	= Material::rubber();
}

// ========================= destructor ====================
SceneController::~SceneController()
{
}

// ========================= set tool ====================
void SceneController::setTool( Tool tool )
{
	if ( tool != _tool )
	{
		_tool = tool;
		emit toolChanged();
	}
}

// ========================= point drawed ====================
void SceneController::pointDrawed( const QPointF& point )
{
	Q_UNUSED(point)
	// TODO
}

// ========================= shape drawed ====================
void SceneController::shapeDrawed( const QPolygonF& polygon )
{
	// TODO handle different tools here
	
	// ignore empty
	if ( polygon.size() < 2 )
	{
		// TODO hanlde these somehow
		return;
	}
	
	// TODO move simplification here
	
	// add polygon to world
	_world.addObject( polygon, _material );
}

// ========================= point right-clicked ====================
void SceneController::pointRightClicked( const QPointF& point )
{
	// remove bodies under point
	_world.destroyBodiesAtPoint( point );
}

// EOF


