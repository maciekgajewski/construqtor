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

// Cq
#include "cqmotorcontroller.h"
#include "cqsimulation.h"

// local
#include "controllerwidget.h"

// ======================== constructor ===========================
ControllerWidget::ControllerWidget(QWidget *parent)
	: QWidget( parent )
    , Ui::ControllerWidget()
{
	setupUi( this );
	_pController = NULL;
}

// =========================== set controler ========================
void ControllerWidget::setController( CqMotorController* pController, CqSimulation* pSimulation )
{
	Q_ASSERT( pController );
	Q_ASSERT( pSimulation );
	
	_pController = pController;
	connect( pController, SIGNAL(destroyed()), SLOT(controllerDestroyed()) );

	connect( pSimulation, SIGNAL(simulationStep()), SLOT(simulationStep()));
	
	simulationStep(); // to update gauges
}

// ========================== desired value changed ===================
void ControllerWidget::on_sliderDesiredValue_valueChanged( int value )
{
	Q_ASSERT( _pController );
	
	double v = _pController->getMinValue() + 
		( _pController->getMaxValue() - _pController->getMinValue() ) * value / 100.0;
	_pController->setDesiredValue( v );
}

// ============================ simulation step =======================
void ControllerWidget::simulationStep()
{
	Q_ASSERT( _pController );
	
	// update current value
	barCurrentValue->setValue( 100*
		( _pController->getCurrentValue() - _pController->getMinValue() )/
		( _pController->getMaxValue() - _pController->getMinValue() )
		);
		
	// update current force
	double maxForce = qMax( qAbs( _pController->getMaxForce() ),
							qAbs( _pController->getMinForce() ) );
	
	barForce->setValue( 100 * ( _pController->getCurrentForce() / maxForce ) );
		
}

// ============================= controller destroyed ==================
void ControllerWidget::controllerDestroyed()
{
	_pController = NULL;
	deleteLater();
}

// EOF 

