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

// box2d
class b2World;

// local
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

private slots:

	void simulationTimerTimeout();	///< Timer handler

private:

	// methods
	
	void InitWorld();				///< Intiializes physical world
	/// Makes sure all children have they physical counterparts created for simulation
	void assurePhysicalObjectsCreated();

	// data

	CqWorld*		_pPhysicalWorld;		///< Physical world
	QTimer			_simulationTimer;		///< Simulation timer
	QGraphicsScene	_scene;					///< Simulation scene
};

#endif
