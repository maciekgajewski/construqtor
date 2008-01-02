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
#ifndef CQWHEEL_H
#define CQWHEEL_H

// Qt
#include <QByteArray>
#include <QSvgRenderer>

// local
#include "cqphysicaldisk.h"

/**
	Wheel is game element - simple unsuspended nailable wheel

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqWheel : public CqPhysicalDisk
{
	Q_OBJECT
	
	Q_PROPERTY( double diameter READ diameter WRITE setDiameter DESIGNABLE true );
	Q_PROPERTY( double connectableDiameter READ connectableDiameter WRITE setConnectableDiameter DESIGNABLE true );
public:

	// construction / destruction 
	CqWheel( double diameter = 0.0 );
	CqWheel( QGraphicsItem* parent );
	virtual ~CqWheel();
	
	// editor flags
	virtual bool canBeMoved() const;
	virtual bool canBeRotated() const;
	virtual bool canConnectHere( const QPointF& /*worldPoint*/ ) { return true; } // connect on entire surface

	// graphics item
	// operations 
	virtual void paint
		( QPainter * painter
		, const QStyleOptionGraphicsItem * option
		, QWidget * widget = 0 );
		
    virtual QRectF boundingRect() const;

	// properties
	void setSvgAppearance( const QByteArray& svg );
	QByteArray svgApperance() const { return _svgAppearanceCode; } // TODO is this needed?

	double connectableDiameter() const { return _connectableDiameter; }
	void setConnectableDiameter( double d ) { _connectableDiameter = d; }

	// i/o
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 

private:

	void init();
	
	QByteArray		_svgAppearanceCode;		///< Source code of wheel appearance
	QSvgRenderer	_svgAppearance;			///< Wheel appearance
	
	double	_connectableDiameter;			///< diameter of connectable sub-disk
};


#endif	// CQWHEEL_H

// EOF

