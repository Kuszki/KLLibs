/***********************************************************************
 *
 * Lib porting and building helpers for KLLibs
 * Copyright (C) Łukasz "Kuszki" Dróżdż  2015
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 **********************************************************************/

#ifndef LIBBUILD_HPP
#define LIBBUILD_HPP

/*! \file		libbuild.hpp
 *  \brief	Pomocnicze deklaracje do budowania biblioteki.
 *
 * Zawiera pomocnicze deklaracje mark używanych przy budowaniu biblioteki.
 *
 */

#if defined(QT_VERSION)

	#if defined(KLLIBS_LIBRARY)
		#include <QtCore/qglobal.h>
		#define EXPORT Q_DECL_EXPORT
	#else
		#define EXPORT Q_DECL_IMPORT
	#endif

#else

	#define EXPORT

#endif

#endif // LIBBUILD_HPP
