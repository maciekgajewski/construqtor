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
#ifndef DRAWWINDOW_H
#define DRAWWINDOW_H

#include "scenecontroller.h"

#include "ui_drawwindow.h"

class DrawWindow: public QWidget, public Ui::DrawWindow
{
	Q_OBJECT
public:
	DrawWindow(QWidget *parent = 0);
	
	void setController( SceneController* pCtrl );	///< Sets scene controler
	
protected slots:

	// button handlers
	void on_buttonStart_clicked();
	void on_buttonStop_clicked();
	
	void on_buttonSteel_clicked();
	void on_buttonRubber_clicked();
	void on_buttonWood_clicked();
	
	void on_buttonNail_clicked();
	void on_buttonBolt_clicked();

private:

	SceneController* _pCtrl;					///< Scene controller
};

#endif
