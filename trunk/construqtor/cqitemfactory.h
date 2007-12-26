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
#ifndef CQITEMFACTORY_H
#define CQITEMFACTORY_H

// Qt
#include <QMap>
#include <QString>

// local
class CqItem;

/**
	Global item factory.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqItemFactory
{
public:

	/// Creator interface - inherited by creator objects for each 
	/// class known to the factory
	class Creator 
	{
	public:
		virtual CqItem* createObject() = 0;
	};

	static void addCreator( const QString& className, Creator* pCreator );
	static CqItem* createItem( const QString& className );
	
private:

	static QMap< QString, Creator* > _creators;
};

// Use this macro in .cpp of CqItem - derrived objects
#define CQ_ADD_TO_FACTORY( classname ) \
	class Creator_ ## classname : public CqItemFactory::Creator \
	{ \
	public: \
		Creator_ ## classname () { CqItemFactory::addCreator( #classname, this ); } \
		virtual CqItem* createObject(){ return new classname (); } \
	}; \
	Creator_ ## classname __staticClassCreator ## classname;

#endif // CQITEMFACTORY_H

// EOF


