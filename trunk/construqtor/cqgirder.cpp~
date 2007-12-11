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

// local
#include "cqsimulation.h"
#include "cqgirder.h"

// ============================== constructor ===============
CqGirder::CqGirder( double length, double width ) : CqPhysicalBox()
{
	setSize( QSizeF(length, width) );
}

// ============================ destructor ==================
CqGirder::~CqGirder()
{
	// nope
}

// ======================== can be selected =============
bool CqGirder::canBeSelected() const
{
	// of course, id simulation permits
	return simulation()->canBeSelected( this );
}

// ========================= can be moved ================
bool CqGirder::canBeMoved() const
{
	// if there is no joins, then yes
	return _joints.empty()  && simulation()->canBeMoved( this );
}

// EOF

