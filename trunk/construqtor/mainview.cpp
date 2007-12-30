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
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>

// CQ
#include "cqitem.h"
#include "cqjoint.h"
#include "cqphysicalbody.h"
#include "cqsimulation.h"
#include "cqrevolutejoint.h"
#include "cqgroundbody.h"

// Local
#include "mainview.h"
#include "ceeditoritem.h"

// constants
static const int RUBBERBAND_DRAG_SENSITIVITY	= 5;	///< number of pixels dragged before rubberband selection starts

// ========================= constructor ======================
MainView::MainView(QWidget* parent): QGraphicsView(parent)
{
	init();
}

// ========================= constructor ======================
MainView::MainView(QGraphicsScene* scene, QWidget* parent): QGraphicsView(scene, parent)
{
	init();
}

// ============================= set simulation =================
/// Sets simulation. Also associates simulatinos's scene with view
void MainView::setSimulation( CqSimulation* pSimulation )
{
	Q_ASSERT( pSimulation );
	
	_pSimulation = pSimulation;
	connect( _pSimulation, SIGNAL(simulationStarted()), SLOT(simulationStarted()));
	connect( _pSimulation, SIGNAL(simulationPaused()), SLOT(simulationPaused()));
	
	setScene( pSimulation->scene() );
	
	// TODO experimental - tunr off scrollbars. DOES NOT WORK
	//setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	//setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	
	setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
	setResizeAnchor( QGraphicsView::AnchorUnderMouse );
}

// ========================== init =============================
void MainView::init()
{
	_pEditor = NULL;
	_pSelectedItem = NULL;
	
	setRenderHint( QPainter::Antialiasing, true );
	_pSimulation = NULL;
	_pGroupSelection = NULL;
	
	_addedObject	= NULL;
	_addedNail		= NULL;
	_mode = SELECTING;
	_viewportInitialized = false;
	_dragging		= false;
	
	// init rubberband
	_rubberbandSelection.setPen( QPen( Qt::blue, 0.0 ) ); // tiny blue frame
	_rubberbandSelection.setBrush( QColor( 0x00, 0x00, 0xff, 0x40 ) ); // 25% opaque blue
	_rubberbandSelection.setZValue( 4.0 );
}

// ========================= destructor ======================
MainView::~MainView()
{
	// nope
}

// ============================= dbl click ===================
void MainView::mouseDoubleClickEvent(QMouseEvent* pEvent )
{
	QGraphicsView::mouseDoubleClickEvent(pEvent);
	
}

// =========================== move =====================
void MainView::mouseMoveEvent(QMouseEvent* pEvent)
{
	// emit scene pos
	QPointF pos = mapToScene( pEvent->pos() );
	emit pointerPos( pos.x(), pos.y() );
	
	// in 'add nail' mode update cursor
	switch ( _mode )
	{
		case ADDING_NAIL:
			if ( canAddNail( mapToScene( pEvent->pos() ) ) )
			{
				setCursor( Qt::CrossCursor );
			}
			else
			{
				unsetCursor();
			}
			break;
			
		case SCROLLING:
			scrollBy( _scrollStart - pEvent->pos() );
			_scrollStart = pEvent->pos();
			break;
			
		case SELECTING:
		{
			if ( _dragging )
			{
				QPoint displacement = mapFromScene( _rubberbandStart ) - pEvent->pos();
				// calcvulate distance as inifinite module of vector
				int distance = qMax( qAbs( displacement.x() ), qAbs( displacement.y() ) );
				if ( distance > RUBBERBAND_DRAG_SENSITIVITY )
				{
					setMode( RUBBERBANDING );
					// show rubberband
					if ( _rubberbandSelection.scene() != scene() )
					{
						scene()->addItem( &_rubberbandSelection );
					}
					
					QSizeF size ( pos.x() - _rubberbandStart.x(), pos.y() - _rubberbandStart.y() );
					_rubberbandSelection.setRect( QRectF( _rubberbandStart, size ) );
					_rubberbandSelection.show();
				}
			}
			break;
		}
			
		case RUBBERBANDING:
		{
			QSizeF size ( pos.x() - _rubberbandStart.x(), pos.y() - _rubberbandStart.y() );
			_rubberbandSelection.setRect( QRectF( _rubberbandStart, size ) );
			_rubberbandSelection.update();
			break;
		}
	
		default:
			unsetCursor();
	}
	
	QGraphicsView::mouseMoveEvent( pEvent );
	
}

