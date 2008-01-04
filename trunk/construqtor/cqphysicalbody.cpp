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

// Qt
#include <QPainter>

// box2d
#include "b2Body.h"

// local
#include "cqphysicalbody.h"
#include "cqitemtypes.h"
#include "cqjoint.h"
#include "cqsimulation.h"

// constants
static const char* TAG_MATERIAL	= "material";
static const char* TAG_LINEAR_VEL = "linearvelocity";
static const char* TAG_ANGULAR_VEL = "angularvelocity";

// ========================== constructor =================================
CqPhysicalBody::CqPhysicalBody( CqItem* parent )
	: CqItem( parent )
{
	init();
}

// =========================== destructor ===================================
CqPhysicalBody::~CqPhysicalBody()
{
	breakAllJoints();
}

// =========================== init ===================================
void CqPhysicalBody::init()
{
	_pBody = NULL;
	_initialAngluarVelocity = 0.0;
	
	// make rotatable
	setEditorFlags( editorFlags() | Rotatable );
}

// =========================== set world ===================================
/// destroys current body, and creates new one. Calls createShape() along the way
void CqPhysicalBody::recreateBody()
{
	if ( world() )
	{
		if ( _pBody )
		{
			// destroy current body
			destroyBody( world() );
		}
		// NOTE: body will be created when needed, by simulation with assureBodyCreated()
	}
}

// =========================== create body ===================================
void CqPhysicalBody::createBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	
	b2BodyDef bodyDef;
	
	// create body shape(s)
	QList< b2ShapeDef* > shapes = createShape();
	
	// add shapes to body
	foreach( b2ShapeDef* pShape, shapes )
	{
		bodyDef.AddShape( pShape );
	}
	
	// attach pointer to self to the box2d body
	// TODO is this needed?
	bodyDef.userData = this;
	
	// damping TODO: experimental
	bodyDef.linearDamping	= 0.0001;
	bodyDef.angularDamping	= 0.001;
	
	// create body
	_pBody = pWorld->CreateBody(&bodyDef);
	
	// get center pos
	b2Vec2 cog = _pBody->GetCenterPosition();
	setCenter( QPointF( cog.x, cog.y ) );
	
	// set body position
	updatePhysicalPos();
	
	// set body velocities
	_pBody->SetAngularVelocity( _initialAngluarVelocity );
	_pBody->SetLinearVelocity( b2Vec2( _initialLinearVelocity.x(), _initialLinearVelocity.y() ) );
	
	// delete shapes
	foreach ( b2ShapeDef* pShape, shapes )
	{
		delete pShape;
	}
	
}

// =========================== update to physical ===================================
void CqPhysicalBody::updatePosToPhysical()
{
	if ( _pBody )
	{
		b2Vec2		b2pos		= _pBody->GetCenterPosition();
		double		b2rotation	= _pBody->GetRotation();
		
		setWorldPos( QPointF( b2pos.x, b2pos.y ) - center() ); // correct pos by COG
		setWorldRotation( b2rotation );
		
	}
}

// =========================== assure body created ===================================
/// Called by simulation before it starts.
/// It couses body creation on bodies which does not have body yet
/// This method is called late, so body creation is deffered as much as possible
void CqPhysicalBody::assureBodyCreated()
{
	Q_ASSERT( world() );
	
	if ( ! _pBody )
	{
		createBody(world());
	}
}

// =========================== destroy body ===================================
void CqPhysicalBody::destroyBody( CqWorld* pWorld )
{
	Q_ASSERT( pWorld );
	Q_ASSERT( _pBody );
	
	pWorld->DestroyBody( _pBody );
	_pBody = NULL;
}

// =========================== type  ===================
int CqPhysicalBody::type() const
{
	return CQ_BODY;
}

// =================================== breaks all atached joints ===
void CqPhysicalBody::breakAllJoints()
{
	// get copy of joint list, couse they will be removed from 'official' list
	// during the process
	QList< CqJoint* > joints = _joints;
	
	foreach( CqJoint* pJoint, joints )
	{
		pJoint->breakJoint();
	}
	
	notifyParent();
}

// ============================== update physical pos ===============
void CqPhysicalBody::updatePhysicalPos()
{
	CqItem::updatePhysicalPos();
	if ( _pBody )
	{
		QPointF pp	= worldPos() + center(); // correct physical pos by COG shift
		double r	= worldRotation();
		
		_pBody->SetCenterPosition( b2Vec2( pp.x(), pp.y() ), r );
		//qDebug("body %s has pos %lf,%lf, rotation %lf",
		//	qPrintable( name() ), pp.x(), pp.y(), r ); // TODO remove debug
		update();
	}
}

