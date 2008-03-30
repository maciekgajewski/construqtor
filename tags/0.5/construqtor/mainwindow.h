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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// local
#include "cqsimulation.h"
#include "ui_mainwindow.h"
class CqMotorController;
class GameManager;

class MainWindow: public QWidget, public Ui::MainWindow
{
Q_OBJECT
public:
	MainWindow( QWidget *parent = 0 );
	
	void setGame( GameManager* pManager );
	
public slots:
	
	// buttons - start/stop
	
	void on_buttonStart_clicked();
	void on_buttonStop_clicked();
	
	// save/load
	void on_buttonSave_clicked();
	void on_buttonLoad_clicked();
	
	// buttons - tools
	void on_buttonDelete_clicked();
	void on_buttonBreak_clicked();
	void on_buttonClone_clicked();
	
	// buttons - elements
	
	void on_buttonGirder100_clicked();
	void on_buttonGirder200_clicked();
	void on_buttonGirder300_clicked();
	void on_buttonRod50_clicked();
	void on_buttonRod100_clicked();
	void on_buttonRod150_clicked();
	
	void on_buttonWheel40_clicked();
	void on_buttonWheel80_clicked();
	void on_buttonWheel120_clicked();
	
	void on_buttonNail_clicked();
	void on_buttonBolt_clicked();
	
	void on_buttonWwE_clicked();
	void on_buttonWwE40_clicked();
	void on_buttonWwE120_clicked();
	
	void on_buttonStone_clicked();
	void on_buttonPalett_clicked();
	
	void on_buttonCylinder100_clicked();
	void on_buttonCylinder50_clicked();
	void on_buttonCylinder100d15_clicked();
	void on_buttonCylinder50d15_clicked();
	
	void on_buttonFork_clicked();
	void on_buttonCap20_clicked();
	void on_buttonAngle4020_clicked();
	void on_buttonTriangle4040_clicked();
	
	void on_buttonWeight90_clicked();
	void on_buttonWeight360_clicked();
	void on_buttonWeight810_clicked();
	
	void scenePointerPos( double x, double y );
	
	void simulationStarted();
	void simulationPaused();
	
	// signals from simulation
	void controllerCreated( CqMotorController* pCtrl );
	void selectedDescription( const QString& description );
	
private:
	CqSimulation*	_pSimulation;
	GameManager*	_pGameManager;
};

#endif