// =========================== press =====================
void MainView::mousePressEvent(QMouseEvent* pEvent)
{
	QPointF scenePos = mapToScene( pEvent->pos() );
	
	// hanlde only if outside editor
	if ( _pEditor && _pEditor->handleContains( scenePos ) )
	{
		QGraphicsView::mousePressEvent( pEvent );
		return;
	}
	
	// left button
	if ( pEvent->button() == Qt::LeftButton )
	{
		// add
		switch ( _mode )
		{
		case ADDING_OBJECT:
		{
			if ( _pSimulation->canAddHere( _addedObject, scenePos ) )
			{
				_pSimulation->addItem( _addedObject );
				_addedObject->setPhysicalPos( scenePos );
			}
			_addedObject = NULL;
			setMode( SELECTING );
		}
		break;
		case ADDING_NAIL:
		{
			if ( canAddNail( scenePos ) )
			{
				addNail( scenePos, _addedNail );
				_addedNail = NULL;
				setMode( SELECTING );
			}
		}
		break;
		case SELECTING:
		{
			_rubberbandStart	= scenePos;
			_dragging			= true;
		}
		} // switch
	}
	// cancel adding with right button
	else if ( pEvent->button() == Qt::RightButton )
	{
		setMode( SCROLLING );
		_scrollStart = pEvent->pos();
	}
}

// =========================== release =====================
void MainView::mouseReleaseEvent(QMouseEvent* pEvent)
{
	QPointF scenePos = mapToScene( pEvent->pos() );
	
	// hanlde only if outside editor
	if ( _pEditor && _pEditor->handleContains( scenePos ) )
	{
		QGraphicsView::mouseReleaseEvent( pEvent );
		return;
	}
	
	_dragging = false;
	
	// left button: end dragging
	if ( pEvent->button() == Qt::LeftButton )
	{
		switch ( _mode )
		{
			case SELECTING:
			{
				selectUnderPoint( pEvent->pos() );
				break;
			}
				
			case RUBBERBANDING:
			{
				QSizeF size ( scenePos.x() - _rubberbandStart.x(), scenePos.y() - _rubberbandStart.y() );
				selectGroup( QRectF( _rubberbandStart, size ) );
				setMode( SELECTING );
				break;
			}
				
			default:
				;// ignore
		}
	}
	
	// with right button: stop scrolling
	else if ( pEvent->button() == Qt::RightButton )
	{
		if ( _mode == SCROLLING )
		{
			setMode( SELECTING );
		}
	}
}

// ======================== select under point ===========
void MainView::selectUnderPoint( const QPoint& pos )
{
	Q_ASSERT( _pSimulation );
	// TODO move decision to simualtion
	if ( _pSimulation->isRunning() )
	{
 		return; // no selecting when simulation is running
	}
	
	// unselect all, but remeber last selected
	//unselectAll();
			
	
	QList<QGraphicsItem *> itemList = items( pos );
	QList<CqItem*> selectableItems;
	// simplest implementation - select first selectable CqItem
	foreach( QGraphicsItem* pItem, itemList )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		if ( pCqItem && pCqItem->canBeSelected() )
		{
			selectableItems.append( pCqItem );
		}
	}
	
	// now select
	CqItem* pItemToSelect = NULL;
	if ( ! selectableItems.isEmpty() )
	{
		// how does it works: if currently selected is on the list, select next one
		// otherwise, select first
		
		if ( _pSelectedItem )
		{
			int selectedIndex = selectableItems.indexOf( _pSelectedItem );
			if ( selectedIndex  >= 0 )
			{
				pItemToSelect = selectableItems[ ( selectedIndex + 1 ) % selectableItems.size() ];
			}
			else
			{
				pItemToSelect = selectableItems[ 0 ];
			} 
		}
		else
		{
			pItemToSelect = selectableItems[ 0 ];
		}
	}
	
	selectItem( pItemToSelect ); // NULL unselects
	
}

// ============================================================================
/// Selects only this item
void MainView::selectItem( CqItem* pItem )
{
	if ( pItem != _pSelectedItem )
	{
		// delete current editor
		delete _pEditor;
		_pEditor = NULL;
		
		if ( _pSelectedItem )
		{
			_pSelectedItem->setSelected( false );
			_pSelectedItem->update();
			
			// if group selection unselected - remove all items from group
			if ( _pSelectedItem == _pGroupSelection )
			{
				delete _pGroupSelection;
				_pGroupSelection = NULL;
			}
		}
		
		if ( pItem )
		{
			pItem->setSelected( true );
			pItem->update();
			// create editor for selected
			_pEditor = new CeEditorItem( pItem );
			_pEditor->setMatrix( matrix().inverted() ); //set inverted transform
			emit selectedDescription( pItem->description() ); // TODO remove this
		}
		else
		{
			emit selectedDescription( "" ); // TODO remove this
		}
		
		
		_pSelectedItem = pItem;
	}
}

