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
#include "cqclipboard.h"
#include "cqitem.h"

// static data
CqClipboard*	CqClipboard::_pInstance	= NULL;

// constants
static const char* TAG_CLIPBOARD = "clipboard";

// ===============================================================
CqClipboard::CqClipboard( QObject* parent )
	: QObject(parent)
{
	init();
}


// ===============================================================
CqClipboard::~CqClipboard()
{
	// none
}

// ===============================================================
void CqClipboard::init()
{
	// nope
}

// ===============================================================
CqClipboard* CqClipboard::instance()
{
	if ( ! _pInstance )
	{
		_pInstance = new CqClipboard();
	}
	
	return _pInstance;
}

// ===============================================================
void CqClipboard::copy( const CqItem* pItem )
{
	clear();
	
	
	CqDocument doc;
	CqElement e = doc.createElement();
	e.appendItem( pItem );
	doc.appenElement( TAG_CLIPBOARD, e );
	
	_xmlData = doc.saveToString();
}

// ===============================================================
/// Creates and returns instance of stored item. Instance is owned by caller
CqItem* CqClipboard::get() const
{
	if ( storesItem() )
	{
		CqDocument doc;
		doc.loadFromString( _xmlData );
		CqElement e = doc.readElement( TAG_CLIPBOARD );
		CqItem* pItem =  e.readItem();
		if ( pItem )
		{
			// take the item from document, so it wont be destroyed when this scope is left
			pItem->setParent( NULL );
			// sleect new, randon id fro item, so it is unique
			pItem->generateNewId();
		}
		
		return pItem;
	}
	
	return NULL;
}

// ===============================================================
void CqClipboard::clear()
{
	_xmlData = "";
}

// ===============================================================
bool CqClipboard::storesItem() const
{
	return ! _xmlData.isEmpty();
}

// EOF


