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
#include <QApplication>

#include "mainwindow.h"
#include "gamemanager.h"
#include "cqsimulation.h"
#include "difficultyselector.h"

int main(int argc, char *argv[])
{
	// initrandom generator
	qsrand( time(NULL) );
	
	QApplication app(argc, argv);
	MainWindow window;
	CqSimulation simulation;
	GameManager manager;
	DifficultySelector selector;
	
	manager.setSimulation( &simulation );
	// select difficulty
	int d = selector.execute();
	switch( d )
	{
		case 1:
			manager.startEasyGame();
			break;
		case 2:
			manager.startIntermediateGame();
			break;
		case 3:
			manager.startHardGame();
			break;
		
		default:
		// bye
			return 0;
	}
	
	window.setGame( &manager );
	
	// configure view
	window.view->setSimulation( &simulation );
	//window.view->rotate( 180 );
	window.view->scale( 50, -50 );
	
	window.show();
	
	return app.exec();
}
