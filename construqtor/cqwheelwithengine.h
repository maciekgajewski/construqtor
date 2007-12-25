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
#include "cqgirder.h"
#include "cqwheel.h"
#include "cqrevolutejoint.h"
#include "cqcompounditem.h"
#include "cqrevolutevelocitycontroler.h"

/**
	 Compound item. Consist of 'motor' and 'wheel' conected wityh motorized
	 revolute joint.
	 
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqWheelWithEngine : public CqCompoundItem
{
	Q_OBJECT
public:
	CqWheelWithEngine( double wheelDiameter );
	virtual ~CqWheelWithEngine();
	
	/// If connection (nail/bolt/...) can be attached at this point
	virtual bool canConnectHere( const QPointF& worldPoint );
	/// Physical body connected to joint in this location
	virtual CqPhysicalBody* bodyHere( const QPointF& scenePoint );
	
	/// Extends base implementation by adding controller to simulation
	virtual void setSimulation( CqSimulation* pSimulation );

private:

	class Motor : public CqRevoluteJoint
	{
		public:
		
		Motor( CqWheelWithEngine* parent = 0 );
		virtual ~Motor(){}
		
		// operations
		virtual void paint
			( QPainter * painter
			, const QStyleOptionGraphicsItem * option
			, QWidget * widget = 0 );
			
		virtual QRectF boundingRect() const;
	
		
	};

	// methods
	
	void init();				///< Init
	
	// data
	
	CqGirder*		_pEngine;		///< Engine sub-element
	CqWheel*		_pWheel;		///< Wheel sub-element
	Motor*			_pMotor;		///< Motorized revolute joint
	
	CqRevoluteVelocityControler _controller;	///< Motor controller
	
	double _wheelDiameter;
};

#endif // CQWHEELWITHENGINE_H

// EOF


