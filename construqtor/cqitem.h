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

/**
	Common subclass for all simulated items. Provides functonality to communicate
	with simulation.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqItem : public QGraphicsItem
{
public:
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
	
	virtual void setPhysicalPos( const QPointF& pos );		///< changes item position
	
	// editability info
	virtual bool canBeSelected() const { return false; }
	virtual bool canBeMoved() const { return false; }
	virtual bool canBeMovedHere( const QPointF& scenePos );
	
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
};

#endif // CQITEM_H

// EOF