// ========================= unselect all ================
void MainView::unselectAll()
{
	_pSelectedItem = NULL;
	
	QList<QGraphicsItem *> itemList = items();
	foreach( QGraphicsItem* pItem, itemList )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		
		if ( pCqItem )
		{
			pCqItem->setSelected( false );
		}
	}
	
	// destroy editor
	delete _pEditor;
	_pEditor = NULL;
	
	emit selectedDescription( "" );
}

// ======================= break joint under point ==============
void MainView::breakJointUnderPoint( const QPoint& pos )
{
	Q_ASSERT( _pSimulation );
	// TODO move decision to simualtion
	if ( _pSimulation->isRunning() )
	{
 		return; // no breaking joints when simulation is running
	}
	
	QList<QGraphicsItem *> itemList = items( pos );
	// simplest implementation - break first body or joint discovered
	foreach( QGraphicsItem* pItem, itemList )
	{
		// top level body?
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( pItem );
		if ( pBody && ! pBody->physicalParent() )
		{
			pBody->breakAllJoints();
			break;
		}
		// top level joint?
		CqJoint* pJoint = dynamic_cast<CqJoint*>( pItem );
		if ( pJoint && ! pJoint->physicalParent() )
		{
			pJoint->breakJoint();
		}
	}
}

// ======================= wheel event  ==============
void MainView::wheelEvent ( QWheelEvent * pEvent )
{
	// caclulate scale factor
	double factor = 1.0;
	int delta = pEvent->delta();
	
	if ( delta > 0 )
	{
		factor = 1.0 + delta/1000.0;
	}
	else if ( delta < 0 )
	{
		factor = 1.0 / ( 1.0 - delta/1000.0 );
	}
	
	setUpdatesEnabled( false );
	scale( factor, factor );
	adjustScale();
	setUpdatesEnabled( true );
	
	// update editor if needed
	if ( _pEditor )
	{
		_pEditor->setMatrix( matrix().inverted() ); //set inverted transform
	}
}

// ============================ resize event ===================
void MainView::resizeEvent( QResizeEvent* /*pEvent*/ )
{
	adjustScale();
}

// ============================= scroll by ====================================
/// Scrols content by 'delta' pixels
void MainView::scrollBy( const QPoint& delta )
{
	QScrollBar* pH = horizontalScrollBar();
	QScrollBar* pV = verticalScrollBar();
	
	Q_ASSERT( pH && pV );
	
	pH->setValue( pH->value() + delta.x() );
	pV->setValue( pV->value() + delta.y() );
	
}

// =============================== adjust scale =============================
void MainView::adjustScale()
{
	QRect sceneRectPixels = mapFromScene( sceneRect() ).boundingRect();
	
	double sx = double(sceneRectPixels.width()) / viewport()->width();
	double sy = double(sceneRectPixels.height()) / viewport()->height();
	
	double min = qMin( sx, sy );
	// make sure both scales are at least 1
	if ( min < 1.0 )
	{
		scale( 1.0 / min, 1.0 / min );
	}
}

// ========================= on simulation started ===========================
void MainView::simulationStarted()
{
	unselectAll();
}

// ========================= on simulation paused ===========================
void MainView::simulationPaused()
{
	// nope
}

// ============================ tool: add object ==============================
void MainView::toolAddObject( CqItem* pItem )
{
	Q_ASSERT( pItem );
	
	// cancel current adding
	delete _addedObject;
	setMode( ADDING_OBJECT );
	_addedObject = pItem;
	
}

// ============================ show event ==============================
void MainView::showEvent( QShowEvent* /*pEvent*/ )
{
	if ( ! _viewportInitialized )
	{
		QList<CqItem*> grounds = _pSimulation->groundItems();
		if ( grounds.size() )
		{
			CqGroundBody* pGround = dynamic_cast<CqGroundBody*>(grounds[0]);
			if ( pGround )
			{
				QRectF worldRect = _pSimulation->worldRect();
				double startHeight = pGround->height( worldRect.left() );
				ensureVisible( worldRect.left() + 2, startHeight - 5, 12, 12 );
				_viewportInitialized = true;
			}
		}
	}
}

