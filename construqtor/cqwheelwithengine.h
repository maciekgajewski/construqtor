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
#ifndef CQWHEELWITHENGINE_H
#define CQWHEELWITHENGINE_H

// Local
#include "cqitem.h"
#include "cqphysicalbox.h"
#include "cqphysicaldisk.h"

/**
	 complex item. Consist of 'motor' and 'wheel' conected wityh motorized
	 revolute joint.
	 
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqWheelWithEngine : public CqItem
{
public:
	CqWheelWithEngine( double wheelDiameter );
	virtual ~CqWheelWithEngine();
	
	// properties
	virtual void setWorld ( CqWorld* pWorld );				///< Sets world
	
	virtual void setRotationRadians( double radians );		///< Rotates item
	virtual double rotationRadians() const;				 	///< Retuens rotation
	
	// info from editor
	virtual void setPhysicalRotation( double radians );		///< Rotates item
	virtual void setPhysicalPos( const QPointF& pos );		///< changes item position
	
	// editability info
	virtual bool canBeSelected() const { return true; }
	virtual bool canBeMoved() const;
	virtual bool canBeRotated() const;
	
	/// If connection (nail/bolt/...) can be attached at this point
	virtual bool canConnectHere( const QPointF& scenePoint );
	/// Physical body connected to joint in this location
	virtual CqPhysicalBody* bodyHere( const QPointF& scenePoint );

	
private:

	// sub - elements
	class Engine : public CqPhysicalBox
	{
		Engine( CqWheelWithEngine* parent );
		virtual ~Engine(){}
		
		// operations
		virtual void paint
			( QPainter * painter
			, const QStyleOptionGraphicsItem * option
			, QWidget * widget = 0 );
		
	};
	
	class Wheel : public CqPhysicalDisk
	{
		Wheel( CqWheelWithEngine* parent );
		virtual ~Wheel(){}
		
		// operations
		virtual void paint
			( QPainter * painter
			, const QStyleOptionGraphicsItem * option
			, QWidget * widget = 0 );
	};

	// methods
	
	void init();				///< Init
	
	// data
	
	Engine	_engine;			///< Engine sub-element
	Wheel	_wheel;				///< Wheel sub-element
	
	CqRevoluteJoint	_joint;		///< Joint
	

};

#endif // CQWHEELWITHENGINE_H

// EOF


