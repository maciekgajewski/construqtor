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

#include "b2Body.h"

#include <QPainter>
#include <QMouseEvent>

#include "drawwidget.h"
#include "world.h"


// ==================== constructor =================
DrawWidget::DrawWidget(QWidget *parent)
 : QGLWidget(parent)
{
	// init
	_drawing	= false;
	_transformation.translate( 0.0, 6.0 );
	_transformation.scale( 0.01, -0.01 ); // 100pixels/meter
	_phys2pixel = _transformation.inverted();
	_pCtrl 	= NULL;
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
	if ( _pCtrl )
	{
		const World* pWorld = _pCtrl->world();
		
		const QList<PhysicalObject>& objects = pWorld->objects();
		for( int i = 0; i < objects.size(); i++ )
		{
			const PhysicalObject& object = objects[ i ];
			
			// select color
			if ( object.material().type == Material::Steel )
			{
				painter.setPen( QPen( Qt::blue, 5 ) );
			}
			else if ( object.material().type == Material::Wood )
			{
				painter.setPen( QPen( Qt::darkRed, 5 ) );
			}
			else
			{
				painter.setPen( QPen( Qt::black, 5 ) );
			}
			
			QPolygonF polygon = _phys2pixel.map( object.outline() );
			painter.drawPolygon( polygon );
			// debug
			//debugDrawShape( painter, object );
		}
	}
	
	// paint ground
	QPointF zero = _phys2pixel.map( QPointF(0, 0) );
	painter.setPen( QPen( Qt::green, 5 ) );
	painter.drawLine( QPointF( 0, zero.y() ), QPointF( width(), zero.y() ) );
	
	
	painter.end();
}

// ==================== debug draw collision  =================
void DrawWidget::debugDrawShape( QPainter& painter, const PhysicalObject& body )
{
	b2Body* pBody = const_cast<b2Body*>(body.b2body()); // workaround for library incosistnecy
	painter.save();
	for ( b2Shape* s = pBody->GetShapeList(); s; s = s->GetNext() )
	{
		// paint polygon
		if ( s->GetType() == e_polyShape )
		{
			b2PolyShape* pPolyShape = (b2PolyShape*) s;
			QPolygonF shape( pPolyShape->m_vertexCount );
			//b2Mat22 rotation = pPolyShape->GetRotationMatrix();
			
			for ( int i =0; i < pPolyShape->m_vertexCount; i++ )
			{
				//b2Vec2 vertex = rotation.Solve( pPolyShape->m_vertices[i] );
				b2Vec2& vertex = pPolyShape->m_vertices[i];
				shape[i] =QPointF( pPolyShape->m_vertices[i].x, vertex.y );
				//qDebug("debug vertex %d: %.2lf,%.2lf", i, double(vertex.x), double(vertex.y) );
			}
			
			// translate to body-local centroid
			shape.translate( pPolyShape->m_localCentroid.x, pPolyShape->m_localCentroid.y );
			
			// apply body transformation
			double angle = pBody->GetRotation();
			QMatrix bodyTransform;
			bodyTransform.translate( pBody->GetCenterPosition().x, pBody->GetCenterPosition().y );
			bodyTransform.rotate( 180*angle / M_PI);
			
			shape = bodyTransform.map( shape );
			
			painter.setPen( QPen( Qt::black, 1 ) );
			painter.drawPolygon( _phys2pixel.map( shape ) );
		}
	}
	painter.restore();

}

// ==================== press event =================
void DrawWidget::mousePressEvent ( QMouseEvent * pEvent )
{
	Q_ASSERT( pEvent );
	
	// start painting with left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		if ( _pCtrl) switch ( _pCtrl->tool() )
		{
			// drawing tools
			case SceneController::ToolPaintObject:
				// initiate drawing
				_drawing = true;
				_drawedPolygon.clear();
				break;
				
			//TODO support others here
		}
	}
	
	// emit signal on right button
	else if ( pEvent->button() == Qt::RightButton )
	{
		QPointF point = _transformation.map( pEvent->pos() );
		emit pointRightClicked( point );
		repaint();
	}
	
}

// ==================== release event =================
void DrawWidget::mouseReleaseEvent ( QMouseEvent * pEvent )
{
	Q_ASSERT( pEvent );
	
	// start painting with left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		if ( _drawing == true )
		{
			_drawing = false;
			newPolygonDrawed();
		}
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
	QPolygonF shape = _transformation.map( _drawedPolygon );
	emit shapeDrawed( shape );
	repaint();
}

// ===================== sets controller ================
void DrawWidget::setController( SceneController* pCtrl )
{
	if ( pCtrl )
	{
		// disconnect
		if ( _pCtrl )
		{
			disconnect( _pCtrl, NULL, this, NULL );
			disconnect( _pCtrl->world(), NULL, this, NULL );
		}
		disconnect( SIGNAL( pointDrawed( const QPointF&  ) ) );
		disconnect( SIGNAL( shapeDrawed( const QPolygonF&  ) ) );
		disconnect( SIGNAL( pointRightClicked( const QPointF&  ) ) );
	
		const World* pWorld = pCtrl->world();
	
		connect( pWorld, SIGNAL(animated()), SLOT(repaint() ) );
		connect( pCtrl, SIGNAL( toolChanged() ), SLOT( ctrlToolChanged() ) );
		connect( this, SIGNAL( pointDrawed( const QPointF&  ) ), pCtrl, SLOT(pointDrawed(const QPointF&)));
		connect( this, SIGNAL( shapeDrawed( const QPolygonF&  ) ), pCtrl, SLOT(shapeDrawed( const QPolygonF&  )));
		connect( this, SIGNAL( pointRightClicked( const QPointF&  ) ), pCtrl, SLOT(pointRightClicked(const QPointF&)));
	
		_pCtrl = pCtrl;
	}
}

// =================== tool changed =======================
void DrawWidget::ctrlToolChanged()
{
	// TODO add some visual clue what happened
}

