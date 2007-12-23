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
#ifndef CQSIMULATION_H
#define CQSIMULATION_H

// qt
#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QDomElement>

// box2d
class b2World;

// local
class CqItem;
class CqMotorController;
#include "cqworld.h"

/**
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqSimulation : public QObject
{
Q_OBJECT
public:

	// constructoon / destruction
	CqSimulation(QObject* parent = NULL);
	~CqSimulation();

	// operations
	void start();			///< starts simulation
	void stop();			///< Stops simulation
	bool isRunning() const;	///< Is simulation running?
	
	QGraphicsScene* scene() { return &_scene; };
	const QGraphicsScene* scene() const { return &_scene; };
	
	void addItem( CqItem* pItem );		///< Adds item do simulation
	void addGroundItem( CqItem* pItem );///< Adds ground item to simulation
	QList<CqItem*> groundItems() const { return _groundItems; }
	
	
	// editor control
	bool canBeSelected( const CqItem* pItem ) const;	///< If itemcan be selected
	bool canBeMoved( const CqItem* pItem ) const;		///< If item can be moved
	/// If moved item can be moved into this position
	bool canBeMovedHere( const CqItem* pItem, const QPointF& pos ) const;
	bool canBeRotated( const CqItem* pItem ) const;		///< If item can be rotated
	/// Can item be added here now?
	bool canAddHere( const CqItem* pItem, const QPointF& pos ) const;
	
	void addController( CqMotorController* pController );	///< adds controler ot controller list
	
	QRectF worldRect() const { return _worldRect; }
	
	double invTimeStep() const;						///< Returns time step [1/s]
	
	// XML storing / reading
	void loadFromXml( const QString& fileName );		///< loads from XML
	void saveToXml( const QString& fileName ) const;	///< saves to XML
	
	virtual QDomElement toXml() const;					///< stores item state as XML. To be reimplemented
	virtual void fromXml( const QDomElement& element );	///< restores item state from XML. To be reimplemented
	

signals:

	void simulationStarted();
	void simulationPaused();
	
	void simulationStep();
	
	void motorControllerCreated( CqMotorController* );

private slots:

	void simulationTimerTimeout();	///< Timer handler
	void controllerDestroyed( QObject* p );	///< removes destroyed

private:

	// methods
	
	void initWorld();				///< Intiializes physical world
	/// Makes sure all children have they physical counterparts created for simulation
	void assurePhysicalObjectsCreated();

	// data

	CqWorld*		_pPhysicalWorld;		///< Physical world
	QTimer			_simulationTimer;		///< Simulation timer
	QGraphicsScene	_scene;					///< Simulation scene
	QList<CqMotorController*>	_controllers;	///< Set of motor controllers
	QRectF			_worldRect;				///< world rectangle
	QList<CqItem*>	_groundItems;			///< List of ground bodies
};

#endif
