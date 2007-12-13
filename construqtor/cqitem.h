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
#ifndef CQITEM_H
#define CQITEM_H

// Qt
#include <QGraphicsItem>

// local
class CqSimulation;
class CqWorld;
class CqPhysicalBody;

/**
	Common subclass for all simulated items. Provides functonality to communicate
	with simulation.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqItem : public QGraphicsItem
{
public:

	// flags
	enum ItemFlag {
		Selectable		=	0x1,			//< Item can be selected
		Movable			=	0x2,			//< Item can be moved
		Rotatable		=	0x4				//< Item can be rotated
	};
	
	// constrution / destruction
	CqItem( QGraphicsItem* parent = NULL );
	virtual ~CqItem();
	
	
	// signals form simulation
	virtual void simulationStep();						///< Called after simulation step
	virtual void simulationStarted();					///< Caled when simulatio is started
	virtual void simulationStopped();					///< Caled when simulatio is started
	
	// properties
	void setSimulation( CqSimulation* pSimulation ) { _pSimulation = pSimulation; }
	CqSimulation* simulation() { return _pSimulation; }
	const CqSimulation* simulation() const { return _pSimulation; }
	
	virtual void setWorld ( CqWorld* pWorld );			///< Sets world
	CqWorld* world() { return _pWorld; }				///< Returns world
	
	virtual void setRotationRadians( double radians );	///< sets rotation in radians
	virtual double rotationRadians() const { return _rotation; } ///< Retuens rotation
	
	int cqFlags() const { return _flags; }				///< Retuns flags
	void setCqFlags( int flags ) { _flags = flags; }	///< Sets flags
	
	// mapping coordinates 
	QPointF mapToPhysical( const QPointF& local );		///< Maps local to physical scene
	double	mapToPhysical( double rotation );			///< Maps local rotation to [husical
		
	QPointF mapFromPhysical( const QPointF& physical );	///< Maps pos from physical to local
	double mapFromPhysical( double rotation );			///< Maps rotation from physical to local
	
	// info from editor
	virtual void setPhysicalRotation( double radians );		///< Rotates item
	virtual void setPhysicalPos( const QPointF& pos );		///< changes item position
	
	// Editability info. Reimplement only if really custom beahvior needed
	virtual bool canBeSelected() const;
	virtual bool canBeMoved() const;
	virtual bool canBeMovedHere( const QPointF& scenePos );
	virtual bool canBeRotated() const;
	
	// geometrical properties
	
	/// If connection (nail/bolt/...) can be attached at this point
	virtual bool canConnectHere( const QPointF& scenePoint ) { return false; }
	/// Physical body connected to joint in this location
	virtual CqPhysicalBody* bodyHere( const QPointF& scenePoint ) { return NULL; }
	
	// child / parent relationship
	QList< CqItem* > physicalChildren();			///< Return list of CqItem children
	CqItem* physicalParent();						///< return CqItem parent
	const CqItem* physicalParent() const;			///< return CqItem parent
	
	virtual void updatePhysicalPos();				///< Updates physical pos to item pos/rotation
	
protected:
	
	// input handlers
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	
private:

	// methods
	
	void init();
	
	// data
	
	CqSimulation*	_pSimulation;						///< Simulation
	CqWorld*		_pWorld;							///< Physical world
	double			_rotation;							///< Rotation
	int				_flags;								///< Behavior flags

};

#endif // CQITEM_H

// EOF