// ============================ can add nail ==============================
bool MainView::canAddNail( const QPointF& point ) const
{
	// look for 'nailable' objects here
	QList<QGraphicsItem *> itemsHere = items( mapFromScene( point ) );
	
	int iNailableHere = 0;
	foreach( QGraphicsItem* pItem, itemsHere )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		// connect only top-level items
		if ( pCqItem && ( pCqItem->physicalParent() == NULL ) && pCqItem->canConnectHere( point ) )
		{
			iNailableHere++;
		}
	}
	
	//qDebug("nailable here: %d", iNailableHere ); // TODO remove
	return iNailableHere == 2;
}

// ============================ add nail ==============================
void MainView::addNail( QPointF& point, CqRevoluteJoint* pNail )
{
	// look for 'nailable' objects here
	QList<QGraphicsItem *> itemsHere = items( mapFromScene( point ) );
	
	int iNailableHere	= 0;
	CqPhysicalBody*		pBody[2];
	foreach( QGraphicsItem* pItem, itemsHere )
	{
		CqItem* pCqItem = dynamic_cast<CqItem*>( pItem );
		if ( pCqItem && ( pCqItem->physicalParent() ==  NULL ) && pCqItem->canConnectHere( point ) )
		{
			// check
			if ( iNailableHere == 2 )
			{
				delete pNail; // TODO this stinks
				return; // bail out!
			}
		
			pBody[ iNailableHere++ ] = pCqItem->bodyHere( point );
		}
		
	}
	
	// create nail
	pNail->setConnectedBodies( pBody[0], pBody[1] );
	pNail->setWorldPos( point );
	_pSimulation->addItem( pNail );
	
}

// ============================ tool - add nail =======================
void MainView::toolAddNail( CqRevoluteJoint* pNail )
{
	setMode( ADDING_NAIL );
	_addedNail = pNail;
}

// ============================ set mode ===============================
void MainView::setMode( Mode mode )
{
	if ( mode != _mode )
	{
		// cleanup after last mode
		if ( _mode == ADDING_NAIL )
		{
			delete _addedNail;
			_addedNail = NULL;
			selectItem( NULL );
			unsetCursor();
		}
		else if ( _mode == ADDING_OBJECT )
		{
			delete _addedObject;
			_addedObject = NULL;
			selectItem( NULL );
		}
		else if ( _mode == SCROLLING )
		{
			unsetCursor();
		}
		else if ( _mode == RUBBERBANDING )
		{
			_rubberbandSelection.hide();
		}
		
		_mode = mode;
		
		// prepare new mode
		if ( _mode == SCROLLING )
		{
			setCursor( Qt::ClosedHandCursor );
		}
	}
}

// =============================== delete selected ==============
void MainView::toolDeleteSelected()
{
	if ( _pSelectedItem )
	{
		// delete editor
		delete _pEditor;
		_pEditor = NULL;
		// delte item
		delete _pSelectedItem;
		unselectAll(); // TODO or use other way to announce selection changed
	}
}

// ================================ break joins in selected item ===
void MainView::toolBreakSelected()
{
	if ( _pSelectedItem )
	{
		// body?
		CqPhysicalBody* pBody = dynamic_cast<CqPhysicalBody*>( _pSelectedItem );
		if ( pBody  )
		{
			pBody->breakAllJoints();
		}
		// joint?
		CqJoint* pJoint = dynamic_cast<CqJoint*>( _pSelectedItem );
		if ( pJoint )
		{
			pJoint->breakJoint();
		}
	}
}

// ============================ select group ============================
void MainView::selectGroup( const QRectF& rect )
{
	// TODO ok, this is not simple task: 1. create 'group selection item', move all items iside rect to the group
	
	QList<QGraphicsItem *> items = scene()->items( rect, Qt::ContainsItemShape );
	QList<CqItem*> selectedItems;
	
	foreach( QGraphicsItem* pItem, items )
	{
		CqItem* pCqItem = dynamic_cast< CqItem* >( pItem );
		
		if ( pCqItem && ! pCqItem->physicalParent() && pCqItem->canBeSelected() )
		{
			selectedItems.append( pCqItem );
		}
	}

	// do we need to create group?
	if ( items.size() == 0 )
	{
		// select nothing
		selectItem( NULL );
	}
	else if ( selectedItems.size() == 1 )
	{
		selectItem( selectedItems[ 0 ] );
	}
	else
	{
		// delete any current group selection
		delete _pGroupSelection;
		_pGroupSelection = NULL;
		
		_pGroupSelection = new CqGroupItem( selectedItems );
		_pSimulation->addItem( _pGroupSelection );
		_pGroupSelection->show();
		_pGroupSelection->setName( "Group selection" );
		
		selectItem( _pGroupSelection );
	}
}

// EOF

