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

#include <math.h>

#include <QPainter>
#include <QMouseEvent>

#include "drawwidget.h"

// ==================== constructor =================
DrawWidget::DrawWidget(QWidget *parent)
 : QGLWidget(parent)
{
	// init
	_drawing	= false;
	_transformation.scale( 0.01, -0.01 ); // 100pixels/meter
	_transformation.translate( 0, -1000 );
	_pWorld 	= NULL;
}


// ==================== destructor =================
DrawWidget::~DrawWidget()
{
}

// ==================== painting =================
void DrawWidget::paintEvent( QPaintEvent* pEvent )
{
	Q_UNUSED( pEvent );
	
	QPainter painter;
	painter.begin( this );
	
	// init painter
	painter.setRenderHint( QPainter::Antialiasing, true );
	
	// paint drawed polygon
	if ( _drawing )
	{
		painter.setPen( QPen( Qt::red, 5 ) );
		if ( _drawedPolygon.size() > 1 )
		{
			painter.drawPolyline( _drawedPolygon.data(), _drawedPolygon.size() );
		}
	}
	
	// paint scene polygons
	if ( _pWorld )
	{
		painter.setPen( QPen( Qt::blue, 5 ) );
		QMatrix phys2pixel = _transformation.inverted();
		const QList<PhysicalObject>& objects = _pWorld->objects();
		for( int i = 0; i < objects.size(); i++ )
		{
			const PhysicalObject& object = objects[ i ];
			QPolygonF polygon = phys2pixel.map( object.outline() );
			painter.drawPolygon( polygon );
		}
	}
	
	
	painter.end();
}

// ==================== press event =================
void DrawWidget::mousePressEvent ( QMouseEvent * pEvent )
{
	Q_ASSERT( pEvent );
	
	// start painting with left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		// initiate drawing
		_drawing = true;
		_drawedPolygon.clear();
	}
	
}

// ==================== release event =================
void DrawWidget::mouseReleaseEvent ( QMouseEvent * pEvent )
{
	Q_ASSERT( pEvent );
	
	// start painting with left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		_drawing = false;
		newPolygonDrawed();
	}
}

// ==================== move event =================
void DrawWidget::mouseMoveEvent ( QMouseEvent * pEvent ) 
{
	Q_ASSERT( pEvent );
	
	// add point to polygon
	if ( _drawing )
	{
		_drawedPolygon.push_back( pEvent->pos() );
		repaint();
	}
}

// =================== creates new polygon from drawed ===========
/// Turns _drawedPolygon into QPolygon, adds to scene
void DrawWidget::newPolygonDrawed()
{
	// ignore empty
	if ( _drawedPolygon.size() < 2 )
	{
		return;
	}
	
	// addpolygon to world
	if ( _pWorld )
	{
		_pWorld->addObject( _transformation.map( _drawedPolygon ) );
	}
		
		
	repaint();
}

// ===================== sets wortld ================
void DrawWidget::setWorld( World* world )
{
	_pWorld = world;
	
	if ( _pWorld )
	{
		connect( _pWorld, SIGNAL(animated()), SLOT(repaint() ) );
	}
}

