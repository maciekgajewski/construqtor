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
#include <QGraphicsSvgItem>
#include <QMessageBox>

// local
#include "gamemanager.h"

// Cq
#include "cqsimulation.h"
#include "cqgroundbody.h"
#include "cqstone.h"
#include "cqpallet.h"
#include "cqgirder.h"
#include "cqdocument.h"
#include "cqelement.h"

// tags
static const char*	TAG_GAME			= "game";	// root element
static const char* 	TAG_SIMULATION		= "simulation";
static const char* 	TAG_INSTRUCTIONS	= "instructions";
static const char* 	TAG_BOX				= "box";;

// ========================================================
GameManager::GameManager(QObject *parent)
 : QObject(parent)
{
	_pSim			= NULL;
	_pBox			= NULL;
	_pInstructions	= NULL;
}

// ========================================================
GameManager::~GameManager()
{
	// nope
}

// ========================================================
void GameManager::startEasyGame()
{
	Q_ASSERT( _pSim );
	startGame( _pSim, 0.2, 0.1, 80 );
}

// ========================================================
void GameManager::startIntermediateGame()
{
	Q_ASSERT( _pSim );
	startGame( _pSim, 0.3, 0.2, 80 );
}

// ========================================================
void GameManager::startHardGame()
{
	Q_ASSERT( _pSim );
	startGame( _pSim, 0.4, 0.4, 80 );
}


// ========================================================
void GameManager::startGame( CqSimulation* pSim, double maxSlope, double stoneSize, int stones )
{
	Q_ASSERT( pSim );
	
	pSim->stop();
	pSim->clear();
	
	// game size
	QRectF worldRect( -250, -100, 500, 200 ); // 500x200 m
	
	// create ground item
	CqGroundBody* pGround = CqGroundBody::randomGround( worldRect, maxSlope );
	
	// find editable and target erea
	// assuming we are going from left to right
	
	double heightAtBegining = pGround->height( worldRect.left() );
	double heightAtEnd = pGround->height( worldRect.right() );
	
	QSizeF areaSize = QSizeF( 20.0, 20.0 );	// 20x20 m. keep it in sync with value in CqGroundBody::randomGround

	QRectF editableArea	= QRectF( QPointF( worldRect.left() + 1.0, heightAtBegining - 1.0 ), areaSize );
	QRectF targetArea	= QRectF( QPointF( worldRect.right() - 1.0 - areaSize.width(), heightAtEnd - 1.0 ), areaSize );
	
	pSim->setWorldRect( worldRect );
	pSim->addGroundItem( pGround );
	pSim->setEditableArea( editableArea );
	pSim->setTargetArea( targetArea );
	
	// add stones
	int stonesCount = stones;
	
	double stoneMargin = areaSize.width() * 2.0;
	for ( int i =0 ; i < stonesCount; i++ )
	{
		CqStone* pStone = CqStone::createRandomStone( stoneSize );
		double stoneX = worldRect.left() + stoneMargin
			+ ( worldRect.width() - 2 * stoneMargin ) * qrand() / RAND_MAX;
			
		double stoneY = pGround->height( stoneX ) + stoneSize;
		
		pStone->setPhysicalPos( QPointF( stoneX, stoneY ) );
		pSim->addGroundItem( pStone );
	}
	
	// add target pallet and box
	double boxX = worldRect.left() + 3.0 + areaSize.width(); // just after forst area
	double boxY = pGround->height( boxX );
	
	CqPallet* pPallet = new CqPallet;
	pPallet->setPhysicalPos( QPointF( boxX, boxY + 0.2 ) );
	pPallet->setEditorFlags(  CqItem::Selectable  );
	
	CqGirder* pBox = new CqGirder;
	pBox->setSize( QSizeF( 1.0, 1.0 ) ); // 1x1 m
	pBox->setMaterial( CqMaterial::wood() );
	pBox->setName( "The Important Package" );
	pBox->loadSvgAppearance( ":/KDE_logo.svg" );
	pBox->setPhysicalPos( QPointF( boxX, boxY + 0.8 ) );
	pBox->setEditorFlags( CqItem::Selectable );
	pBox->setCollisionGroup( CqItem::CollisionAll );

	pSim->addItem( pPallet );
	pSim->addItem( pBox );
	
	// Add instructions item
	QGraphicsSvgItem* pInstructions = new QGraphicsSvgItem( ":/instructions.svg", NULL );
	QRectF defaultRect = pInstructions->boundingRect();
	pInstructions->scale( 10.0 / defaultRect.width(), -10.0 / defaultRect.height() );
	pSim->scene()->addItem( pInstructions );
	pInstructions->setPos( boxX - 2.0, boxY + 10.0 );
	pInstructions->show();
	pInstructions->setZValue( 20.0 );
	
	// run for 5 seconds, to lest stone settle
	pSim->run( 5.0 );
	
	// set these pointers after sim is pre-run
	_pInstructions = pInstructions;
	_pBox = pBox;
	
}
// ===========================================================================
void GameManager::setSimulation( CqSimulation* pSim )
{
	if ( pSim )
	{
		connect( pSim, SIGNAL(simulationStep()), SLOT(simulationStep()) );
	}
	if ( _pSim )
	{
		disconnect( _pSim, 0, this, 0 );
	}
	_pSim = pSim;
}

// ===========================================================================
void GameManager::simulationStep()
{
	if ( _pInstructions )
	{
		delete _pInstructions;
		_pInstructions = NULL;
	}
	
	if ( _pBox && _pSim )
	{
		// TODO don't know why this workaround is needed. It's too late now to fight it
		QRectF ta = _pSim->targetArea();
		QPointF p = _pBox->worldPos();
		if ( ta.contains( p ) )
		//if ( _pSim->isInTargetArea( _pBox->worldPos() ) );
		{
			qDebug("success!");
			_pBox = NULL;
			QMessageBox::information( NULL, "Success!", "You managed to deliver the package, congratulations!" );
		}
	}
}

// ===========================================================================
void GameManager::loadGame( const QString& path )
{
	if ( _pSim )
	{
		CqDocument doc;
		doc.loadFromFile( path );
		
		CqElement root = doc.readElement( TAG_GAME );
		
		CqElement simulation = root.readElement( TAG_SIMULATION );
		_pSim->load( simulation );
		
		_pInstructions = NULL; // TODO create CqSvgItem, read it
		_pBox = root.readItemPointer( TAG_BOX );
	}
}

// ===========================================================================
void GameManager::saveGame( const QString& path )
{
	if ( _pSim )
	{
		CqDocument doc;
		CqElement root = doc.createElement();
		
		CqElement simulation = doc.createElement();
		_pSim->store( simulation );
		
		root.appendElement( TAG_SIMULATION, simulation );
		if ( _pBox )
		{
			root.appendItemPointer( TAG_BOX, _pBox );
		}
		
		doc.appenElement( TAG_GAME, root );
		doc.saveToFile( path );
	}
}

// EOF

