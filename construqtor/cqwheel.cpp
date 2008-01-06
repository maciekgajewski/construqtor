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
#include "cqwheel.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqWheel );

// tags
static const char* TAG_CONNECTABLE_DIAMETER	= "connectablediameter";
static const char* TAG_SVG_APPEARANCE		= "svgappearance";


// ============================== constructor ===============
CqWheel::CqWheel( CqItem* parent ) : CqPhysicalDisk( parent )
{
	init();
}

// ============================ destructor ==================
CqWheel::~CqWheel()
{
	// nope
}

// ================================ init ====================
void CqWheel::init()
{
	setEditorFlags( editorFlags() | Selectable | Movable );
	setMaterial( CqMaterial( 100.0, 2.0, 0.3 ) );
	
	setName( "Wheel" );
}

// ========================= can be moved ================
bool CqWheel::canBeMoved() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalDisk::canBeMoved();
}

// ========================= can be rotated ================
bool CqWheel::canBeRotated() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalDisk::canBeRotated();
}

// ==============================================================
void CqWheel::setSvgAppearance( const QByteArray& svg )
{
	_svgAppearanceCode = svg;
	_svgAppearance.load( svg );
	
	if ( ! _svgAppearance.isValid() )
	{
		qWarning("invalid SVG loaded");
	}
}

// ==============================================================
void CqWheel::paint
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
		CqPhysicalDisk::paint( pPainter, option, widget );
	}
}
	
// ==============================================================
QRectF CqWheel::boundingRect() const
{
	// TODO remove if not needed
	return CqPhysicalDisk::boundingRect();
}

// ==============================================================
void CqWheel::store( CqElement& element ) const
{
	CqPhysicalDisk::store( element );
	
	element.appendDouble( TAG_CONNECTABLE_DIAMETER, _connectableDiameter );
	element.appendData( TAG_SVG_APPEARANCE, _svgAppearanceCode );
}

// ==============================================================
void CqWheel::load( const CqElement& element )
{
	CqPhysicalDisk::load( element );
	
	_connectableDiameter = element.readDouble( TAG_CONNECTABLE_DIAMETER );
	setSvgAppearance( element.readData( TAG_SVG_APPEARANCE ) );
	
}

// ===============================================================
QString CqWheel::description() const
{
	QString d = QString("%1, %2%3cm").arg( name() ).arg( QChar( 0x00F8 ) ).arg( 100*diameter() );
	return d;
}

// ===============================================================
void CqWheel::loadSvgAppearance( const QString& path )
{
	// load svg
	QFile file( path );
	if ( file.open( QIODevice::ReadOnly ) )
	{
		setSvgAppearance( file.readAll() );
	}
	else
	{
		qWarning("could ot open file: %s", qPrintable( file.errorString() ) );
	}
}

// EOF

