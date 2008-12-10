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
#ifndef CQDOCUMENT_H
#define CQDOCUMENT_H

// Qt
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QMap>
#include <QUuid>

// local
#include "cqelement.h"

/**
	This is a core of CQ I/O system. Document is a storage facility, which holds tree of elements,
	and within each element it holds values of different types, identified ith string key.
	Current implemebntation is wrpper around Qt's XML DOM classes, but I dont see why another
	couldn't be used - i.e. binary for fast network transfer.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqDocument : public QObject
{
Q_OBJECT
public:
	CqDocument( QObject *parent = 0 );
	virtual ~CqDocument();
	
	void appenElement( const QString& tag, const CqElement& element );
	CqElement readElement( const QString& tag );

	CqElement createElement();		///< Creates element

	// i/o
	void saveToFile( const QString& path ) const;
	void loadFromFile( const QString& path );
	
	QString saveToString() const;
	void loadFromString( const QString& );
	
	// item dictionary
	CqItem* itemFromDictionary( const QUuid& id ) const { return _items[ id ]; }
	
private:

	// methods
	
	void preCreateItems();
	
	// data
	
	QDomDocument	_document;		///< Actual document
	
	QMap< QUuid, CqItem* >	_items;	///< created items dictionary
};

#endif // CQDOCUMENT_H

// EOF


