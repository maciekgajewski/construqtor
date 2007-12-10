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
#ifndef MAINVIEW_H
#define MAINVIEW_H

// Qt
#include <QGraphicsView>


// Cq
class CqItem;

// local
class CeEditorItem;

/**
	Main scene view implements viewport control and editing.
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class MainView : public QGraphicsView
{
Q_OBJECT
public:
	MainView( QWidget* parent = NULL );
	MainView(QGraphicsScene* scene, QWidget* parent = NULL);
	
	virtual ~MainView();

signals:

	void pointerPos( double x, double y );	///< Signals mouse pointer pos in scene corrds
	
protected:

	// mouse events
	
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent ( QWheelEvent * event );
	
	// methods
	
	void init();
	
	void selectUnderPoint( const QPoint& pos );
	//void startDragUnderPoint( const QPoint& pos );
	void unselectAll();
	void breakJointUnderPoint( const QPoint& pos );
	
	
	//CqItem* _draggedItem;	///< Currently dragged item TODO remove
	CeEditorItem* _pEditor;	///< editor item
	
protected slots:

	void simulationStarted();
	void simulationPaused();
};

#endif	// MAINVIEW_H
