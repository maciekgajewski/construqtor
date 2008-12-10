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
#ifndef CQITEM_H
#define CQITEM_H

// Qt
#include <QGraphicsItem>
#include <QObject>
#include <QUuid>

// local
#include "cqelement.h"
class CqSimulation;
class CqWorld;
class CqPhysicalBody;

/**
	Common subclass for all simulated items. Provides functonality to communicate
	with simulation.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqItem : public QObject, public QGraphicsItem
{
	Q_OBJECT
public:

	/// Flags describing editing this item
	enum EditorFlag {
		Selectable		=	0x1,			///< Item can be selected
		Movable			=	0x2,			///< Item can be moved
		Rotatable		=	0x4				///< Item can be rotated
	};
	
	/// Collision group flag. Items collide only when have same flag set
	enum CollisionGroup
	{
		CollisionConstruction1		= 0x0001,				///< construction elements - group 1
		CollisionConstruction2		= 0x0002,				///< construction elements - group 2
		
		CollisionCylinder			= 0x0004,				///< cylinder's piston
		
		CollisionConstruction		= 0x0003,				///< collides with all construction items
		CollisionAll				= 0xffff
	}; // TODO definde Q_FLAG
	
	// constrution / destruction
	explicit CqItem( CqItem* parent = NULL );
	virtual ~CqItem();
	
	
	/// Blocks access to underlying QObject::setParent()
	void setParent( QObject* ){Q_ASSERT(false);}
	
	// signals form simulation
	virtual void simulationStep();						///< Called on simulation step
	virtual void calculationStep(){};					///< Called on low-level box2d simulatio step
	virtual void simulationStarted();					///< Caled when simulatio is started
	virtual void simulationStopped();					///< Caled when simulatio is started
	
	// properties
	virtual void setSimulation( CqSimulation* pSimulation ) { _pSimulation = pSimulation; }
	CqSimulation* simulation() { return _pSimulation; }
	const CqSimulation* simulation() const { return _pSimulation; }
	
	virtual void setWorld ( CqWorld* pWorld );			///< Sets world
	CqWorld* world() { return _pWorld; }				///< Returns world
	
	virtual void setRotationRadians( double radians );	///< sets rotation in radians
	virtual double rotationRadians() const { return _rotation; } ///< Retuens rotation
	
	int editorFlags() const { return _flags; }				///< Retuns flags
	void setEditorFlags( int flags ) { _flags = flags; }	///< Sets flags
	
	virtual void setSelected( bool selected );
	bool selected() const { return _selected; }
	
	virtual QString description() const { return _name;} 		///< Object description
	
	QUuid id() const { return _id; }					///< Unique id
	void setId( const QUuid& id ) { _id = id; }			///< Sets id
	virtual void generateNewId();						///< Generates new, unique id
	
	void setCollisionGroup( int cd ){ _collisionGroup = cd; }
	int collisionGroup() const { return _collisionGroup; }
	
	// mapping coordinates 
	QPointF mapToWorld( const QPointF& local );			///< Maps local to physical scene
	double	mapToWorld( double rotation );				///< Maps local rotation to [husical
		
	QPointF mapFromWorld( const QPointF& physical );	///< Maps pos from physical to local
	double mapFromWorld( double rotation );				///< Maps rotation from physical to local
	
	QPointF worldPos() const;							///< item positon in world's coordsa
	double worldRotation() const;						///< item rotation in world coords
	void setWorldPos( const QPointF pos );
	void setWorldRotation( double rotation );
	void setName( const QString& name ) { _name = name; }
	QString name() const { return _name; }
	
	// info from editor
	virtual void setPhysicalRotation( double radians );		///< Rotates item
	virtual void setPhysicalPos( const QPointF& pos );		///< changes item position
	virtual void breakConections(){};						///< breaks all conections - joints etc
	
	// Editability info. Reimplement only if really custom beahvior needed
	virtual bool canBeSelected() const;
	virtual bool canBeMoved() const;
	virtual bool canBeMovedHere( const QPointF& scenePos );
	virtual bool canBeRotated() const;
	
	// geometrical properties
	
	QPointF center() const { return _center; }			///< Local center
	void setCenter( const QPointF& c ){ _center = c; }
	
	QPointF centerRotated() const;						//!< Local center ,rotated y rotation	
	void setCenterRotated( const QPointF& c );			//!< Sets center in rotated coords
	
	/// If connection (nail/bolt/...) can be attached at this point
	virtual bool canConnectHere( const QPointF& /*worldPoint*/ ) { return false; }
	/// Physical body connected to joint in this location
	virtual CqPhysicalBody* bodyHere( const QPointF& /*worldPoint*/ ) { return NULL; }
	
	// child / parent relationship
	CqItem* physicalParent(){ return _pPhysicalParent; }
	const CqItem* physicalParent() const { return _pPhysicalParent; }
	void setPhysicalParent( CqItem* pParent );		///< Sets physical parent
	virtual void childChanged( CqItem* ){};			///< Info from child - child changed
	virtual void childDeleted( CqItem* ){}			///< Info form chi;ld - deleted
	virtual void notifyParent();					///< Set info to parent
	
	virtual void updatePhysicalPos(){};				///< Updates physical pos to item pos/rotation
	virtual void updatePosToPhysical(){};			///< Updates item pos to physical pos (after simulation step)
	
	
	// storing / reading
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 
	
protected:
	
	// input handlers
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	
	// data
	
	double			_rotation;							///< Rotation
	
private:

	// methods
	
	void init();
	
	// data
	
	CqSimulation*	_pSimulation;						///< Simulation
	CqWorld*		_pWorld;							///< Physical world
	int				_flags;								///< Behavior flags
	bool			_selected;							///< Selected
	CqItem*			_pPhysicalParent;					///< Parent item
	QPointF			_center;							///< Local center, rotation axis, COG
	QString			_name;								///< Name
	QUuid			_id;								///< Unique ID

	int			_collisionGroup;			///< Body's collision group

};

#endif // CQITEM_H

// EOF


