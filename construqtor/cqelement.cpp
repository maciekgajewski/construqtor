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

// local
#include "cqelement.h"
#include "cqitem.h"

// tags and attributes
static const char* TAG_UNNAMED	= "unnamed";
const char* CqElement::TAG_ITEM	= "item";		///< Default tag used to store items

static const char* ATTR_TYPE	= "type";		///< Attr storing element type
static const char* ATTR_CLASS	= "class";		///< Attr storing item class
static const char* ATTR_ID		= "id";			///< Attr storing item class

static const char* TAG_RECT_TOPLEFT	= "topleft";	///< Rect pos
static const char* TAG_RECT_SIZE	= "size";		///< Rect size
static const char* TAG_POINT	= "point";			///< Polygon point
static const char* TAG_POINT_X	= "x";				///< Point coord
static const char* TAG_POINT_Y	= "y";				///< Point coord
static const char* TAG_WIDTH	= "width";			///< Size
static const char* TAG_HEIGHT	= "height";			///< Size

// types
static const char* TYPE_POINTER	= "pointer";
static const char* TYPE_POINT	= "point";
static const char* TYPE_POLYGON	= "polygon";
static const char* TYPE_RECT	= "rectangle";
static const char* TYPE_ITEM	= "item";
static const char* TYPE_STRING	= "string";
static const char* TYPE_DOUBLE	= "double";
static const char* TYPE_INT		= "int";
static const char* TYPE_SIZE	= "size";

// ========================== constructor =========================
CqElement::CqElement ( QObject *parent )
	: QObject ( parent )
{
	// nope
}

// ========================== constructor =========================
CqElement::CqElement ( const QDomElement& element, QObject *parent )
	: QObject ( parent )
	, _element( element )
{
}

// ========================== constructor =========================
CqElement::CqElement ( const CqElement& source )
	: QObject( parent() )
	, _element( source._element )
{
	// TODO any copying here
}

// ========================== destructor ==========================
CqElement::~CqElement()
{
	// nope
}

// ==================================================================
CqElement CqElement::createElement()
{
	return _element.ownerDocument().createElement( TAG_UNNAMED );
}

// ==================================================================
void CqElement::appendString( const QString& tag, const QString& value )
{
	QDomElement e = _element.ownerDocument().createElement( tag );
	e.setAttribute( ATTR_TYPE, TYPE_STRING );
	QDomText text = _element.ownerDocument().createTextNode( value );
	e.appendChild( text );
	_element.appendChild( e );
}

// ==================================================================
void CqElement::appendDouble( const QString& tag, double value )
{
	QDomElement e = _element.ownerDocument().createElement( tag );
	e.setAttribute( ATTR_TYPE, TYPE_DOUBLE );
	QDomText text = _element.ownerDocument().createTextNode( QString::number( value ) );
	e.appendChild( text );
	_element.appendChild( e );
}

// ==================================================================
void CqElement::appendInt( const QString& tag, int value )
{
	QDomElement e = _element.ownerDocument().createElement( tag );
	e.setAttribute( ATTR_TYPE, TYPE_INT );
	QDomText text = _element.ownerDocument().createTextNode( QString::number( value ) );
	e.appendChild( text );
	_element.appendChild( e );
}

// ==================================================================
void CqElement::appendPointF( const QString& tag, const QPointF& value )
{
	CqElement e = createElement();
	e.domElement().setAttribute( ATTR_TYPE, TYPE_POINT );
	e.appendDouble( TAG_POINT_X, value.x() );
	e.appendDouble( TAG_POINT_Y, value.y() );

	appendElement( tag, e );
}

// ==================================================================
void CqElement::appendSizeF( const QString& tag, const QSizeF& value )
{
	CqElement e = createElement();
	e.domElement().setAttribute( ATTR_TYPE, TYPE_SIZE );
	e.appendDouble( TAG_WIDTH, value.width() );
	e.appendDouble( TAG_HEIGHT, value.height() );

	appendElement( tag, e );
}

// ==================================================================
void CqElement::appendRectF( const QString& tag, const QRectF& value )
{
	CqElement w = createElement();
	w.domElement().setAttribute( ATTR_TYPE, TYPE_RECT );
	w.appendPointF( TAG_RECT_TOPLEFT, value.topLeft() );
	w.appendSizeF( TAG_RECT_SIZE, value.size() );
	
	appendElement( tag, w );
	
}

// ==================================================================
void CqElement::appendPolygonF( const QString& tag, const QPolygonF& value )
{
	CqElement e = createElement();
	e.domElement().setAttribute( ATTR_TYPE, TYPE_POLYGON );
	foreach( QPointF point, value )
	{
		e.appendPointF( TAG_POINT, point );
	}

	appendElement( tag, e );

}

// ==================================================================
void CqElement::appendItemPointer( const QString& tag, const CqItem* pointer )
{
	QDomElement e = _element.ownerDocument().createElement( tag );
	e.setAttribute( ATTR_TYPE, TYPE_POINTER );
	if ( pointer )
	{
		e.setAttribute( ATTR_CLASS, pointer->metaObject()->className() );
		QDomText text = _element.ownerDocument().createTextNode( pointer->id().toString() );
		e.appendChild( text );
	}
	_element.appendChild( e );
}

// ==================================================================
void CqElement::appendItem( const CqItem* pItem, const QString& tag /*= TAG_ITEM*/ )
{
	Q_ASSERT( pItem );
	
	CqElement e = createElement();
	
	pItem->store( e );
		
	e.domElement().setAttribute( ATTR_ID, pItem->id().toString() );
	e.domElement().setAttribute( ATTR_TYPE, TYPE_ITEM );
	e.domElement().setAttribute( ATTR_CLASS, pItem->metaObject()->className() );
	
	appendElement( tag, e );
}

// ==================================================================
void CqElement::appendElement( const QString& tag, const CqElement& element )
{
	QDomElement e = element.domElement();
	e.setTagName( tag );
	_element.appendChild( e );
}

// ==================================================================
QString	CqElement::readString( const QString& tag ) const
{
	return QString();
}

// ==================================================================
double	CqElement::readDouble( const QString& tag ) const
{
	return 0.0;
}

// ==================================================================
int CqElement::readInt( const QString& tag ) const
{
	return 0;
}

// ==================================================================
QPointF	CqElement::readPointF( const QString& tag ) const
{
	return QPointF();
}

// ==================================================================
QSizeF	CqElement::readSizeF( const QString& tag ) const
{
	return QSizeF();
}

// ==================================================================
QRectF	CqElement::readRectF( const QString& tag ) const
{
	return QRectF();
}

// ==================================================================
QPolygonF CqElement::readPolygonF( const QString& tag ) const
{
	return QPolygonF();
}

// ==================================================================
CqItem*	CqElement::readItemPointer( const QString& tag ) const
{
	return NULL;
}

// ==================================================================
/// Creates and reads item from subelement identified by tag.
/// Item is created with \b new, and is not owned. Should be destroyed by caller
CqItem*	CqElement::readItem( const QString& tag ) const
{
	return NULL;
}

// ==================================================================
CqElement CqElement::readElement( const QString& tag /*= TAG_ITEM*/ ) const
{
	return CqElement();
}

// ==================================================================
bool CqElement::hasElement( const QString& tag ) const
{
	return ! _element.firstChildElement( tag ).isNull();
}

// EOF