// =============================== add joint ======================
void CqPhysicalBody::addJoint( CqJoint* pJoint )
{
	// duplicate check
	if ( _joints.contains( pJoint ) )
	{
		qWarning("joint added once again to same body");
	}
	else
	{
		_joints.append( pJoint );
		// notify parent
		notifyParent();
	}
}

// =============================== remove joint ======================
void CqPhysicalBody::removeJoint( CqJoint* pJoint )
{
	// existence check
	if ( _joints.contains( pJoint ) )
	{
		_joints.removeAll( pJoint );
		notifyParent();
	}
	else
	{
		qWarning("joint remove from body, to each it doesn;t belogns");
	}
}

// ========================= debug: draw collision ==================
void CqPhysicalBody::debugDrawCollision( QPainter* pPainter )
{
	// no body - no bodpainting :)
	if ( ! _pBody )
	{
		return;
	}
	
	Q_ASSERT( pPainter );
	
	pPainter->save();
	pPainter->setPen( QPen( Qt::red, 0 ) );
	pPainter->setBrush( QBrush() );
	
	for ( b2Shape* s = _pBody->GetShapeList(); s; s = s->GetNext() )
	{
		// paint polygon
		if ( s->GetType() == e_polyShape )
		{
			b2PolyShape* pPolyShape = (b2PolyShape*) s;
			QPolygonF shape( pPolyShape->m_vertexCount );
			//b2Mat22 rotation = pPolyShape->GetRotationMatrix();
			
			for ( int i =0; i < pPolyShape->m_vertexCount; i++ )
			{
				b2Vec2& vertex = pPolyShape->m_vertices[i];
				shape[i] =QPointF( pPolyShape->m_vertices[i].x, vertex.y );
			}
			
			// translate to body-local centroid
			shape.translate
				( pPolyShape->m_localCentroid.x + center().x()
				, pPolyShape->m_localCentroid.y + center().y()
				);
			
			pPainter->drawPolygon( shape );
		}
		// TODO other shape types
	}
	
	pPainter->restore();
}

// ============================ simulation started ================
/// Wakes up body on simualtion start
void CqPhysicalBody::simulationStarted()
{
	CqItem::simulationStarted();
	
	if ( _pBody )
	{
		_pBody->WakeUp();
	}
}

// =========================== mass ===============================
double CqPhysicalBody::mass() const
{
	if ( _pBody )
	{
		return _pBody->GetMass();
	}
	
	return 0.0;
}

// ============================= description ====================
/// Adds mass to description
QString CqPhysicalBody::description()
{
	return QString("%1, %2kg").arg( CqItem::description() ).arg( mass() );
}

// ==============================================================
void CqPhysicalBody::store( CqElement& element ) const
{
	CqItem::store( element );
	
	// store material
	CqElement materialElement = element.createElement();
	_material.store( materialElement );
	element.appendElement( TAG_MATERIAL, materialElement );
	
	// TODO pen and brush here
	
	// store body properties, if created
	if ( _pBody )
	{
		b2Vec2 lv = _pBody->GetLinearVelocity();
		double av = _pBody->GetAngularVelocity();
		
		element.appendPointF( TAG_LINEAR_VEL, QPointF( lv.x, lv.y ) );
		element.appendDouble( TAG_ANGULAR_VEL, av );
	}
}

// ==============================================================
void CqPhysicalBody::load( const CqElement& element )
{
	CqItem::load( element );

	// get material
	CqElement materialElement = element.readElement( TAG_MATERIAL );
	_material.load( materialElement );
	
	// get velocities
	if ( element.hasElement( TAG_LINEAR_VEL ) && element.hasElement( TAG_ANGULAR_VEL ) )
	{
		QPointF lv = element.readPointF( TAG_LINEAR_VEL );
		double av = element.readDouble( TAG_ANGULAR_VEL );
		if ( _pBody )
		{
			_pBody->SetAngularVelocity( av );
			_pBody->SetLinearVelocity( b2Vec2( lv.x(), lv.y() ) );
		}
		else
		{
			_initialAngluarVelocity	= av;
			_initialLinearVelocity	= lv;
		}
	}
}


// EOF

