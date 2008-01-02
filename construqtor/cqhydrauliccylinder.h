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
#ifndef CQHYDRAULICCYLINDER_H
#define CQHYDRAULICCYLINDER_H

// local
#include "cqcompounditem.h"
class CqGirder;
class CqPrismaticJoint;

/**
	Hydraulic cylinder - two long bodies connected together with prismatic joint,
	translating along longitudinal axis.
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqHydraulicCylinder : public CqCompoundItem
{
	Q_OBJECT
public:

	// construction / destruction
	CqHydraulicCylinder( QGraphicsItem* pParent = NULL );
	virtual ~CqHydraulicCylinder();

private:

	// data
	CqGirder*			_pBarrel;
	CqGirder*			_pPiston;
	CqPrismaticJoint*	_pMotor;
	
};

#endif // CQHYDRAULICCYLINDER_H

// EOF


