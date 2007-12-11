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

#include "mainwindow.h"

// =========================== cosntructor =======================
MainWindow::MainWindow(QWidget *parent)
    : QWidget( parent ), Ui::MainWindow()
{
	setupUi( this );
	simulation = NULL;
	
	connect( view, SIGNAL(pointerPos(double,double)), SLOT(scenePointerPos(double,double)));
}

// =========================== start =======================
void MainWindow::on_buttonStart_clicked()
{
	simulation->start();
}

// =========================== stop =======================
void MainWindow::on_buttonStop_clicked()
{
	simulation->stop();
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

// EOF