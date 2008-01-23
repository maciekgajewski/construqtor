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
#ifndef DIFFICULTYSELECTOR_H
#define DIFFICULTYSELECTOR_H

// Qt
#include <QEventLoop>
// local
#include "ui_difficultyselector.h"

class DifficultySelector: public QWidget, public Ui::DifficultySelector
{
Q_OBJECT
public:
	DifficultySelector(QWidget *parent = 0);
	
	int execute();	///< Returns selected level
	
protected slots:
	
	void on_buttonEasy_clicked();
	void on_buttonIntermediate_clicked();
	void on_buttonHard_clicked();

protected:
	virtual void closeEvent( QCloseEvent* );

private:

	int _result;
	QEventLoop	_eventLoop;
};

#endif // DIFFICULTYSELECTOR_H

// EOF

