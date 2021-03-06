/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                         *
 *  Lib and code porting and building helpers for KLLibs                   *
 *  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the  Free Software Foundation, either  version 3 of the  License, or   *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This  program  is  distributed  in the hope  that it will be useful,   *
 *  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
 *  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have  received a copy  of the  GNU General Public License   *
 *  along with this program. If not, see http://www.gnu.org/licenses/.     *
 *                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef LIBBUILD_HPP
#define LIBBUILD_HPP

/*! \file		libbuild.hpp
 *  \brief	Pomocnicze deklaracje do budowania biblioteki.
 *
 * Zawiera pomocnicze deklaracje mark używanych przy budowaniu biblioteki.
 *
 */

#if defined(USING_QT)

	#include <QtCore/qglobal.h>

	#if defined(KLLIBS_LIBRARY)

		#define KLLIBS_EXPORT Q_DECL_EXPORT

	#else

		#define KLLIBS_EXPORT Q_DECL_IMPORT

	#endif

#else

	#define KLLIBS_EXPORT

#endif

#endif // LIBBUILD_HPP
