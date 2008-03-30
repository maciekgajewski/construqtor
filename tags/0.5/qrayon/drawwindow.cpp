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

#include <QGLWidget>

#include "drawwindow.h"

DrawWindow::DrawWindow(QWidget *parent)
    :QWidget(parent)
{
	// duty
	setupUi( this );
	
	_pCtrl = NULL;
}

// ======================== set controller ==============
void DrawWindow::setController( SceneController* pCtrl )
{
	_pCtrl = pCtrl;
	drawWidget->setController( _pCtrl );
}

// ======================== button start ===============
void DrawWindow::on_buttonStart_clicked()
{
	if ( _pCtrl )
	{
		_pCtrl->startSimulation();
	}
}

// ======================== button stop ===============
void DrawWindow::on_buttonStop_clicked()
{
	if ( _pCtrl )
	{
		_pCtrl->stopSimulation();
	}
}

// ======================== button steel ===============
void DrawWindow::on_buttonSteel_clicked()
{
	if ( _pCtrl )
	{
		_pCtrl->setMaterial( Material::steel() );
	}
}

// ======================== button rubber ===============
void DrawWindow::on_buttonRubber_clicked()
{
	if ( _pCtrl )
	{
		_pCtrl->setMaterial( Material::rubber() );
	}
}

// ======================== button wood ===============
void DrawWindow::on_buttonWood_clicked()
{
	if ( _pCtrl )
	{
		_pCtrl->setMaterial( Material::wood() );
	}
}


void DrawWindow::on_buttonNail_clicked()
{
}

void DrawWindow::on_buttonBolt_clicked()
{
}

// EOF


