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
#include "cqsimulation.h"
#include "cqgirder.h"
#include "cqitemfactory.h"

CQ_ADD_TO_FACTORY( CqGirder );

// ============================== constructor ===============
CqGirder::CqGirder( CqItem* parent ) : CqPhysicalBox( parent )
{
	setEditorFlags( editorFlags() | Selectable | Movable | Rotatable );
	setName( "Girder" );
}

// ============================ destructor ==================
CqGirder::~CqGirder()
{
	// nope
}

// ========================= can be moved ================
bool CqGirder::canBeMoved() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalBox::canBeMoved();
}

// ========================= can be rotated ================
bool CqGirder::canBeRotated() const
{
	// if there is no joins, then yes
	return _joints.empty()  && CqPhysicalBox::canBeRotated();
}

// EOF

