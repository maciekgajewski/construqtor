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

// Qt
#include <QFile>

// local
#include "cqdocument.h"
#include "cqelement.h"
#include "gexception.h"

// constants
static const char* DOCTYPE		= "constructor-data-file";
static const char* TAG_UNNAMED	= "unnamed";	///< Default tag

// ============================== csontructor ====================
CqDocument::CqDocument(QObject *parent)
	: QObject(parent)
	, _document( DOCTYPE )
{
	// nope
}

// ============================== destructor ====================
CqDocument::~CqDocument()
{
	// nope
}

// =============================== append element ===============
void CqDocument::appenElement( const QString& tag,const CqElement& element )
{
	QDomElement e = element.domElement();
	e.setTagName( tag );
	_document.appendChild( e );
}

// ================================= read element ================
CqElement CqDocument::readElement( const QString& tag )
{
	QDomElement e = _document.firstChildElement( tag );
	
	return e;
}

// =============================== save to file ===============
void CqDocument::saveToFile( const QString& path )
{
	QFile file( path );
	if ( ! file.open(QIODevice::WriteOnly) )
	{
		throw GSysError( QString("Error opening file %1: %2").arg( path ).arg( file.errorString() ) );
	}
	int bytes = file.write( _document.toByteArray( 4 ) ); // indent 4
	if ( bytes < 0 )
	{
		file.close();
		throw GSysError( QString("Error writing to file %1: %2").arg( path ).arg( file.errorString() ) );
	}
	qDebug("bytes written: %d", bytes ); // TODO remove
	
	file.close();
}

// =============================== load from file ===============
void CqDocument::loadFromFile( const QString& path )
{
	QFile file( path );
	if ( ! file.open(QIODevice::ReadOnly) )
	{
		throw GSysError( QString("Error opening file %1: %2").arg( path ).arg( file.errorString() ) );
	}
	if ( ! _document.setContent( &file ) )
	{
		file.close();
		throw GSysError( QString("Error reading from file %1: %2").arg( path ).arg( file.errorString() ) );
	}
	
	file.close();
}

// ================================ create element ===============
CqElement CqDocument::createElement()
{
	// NOTE: this uses converting condtructor of CqElement
	return _document.createElement( TAG_UNNAMED );
}

// EOF

