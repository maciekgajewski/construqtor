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
class CqSimulation;
class CqRevoluteJoint;

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

	void setSimulation( CqSimulation* pSimulation );
	
	// toolbox
	void toolAddObject( CqItem* pItem );	///< Adds new item to scene
	void toolAddNail( CqRevoluteJoint* pNail ); ///< Adds new nail

signals:

	void pointerPos( double x, double y );	///< Signals mouse pointer pos in scene corrds
	
protected:

	/// work modes
	enum Mode{
		SELECTING,
		ADDING_OBJECT,
		ADDING_NAIL,
		SCROLLING
		};

	void setMode( Mode mode );			///< Proper way to change mode
	
	// mouse events
	
	virtual void mouseDoubleClickEvent(QMouseEvent* event);
	virtual void mouseMoveEvent(QMouseEvent* event);
	virtual void mousePressEvent(QMouseEvent* event);
	virtual void mouseReleaseEvent(QMouseEvent* event);
	virtual void wheelEvent ( QWheelEvent * event );
	
	virtual void showEvent( QShowEvent* pEvent );
	
	// other events
	
	virtual void resizeEvent( QResizeEvent* pEvent );
	
	// methods
	
	void init();
	
	void scrollBy( const QPoint& delta );	///< Scrolls content
	void adjustScale();						///< Makes sure scale is sane
	
	void selectUnderPoint( const QPoint& pos );
	//void startDragUnderPoint( const QPoint& pos );
	void unselectAll();
	void breakJointUnderPoint( const QPoint& pos );
	
	bool canAddNail( const QPointF& point ) const;		///< Checks if nail may be added here
	void addNail( QPointF& point, CqRevoluteJoint* pNail ); ///< Adds nail at point
	
	
	//CqItem* _draggedItem;	///< Currently dragged item TODO remove
	CeEditorItem* _pEditor;	///< editor item
	
protected slots:

	void simulationStarted();
	void simulationPaused();

private:

	// data
	CqSimulation*		_pSimulation;
	CqItem*				_addedObject;		///< Added object
	CqRevoluteJoint*	_addedNail;			///< Added joint
	Mode				_mode;				///< Current work mode
	QPoint				_scrollStart;		///< Scorll start point
	
	bool _viewportInitialized;				///< Vievport position initialized in showEvent
};

#endif	// MAINVIEW_H
