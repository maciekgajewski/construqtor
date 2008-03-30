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
#ifndef CQCLIPBOARD_H
#define CQCLIPBOARD_H

// Qt
#include <QObject>

// Cq
#include "cqdocument.h"

/**
	A clipboard. Thi is a singleton - access via instance().
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqClipboard : public QObject
{
Q_OBJECT
public:
	CqClipboard( QObject* parent = NULL );
	virtual ~CqClipboard();
	
	static CqClipboard* instance();

	// clipboard operations
	
	void copy( const CqItem* pItem );		///< Stores copy of item in clipboard, removes old one
	CqItem* get() const;					///< Creates instance of stored intem
	void clear();							///< Clears clipboard
	bool storesItem() const;				///< Returns \b true if any item is stored
	

private:

	// methods
	
	void init();

	// data

	static CqClipboard*	_pInstance;		///< Singleton instance
	QString				_xmlData;		///< Document which stores item
};

#endif // CQCLIPBOARD_H

// EOF


