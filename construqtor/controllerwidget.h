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
#ifndef CONTROLLERWIDGET_H
#define CONTROLLERWIDGET_H

// local
class CqMotorController;
class CqSimulation;
#include "ui_ControllerWidget.h"


class ControllerWidget: public QWidget, public Ui::ControllerWidget
{
Q_OBJECT
public:
	ControllerWidget(QWidget *parent = 0 );
	
	void setController( CqMotorController* pController, CqSimulation* pSimulation );
	
private slots:
	void on_sliderDesiredValue_valueChanged( int value );
	void simulationStep();
	void controllerDestroyed();

private:
	CqMotorController* _pController;	///< Associated motor controler
};

#endif // CONTROLLERWIDGET_H

// EOF


