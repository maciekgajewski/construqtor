/***************************************************************************
 *   Copyright (C) 2006 by Autostop                   *
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

///@file gexception.h Basic set of exceptions

#ifndef GEXCEPTION_H
#define GEXCEPTION_H

// qt
#include <QString>

/// Base exception class. Contains field for string, human-readable message.
class GException{
private:
	
	QString _msg;		///< Error message
	
public:
	
	/// Returns exception message
	virtual QString getMessage() const { return _msg; };
	
	/// Initializing  constructor
	explicit GException( const QString& msg ) { _msg = msg; }

	/// Default constructor
	GException(){}

	// destructor
	virtual ~GException(){}
	

};

/// Timeout exception, used in network operations
class GTimeoutError:public GException
{
public:
	GTimeoutError( const QString& msg ) : GException( msg ){}
	virtual ~GTimeoutError(){}
};

/// Dataset exception
class GDatasetError : public GException
{
public:
	GDatasetError( const QString& msg ):GException( msg ){}
	virtual ~GDatasetError(){}
};

/// Configuration error. throwed when required configuration is missing
class GConfigurationError:public GException
{
    public:
    GConfigurationError( const QString& msg ):GException(msg){}
	virtual ~GConfigurationError(){}
};

/// Conversion error - throwed on invalid type conversion, string<->num conversion etc
class GConvertError:public GException
{
    public:
    GConvertError( const QString& msg ):GException(msg){}
	virtual ~GConvertError(){}
};

/// Value error - throwed when some value is out of allowed range
class GValueError:public GException
{
    public:
    GValueError( const QString& msg ):GException(msg){}
	virtual ~GValueError(){}
};

/// Sys error - throwed on OS service error
class GSysError:public GException
{
    public:
    GSysError( const QString& msg ):GException(msg){}
	virtual ~GSysError(){}
};

#endif // !GEXCEPTION_H

// eof
