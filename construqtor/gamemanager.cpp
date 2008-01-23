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

#include "gamemanager.h"

// Cq
#include "cqsimulation.h"
#include "cqgroundbody.h"
#include "cqstone.h"
#include "cqpallet.h"
#include "cqgirder.h"

// ========================================================
GameManager::GameManager(QObject *parent)
 : QObject(parent)
{
	// nope
}

// ========================================================
GameManager::~GameManager()
{
	// nope
}

// ========================================================
void GameManager::startEasyGame( CqSimulation* pSim )
{
	startGame( pSim, 0.2, 0.1, 80 );
}

// ========================================================
void GameManager::startIntermediateGame( CqSimulation* pSim )
{
	startGame( pSim, 0.3, 0.2, 80 );
}

// ========================================================
void GameManager::startHardGame( CqSimulation* pSim )
{
	startGame( pSim, 0.4, 0.3, 80 );
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
	
	pSim->addItem( pPallet );
	pSim->addItem( pBox );
	
	
	// run for 5 seconds, to lest stone settle
	pSim->run( 5.0 );
	
}

// EOF

