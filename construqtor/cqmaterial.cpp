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
// Qt
#include <QtGlobal>

// Box 2D
#include "b2Shape.h"

// local
#include "cqmaterial.h"
#include "cqelement.h"
	
static const char* TAG_FRICTION		= "friction";
static const char* TAG_DENSITY		= "density";
static const char* TAG_RESTITUTION	= "restitution";
static const char* TAG_TYPE			= "type";

// ====================== copy to shape derf ================
void CqMaterial::copyToShapeDef( b2ShapeDef* pShape ) const
{
	Q_ASSERT( pShape );
	
	pShape->density = density;
	pShape->friction = friction;
	pShape->restitution = restitution;
}

// ============================= store =====================
void CqMaterial::store( CqElement& element ) const
{
	element.appendDouble( TAG_FRICTION, friction );
	element.appendDouble( TAG_DENSITY, density );
	element.appendDouble( TAG_RESTITUTION, restitution );
	element.appendInt( TAG_TYPE, int(type) );
}

// ============================== load =====================
void CqMaterial::load( const CqElement& element )
{
	friction = element.readDouble( TAG_FRICTION );
	density = element.readDouble( TAG_DENSITY );
	restitution = element.readDouble( TAG_RESTITUTION );
	type = (Type)element.readInt( TAG_TYPE );
}

// EOF
