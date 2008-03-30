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
#ifndef CQGIRDER_H
#define CQGIRDER_H

// Qt
#include <QSvgRenderer>

// local
#include "cqphysicalbox.h"


/**
	Girder is game element - nailable and breakable girder

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqGirder : public CqPhysicalBox
{
	Q_OBJECT
public:

	// construction / destruction 
	explicit CqGirder( CqItem* parent = NULL );
	virtual ~CqGirder();
	
	// editor flags
	virtual bool canBeMoved() const;
	virtual bool canBeRotated() const;
	
	virtual bool canConnectHere( const QPointF& /*worldPoint*/ ) { return true; } // connect on entire surface
	
	virtual QString description() const;
	
	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );

	// properties
	void setSvgAppearance( const QByteArray& svg );
	void loadSvgAppearance( const QString& path );
	QByteArray svgApperance() const { return _svgAppearanceCode; } // TODO is this needed?
	
	// i/o
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 
	
private:
	QByteArray		_svgAppearanceCode;		///< Source code of wheel appearance
	QSvgRenderer	_svgAppearance;			///< Wheel appearance
};


#endif // CQGIRDER_H

// EOF


