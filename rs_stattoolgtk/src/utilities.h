/***************************************************************************
 *   Copyright (C) 2006 by Mike Polan                                      *
 *   kanadakid@gmail.com                                                   *
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
// utilities.h: various extra functions

#ifndef UTILITIES_H
#define UTILITIES_H

// necessary includes
#include <glibmm/ustring.h>
#include "io.h"

// Utilities namespace
namespace Utils {

// allocate memory
void* alloc(void *ptr, size_t size);

// convert a glib::ustring to an int
int ustring_to_int(const Glib::ustring &str);

// convert an int to a glib::ustring
Glib::ustring int_to_ustring(int a);

// translate an IOHandler error to string
Glib::ustring translate_io_error(IOHandler::IOError error);

// format a numeric string with comma separators
Glib::ustring format_numeric(const Glib::ustring &str);

}; // namespace Utils

#endif
