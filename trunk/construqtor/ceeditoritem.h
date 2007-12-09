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
#ifndef CEEDITORITEM_H
#define CEEDITORITEM_H

// Qt
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

// Cq
class CqItem;

/**
	The Editor Item is a special-item associated with CqItem, which paints handles,
	handles input and manipulates Cqtems position and rotation.

	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CeEditorItem : public QGraphicsItem
{
public:
	explicit CeEditorItem( CqItem* pItem );
	virtual ~CeEditorItem();

	virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
	virtual QRectF boundingRect () const;
	
	/// Checks if this point is contained by any handle
	bool handleContains( const QPointF& point) const;

protected:

	// mouse handlers
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent ){ pEvent->ignore(); }
	virtual void mousePressEvent ( QGraphicsSceneMouseEvent * pEvent ){ pEvent->ignore(); }
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * pEvent ){ pEvent->ignore(); }

private:
	
	static const double SIZE;		///< Editor's size
	
	// private classes
	class MoveHandler : public QGraphicsItem
	{
		public:
		static const double SIZE;		///< Move handler size
		/// Constructor
		MoveHandler( CqItem* pItem, CeEditorItem* parent );
		
		virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
		virtual QRectF boundingRect () const;
		
		// mouse handlers
		virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * pEvent );
		virtual void mousePressEvent ( QGraphicsSceneMouseEvent * pEvent );
		virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * pEvent );
		
		CqItem* _pItem;		///< Moved object
		bool	_dragging;	///< Is currently dragging
		
	};
	
	
	// methods
	void createHandles();
	
	// data
	
	CqItem*	_pItem;					///< Associated item
	MoveHandler*	_pMoveHandler;	///< Handler used to move object
};

#endif // CEEDITORITEM_H


// EOF


