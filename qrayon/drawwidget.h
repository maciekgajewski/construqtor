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
#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QGLWidget>

#include "scenecontroller.h"

/**
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class DrawWidget : public QGLWidget
{
Q_OBJECT
public:
	
	DrawWidget(QWidget *parent = 0);
	~DrawWidget();
	
	void setController( SceneController* pCtrl );	///< Sets world

signals:

	// signals for controller
	void pointDrawed( const QPointF& point );	///< Point was drawed
	void shapeDrawed( const QPolygonF& shape );	///< Shap[e was drawed
	void pointRightClicked( const QPointF& point );	///< Point was right-clicked
	
protected slots:

	// infor from controller
	void ctrlToolChanged();
	
	
protected:

	// event handlers
	virtual void paintEvent ( QPaintEvent * event );
	virtual void mousePressEvent ( QMouseEvent * event );
	virtual void mouseReleaseEvent ( QMouseEvent * event );
	virtual void mouseMoveEvent ( QMouseEvent * event ) ;

private:

	void newPolygonDrawed();			///< Creates new scene polygon from drawed
	
	// debug painting collision shape
	void debugDrawShape( QPainter& painter, const PhysicalObject& object );
	
	QPolygonF		_drawedPolygon;		///< polygon being drawed
	bool			_drawing;			///< flag: drawinf with mouse
	
	QMatrix 	_transformation;		///< Transformation pixels->meters
	QMatrix		_phys2pixel;			///< Cached inverted transofrmation meters->pixel
	
	/// Scene controller
	SceneController*	_pCtrl;
	
};

#endif
