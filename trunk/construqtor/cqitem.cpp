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
#include "cqitem.h"

// ========================== constructor ======================
CqItem::CqItem( QGraphicsItem* parent )
	: QGraphicsItem(parent)
{
	_pSimulation = NULL;
}

// ========================== destructor ======================
CqItem::~CqItem()
{
	// none
}

// =========================== simulation step ===================
void CqItem::simulationStep()
{
	// nothing here
}

// =========================== simulationstarted ===================
void CqItem::simulationStarted()
{
	// nothing here
}

// =========================== simulation stopped ===================
void CqItem::simulationStopped()
{
	// nothing here
}

// ============================== dbl click ==================
void CqItem::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	if ( canBeEdited() )
	{
		breakAllJoints();
	}
}

// ============================== mouse release ==================
void CqItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * pEvent )
{
	// use default handler to select/deselect, but 
	setFlag( QGraphicsItem::ItemIsSelectable, canBeSelected() );

	QGraphicsItem::mouseReleaseEvent( pEvent );
}

// =================================== breaks all atached joints ===
void CqItem::breakAllJoints()
{
	// TODO add list of joints to each body
}

// EOF




