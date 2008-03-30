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
#include <QFile>

// local
#include "cqsimulation.h"
#include "cqgirder.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqGirder );

// tags
static const char* TAG_SVG_APPEARANCE		= "svgappearance";

// ============================== constructor ===============
CqGirder::CqGirder( CqItem* parent ) : CqPhysicalBox( parent )
{
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
	setName( "Girder" );
	setCollisionGroup( CollisionConstruction );
}

// ============================ destructor ==================
CqGirder::~CqGirder()
{
	// nope
}

// ========================= can be moved ================
bool CqGirder::canBeMoved() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalBox::canBeMoved();
}

// ==============================================================
void CqGirder::paint
	( QPainter * pPainter
	, const QStyleOptionGraphicsItem * option
	, QWidget * widget )
{
	if ( _svgAppearance.isValid() )
	{
		_svgAppearance.render( pPainter, boundingRect() );
	}
	else
	{
		CqPhysicalBox::paint( pPainter, option, widget );
	}
}

// ========================= can be rotated ================
bool CqGirder::canBeRotated() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalBox::canBeRotated();
}

// =========================================================
QString CqGirder::description() const
{
	double m = mass();
	QString d = QString("%1, %2cmx%3cm").arg( name() ).arg( 100*size().width() ).arg( 100*size().height() );
	if ( m > 0.0 )
	{
		// mass known
		return QString("%1, %2kg").arg( d ).arg( m );
	}
	// mass unknown
	return d;
}

// ===============================================================
void CqGirder::loadSvgAppearance( const QString& path )
{
	// load svg
	QFile file( path );
	if ( file.open( QIODevice::ReadOnly ) )
	{
		setSvgAppearance( file.readAll() );
	}
	else
	{
		qWarning("CqGirder: could ot open file: %s", qPrintable( file.errorString() ) );
	}
}

// ==============================================================
void CqGirder::setSvgAppearance( const QByteArray& svg )
{
	_svgAppearanceCode = svg;
	_svgAppearance.load( svg );
	
	if ( ! _svgAppearance.isValid() && ! svg.isEmpty() )
	{
		qWarning("CqGirder: invalid SVG loaded");
	}
}

// ==============================================================
void CqGirder::store( CqElement& element ) const
{
	CqPhysicalBox::store( element );
	
	element.appendData( TAG_SVG_APPEARANCE, _svgAppearanceCode );
}

// ==============================================================
void CqGirder::load( const CqElement& element )
{
	CqPhysicalBox::load( element );
	
	setSvgAppearance( element.readData( TAG_SVG_APPEARANCE ) );
	
}

// EOF

