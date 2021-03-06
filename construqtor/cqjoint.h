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
#ifndef CQJOINT_H
#define CQJOINT_H

// Qt
#include <QPointer>

// Box 2D
class b2Joint;

// local
#include "cqitem.h"
class CqPhysicalBody;
class CqWorld;

/**
	Graphics item representign general Box 2D's joint

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqJoint : public CqItem
{
	Q_OBJECT
public:
	// constrution / destruction
	explicit CqJoint( CqItem* parent = NULL );
	virtual ~CqJoint();

	// properties
	
	void setConnectedBodies( CqPhysicalBody* pBody1, CqPhysicalBody* pBody2 );
	const CqPhysicalBody* body1() const { return _pBody1; }
	const CqPhysicalBody* body2() const { return _pBody2; }
	CqPhysicalBody* body1(){ return _pBody1; }
	CqPhysicalBody* body2(){ return _pBody2; }
	
	b2Joint* b2joint() { return _pJoint; }
	const b2Joint* b2joint() const { return _pJoint; }
	
	// info from simulation
	void assureJointCreated();				///< Makes sure that body was created
	
	// info from body
	void breakJoint();						///< Breaks joint
	virtual void breakConections(){ breakJoint(); }	///< breaks all conections - joints etc
	
	virtual int type() const;	///< RTTI
	
	// i/o
	virtual void store( CqElement& element ) const;		///< stores item state 
	virtual void load( const CqElement& element );		///< restores item state 
	
	// tools
	void wakeUpBodies();					///< Wakes bodies up
	
protected:

	// remplementables
	
	virtual b2Joint* createJoint( CqWorld* pWorld ) = 0;
	void destroyJoint( CqWorld* pWorld );
	
	void recreateJoint();			///< Called when joint params is changedm so B2D joint has to be re-created

	// data

private:

	// methods

	void init();					///< Initializes
	
	// data
	
	b2Joint*	_pJoint;			///< Physical joint object
	CqPhysicalBody		*_pBody1, *_pBody2;	///< Bodies connected by joint
};

#endif // CQJOINT_H

// EOF


