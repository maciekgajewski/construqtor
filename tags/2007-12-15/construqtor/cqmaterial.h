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
#ifndef CQMATERIAL_H
#define CQMATERIAL_H

// box 2d
class b2ShapeDef;

/**
	Material description record.
	
	@author Maciek Gajewski <maciej.gajewski0@gmail.com>
*/
class CqMaterial
{
public:
	
	// construction / destruction
	CqMaterial( double d = 1.0, double f = 0.7, double r = 0.1){ density = d; friction = f; restitution = r;}
	~CqMaterial(){}

	enum Type { Steel, Rubber, Wood, Custom };
	
	Type type;					/// Well known material type
	
	double friction;			///< Friction [0.0->1.0]
	double restitution;			///< Restotution ("bounciness")[0.0->1.0]
	double density;				///< Density [kg/m2]
	
	/// Copies material spec to shape
	void copyToShapeDef( b2ShapeDef* pShape ) const;
	
	/// Rubber definiton
	inline static CqMaterial rubber()
	{
		CqMaterial m;
		m.density		= 1.0;
		m.friction		= 1.0;
		m.restitution	= 0.5;
		
		m.type = Rubber;
		
		return m;
	}
	
	/// Steel definition
	inline static CqMaterial steel()
	{
		CqMaterial m;
		m.density		= 5.0;
		m.friction		= 0.2;
		m.restitution	= 0.3;
		
		m.type = Steel;
		
		return m;
	}

	/// Wood definition
	inline static CqMaterial wood()
	{
		CqMaterial m;
		m.density		= 2.0;
		m.friction		= 0.7;
		m.restitution	= 0.1;
		
		m.type = Wood;
		
		return m;
	}
};

#endif // CQMATERIAL_H

// EOF


