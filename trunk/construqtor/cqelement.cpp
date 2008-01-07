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
#include "cqdocument.h"
#include "cqitem.h"
#include "cqitemfactory.h"

// tags and attributes
// TODO clean this mess with sttaic/class static items
static const char* TAG_UNNAMED	= "unnamed";
const char* CqElement::TAG_ITEM	= "item";		///< Default tag used to store items

const char* CqElement::ATTR_TYPE	= "type";		///< Attr storing element type
const char* CqElement::ATTR_CLASS	= "class";		///< Attr storing item class
const char* CqElement::ATTR_ID		= "id";			///< Attr storing item class

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
const char* CqElement::TYPE_ITEM	= "item";
static const char* TYPE_STRING	= "string";
static const char* TYPE_DATA	= "data";
static const char* TYPE_DOUBLE	= "double";
static const char* TYPE_INT		= "int";
static const char* TYPE_SIZE	= "size";

// ========================== constructor =========================
CqElement::CqElement ( QObject *parent )
	: QObject ( parent )
{
	_pDocument = NULL;
}

// ========================== constructor =========================
CqElement::CqElement ( const QDomElement& element, QObject *parent )
	: QObject ( parent )
	, _element( element )
{
	_pDocument = NULL;
}

// ========================== constructor =========================
CqElement::CqElement ( const CqElement& source )
	: QObject( parent() )
	, _element( source._element )
{
	// TODO any copying here
	_pDocument = source._pDocument;
}

// ========================== destructor ==========================
CqElement::~CqElement()
{
	// nope
}

