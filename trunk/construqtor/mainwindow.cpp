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

#include "cqnail.h"
#include "cqgirder.h"
#include "cqwheel.h"
#include "cqwheelwithengine.h"
#include "controllerwidget.h"
#include "cqbolt.h"
#include "cqstone.h"

#include "mainwindow.h"

// =========================== cosntructor =======================
MainWindow::MainWindow(QWidget *parent)
    : QWidget( parent ), Ui::MainWindow()
{
	setupUi( this );
	_pSimulation = NULL;
	
	connect( view, SIGNAL(pointerPos(double,double)), SLOT(scenePointerPos(double,double)));
}

// =========================== start =======================
void MainWindow::on_buttonStart_clicked()
{
	simulationStarted();
	_pSimulation->start();
}

// =========================== stop =======================
void MainWindow::on_buttonStop_clicked()
{
	_pSimulation->stop();
	simulationPaused();
}

// ================================= on start =============
void MainWindow::simulationStarted()
{
	buttonGirder100->setEnabled(false);
	buttonGirder200->setEnabled(false);
	buttonGirder300->setEnabled(false);
	buttonNail->setEnabled(false);
	buttonWheel40->setEnabled(false);
	buttonWheel80->setEnabled(false);
	buttonWwE->setEnabled(false);
	buttonBolt->setEnabled(false);
	buttonStone->setEnabled(false);
}

// =================================== on stop ============
void MainWindow::simulationPaused()
{
	buttonGirder100->setEnabled(true);
	buttonGirder200->setEnabled(true);
	buttonGirder300->setEnabled(true);
	buttonNail->setEnabled(true);
	buttonWheel40->setEnabled(true);
	buttonWheel80->setEnabled(true);
	buttonWwE->setEnabled(true);
	buttonBolt->setEnabled(true);
	buttonStone->setEnabled(true);
}

// =========================== pointer pos  =======================
void MainWindow::scenePointerPos( double x, double y )
{
	labelPos->setText( QString("%1, %2").arg( x ).arg( y ) );
}

// =========================== girder 200 =======================
void MainWindow::on_buttonGirder100_clicked()
{
	CqGirder* pGirder = new CqGirder( 1.0, 0.2 ); // 100x20 cm
	view->toolAddObject( pGirder );
}

// =========================== girder 200 =======================
void MainWindow::on_buttonGirder200_clicked()
{
	CqGirder* pGirder = new CqGirder( 2.0, 0.2 ); // 200x20 cm
	view->toolAddObject( pGirder );
}

// =========================== girder 200 =======================
void MainWindow::on_buttonGirder300_clicked()
{
	CqGirder* pGirder = new CqGirder( 3.0, 0.2 ); // 300x20 cm
	view->toolAddObject( pGirder );
}

// ============================ nail ==============================
void MainWindow::on_buttonNail_clicked()
{
	CqNail* pNail = new CqNail();
	view->toolAddNail( pNail );
}

// ============================ bolt ==============================
void MainWindow::on_buttonBolt_clicked()
{
	CqBolt* pBolt = new CqBolt();
	view->toolAddNail( pBolt );
}

// ============================ wheel 40  ==============================
void MainWindow::on_buttonWheel40_clicked()
{
	CqWheel* pWheel = new CqWheel( 0.40 );
	view->toolAddObject( pWheel );
}

// ============================ wheel 80  ==============================
void MainWindow::on_buttonWheel80_clicked()
{
	CqWheel* pWheel = new CqWheel( 0.80 );
	view->toolAddObject( pWheel );
}

// ============================= Wheel with engine =======================
void MainWindow::on_buttonWwE_clicked()
{
	CqWheelWithEngine* pWwE = new CqWheelWithEngine( 0.80 );
	view->toolAddObject( pWwE );
}

// ============================= controller cretaed ========================
void MainWindow::controllerCreated( CqMotorController* pCtrl )
{
	ControllerWidget* pCW = new ControllerWidget( boxControllers );
	boxControllers->layout()->addWidget( pCW );
	pCW->setController( pCtrl, _pSimulation );
	pCW->show();
	
	
}

// ========================== set simulation ==================================
void MainWindow::setSimulation( CqSimulation* pSimulation )
{
	Q_ASSERT( pSimulation );
	
	_pSimulation = pSimulation;
	
	connect( pSimulation
		, SIGNAL(motorControllerCreated(CqMotorController*))
		, SLOT(controllerCreated(CqMotorController*))
		);
}

// =========================== stone ================================
void MainWindow::on_buttonStone_clicked()
{
	CqStone* pStone = CqStone::createRandomStone( 0.80 );
	view->toolAddObject( pStone );
}

// EOF

