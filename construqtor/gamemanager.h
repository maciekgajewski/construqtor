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
#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

// Qt
#include <QObject>

// Cq
class CqSimulation;

/**
	Initializes and mabages game simulation.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class GameManager : public QObject
{
Q_OBJECT
public:
	GameManager(QObject *parent = 0);
	~GameManager();
	
public slots:

	void startEasyGame( CqSimulation* pSim );
	void startIntermediateGame( CqSimulation* pSim );
	void startHardGame( CqSimulation* pSim );
	
private:

	void startGame( CqSimulation* pSim, double maxSlope, double stoneSize, int stones );
};

#endif // GAMEMANAGER_H

// EOF