// ==================================================================
CqElement CqElement::createElement()
{
	CqElement e( _element.ownerDocument().createElement( TAG_UNNAMED ) );
	e.setDocument( _pDocument );
	
	return e;
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
void CqElement::appendData( const QString& tag, const QByteArray& data )
{
	QDomElement e = _element.ownerDocument().createElement( tag );
	e.setAttribute( ATTR_TYPE, TYPE_DATA );
	QDomCDATASection cdata = _element.ownerDocument().createCDATASection( data );
	e.appendChild( cdata );
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

// =========================== get text =============================
/// Returns text od element with specified tag and type
QString CqElement::getSubelementText( const QString& tag, const QString& type ) const
{
	// get element
	QDomElement e = getNextElement( tag, type );
	if ( e.isNull() )
	{
		return QString::null;
	}
	
	return e.text();
}

// ==================================================================
/// Returns null string if value can not be readed
QString	CqElement::readString( const QString& tag ) const
{
	return getSubelementText( tag, TYPE_STRING );
}

// ==================================================================
QByteArray CqElement::readData( const QString& tag ) const
{
	return getSubelementText( tag, TYPE_DATA).toAscii(); // TODO wild try
}

// ==================================================================
/// Return 0.0 on error
double	CqElement::readDouble( const QString& tag ) const
{
	QString s = getSubelementText( tag, TYPE_DOUBLE );
	bool ok = false;
	double d = s.toDouble( &ok );
	if ( ! ok )
	{
		return 0.0;
	}
	
	return d;
}

// ==================================================================
/// Return 0 on error
int CqElement::readInt( const QString& tag ) const
{
	QString s = getSubelementText( tag, TYPE_INT );
	bool ok = false;
	int i = s.toInt( &ok );
	if ( ! ok )
	{
		return 0;
	}
	
	return i;
}

// ==================================================================
QPointF	CqElement::readPointF( const QString& tag ) const
{
	QDomElement e = getNextElement( tag, TYPE_POINT );
	if ( e.isNull() )
	{
		return QPointF();
	}
	
	CqElement w( e );
	
	double x = w.readDouble( TAG_POINT_X );
	double y = w.readDouble( TAG_POINT_Y );
	
	return QPointF( x, y );
}

// ==================================================================
QSizeF	CqElement::readSizeF( const QString& tag ) const
{
	QDomElement e = getNextElement( tag, TYPE_SIZE );
	if ( e.isNull() )
	{
		return QSizeF();
	}
	
	CqElement wrapper( e );
	
	double w = wrapper.readDouble( TAG_WIDTH);
	double h = wrapper.readDouble( TAG_HEIGHT );
	
	return QSizeF( w, h );
}

// ==================================================================
QRectF	CqElement::readRectF( const QString& tag /*= TAG_ITEM*/ ) const
{
	QDomElement e = getNextElement( tag, TYPE_RECT );
	if ( e.isNull() )
	{
		return QRectF();
	}
	
	CqElement w( e );
	
	QPointF topLeft = w.readPointF( TAG_RECT_TOPLEFT );
	QSizeF size = w.readSizeF( TAG_RECT_SIZE );
	
	return QRectF( topLeft, size );
}

// ==================================================================
QPolygonF CqElement::readPolygonF( const QString& tag ) const
{
	QDomElement e = getNextElement( tag, TYPE_POLYGON );
	if ( e.isNull() )
	{
		return QPolygonF();
	}
	
	CqElement w( e );
	
	QPolygonF result;
	forever
	{
		QPointF p = w.readPointF( TAG_POINT );
		if ( p.isNull() )
		{
			break;
		}
		result.append( p );
	}
	
	return result;
}

// ==================================================================
CqItem*	CqElement::readItemPointer( const QString& tag ) const
{
	Q_ASSERT( _pDocument );
	
	QDomElement e = getNextElement( tag, TYPE_POINTER );
	if ( e.isNull() )
	{
		return NULL;
	}
	
	QUuid  id( e.text() );
	
	return _pDocument->itemFromDictionary( id );
}

// ==================================================================
/// Creates and reads item from subelement identified by tag.
/// Item is created with \b new, and is not owned. Should be destroyed by caller
CqItem*	CqElement::readItem( const QString& tag ) const
{
	
	CqItem* pItem = itemFromElement( getNextElement( tag, TYPE_ITEM ) );
	// ok, item was readed by someone, so it should not be aour child, someone will take care of him
	if ( pItem && pItem->parent() == _pDocument )
	{
		pItem->setParent( NULL );
	}
	
	return pItem;
}


// =========================== item form element ======================
CqItem*	CqElement::itemFromElement( const QDomElement& element ) const
{
	Q_ASSERT( _pDocument );
	
	if ( element.attribute( ATTR_TYPE, QString::null ) != TYPE_ITEM )
	{
		//qWarning("no item element with tag %s", qPrintable( element.tagName() ) );
		return NULL;
	}
	
	QUuid id( element.attribute( ATTR_ID ) );
	if ( id.isNull() )
	{
		qWarning("could not red id from element's attribute");
		return NULL;
	}
	
	// try get pre-created item
	CqItem* pItem = _pDocument->itemFromDictionary( id );
	
	// if thi fails - create new one
	if ( ! pItem )
	{
		QString type = element.attribute( ATTR_CLASS );
		pItem = CqItemFactory::createItem( type );
	}
	
	Q_ASSERT( pItem );
	
	// create wrapper
	CqElement wrapper( element );
	wrapper.setDocument( document() );
		
	// read item
	pItem->setId( id );
	pItem->load( wrapper );
	
	return pItem;
}

// ==================================================================
CqElement CqElement::readElement( const QString& tag  ) const
{
	return getNextElement( tag );
}

// ==================================================================
bool CqElement::hasElement( const QString& tag ) const
{
	return ! _element.firstChildElement( tag ).isNull();
}

// ==================================================================
/// Seeks for next element with given tag and type
/// Ignores type if null type=QString::null
QDomElement CqElement::getNextElement( const QString& tag, const QString&  type ) const
{
	// rewind if new tag is searched
	if ( _lastTag != tag )
	{
		_lastTag = tag;
		_lastFound = QDomElement(); // rewind
	}
	do
	{
		if ( _lastFound.isNull() )
		{
			_lastFound = _element.firstChildElement( tag );
		}
		else
		{
			_lastFound = _lastFound.nextSiblingElement( tag );
		}
		
		if ( _lastFound.isNull() )
		{
			//qWarning("no element with tag %s", qPrintable( tag ) );
			return QDomElement();
		}
		
		// check type
		if ( type.isNull() || _lastFound.attribute( ATTR_TYPE, QString::null ) == type )
		{
			return _lastFound;
		}
		
	} while ( ! _lastFound.isNull() );
	
	if ( _lastFound.isNull() )
	{
		qWarning("could not find element with tag %s ant type %s", qPrintable( tag ), qPrintable( type ) );
	}
	
	return _lastFound;
}

// EOF
