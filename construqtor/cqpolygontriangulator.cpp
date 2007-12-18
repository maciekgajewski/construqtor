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
extern "C" {
#include "gpc.h"
}

#include "cqpolygontriangulator.h"

// ==================== constructor ==========
CqPolygonTriangulator::CqPolygonTriangulator()
{
}

// ===================== destructor ===========
CqPolygonTriangulator::~CqPolygonTriangulator()
{
}


// ====================== triangulate =============
/// Takes any polygon, and returns list of triangles
QList< QPolygonF > CqPolygonTriangulator::triangulate( const QPolygonF& polygon )
{
	Q_ASSERT( polygon.size() >= 3 ); // don't deal with degenerated polygons
	
	QList< QPolygonF > triangles;
	
	if ( polygon.size() == 3)
	{
		triangles.append( polygon );
		return triangles;
	}
	
	// create GPC polygon from QPolygonF
	gpc_vertex_list	vertexList;
	gpc_polygon		gpcpolygon;
	vertexList.num_vertices	= polygon.size();
	vertexList.vertex		= new gpc_vertex[ polygon.size() ];
	
	for( int i = 0; i < vertexList.num_vertices; i++ )
	{
		vertexList.vertex[i].x = polygon[i].x();
		vertexList.vertex[i].y = polygon[i].y();
	}
	
	gpcpolygon.num_contours		= 1;
	gpcpolygon.hole				= NULL;
	gpcpolygon.contour			 = &vertexList;
	
	// request triangles
	gpc_tristrip tristrip;
	
	gpc_polygon_to_tristrip( &gpcpolygon, &tristrip );
	
	// create triangles from tristrups
	for( int s = 0; s < tristrip.num_strips; s++ )
	{
		gpc_vertex_list& strip	= tristrip.strip[s];
		int numTriangles		= strip.num_vertices - 2;
		for ( int t = 0; t < numTriangles; t++ )
		{
			QPolygonF triangle;
			triangle.append( QPointF( strip.vertex[t].x,	strip.vertex[t].y ) );
			triangle.append( QPointF( strip.vertex[t+1].x,	strip.vertex[t+1].y ) );
			triangle.append( QPointF( strip.vertex[t+2].x,	strip.vertex[t+2].y ) );
			
			triangles.append( triangle );
		}
	}
	
	// release polygon
	delete[] vertexList.vertex;
	gpc_free_tristrip( &tristrip );
	
	return triangles;
}
