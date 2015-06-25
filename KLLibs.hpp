/***********************************************************************
 *
 * KLLibs - Lightweight C++ multiplatform libs
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

/*! \file		KLLibs.hpp
 *  \brief	Pomocnicze deklaracje.
 *
 * Dołącza cały projekt przy pomocy jednego pliku.
 *
 */

/*! \page		links Linki
 *
 *  \section	source Kod źródłowy
 *
 * - [Projekt w serwisie GitHub](https://github.com/Kuszki/KLLibs)
 *
 *  \section	license Licencja
 *
 * - [GNU GPL v2 angielski](http://www.gnu.org/licenses/gpl-2.0.html)
 * - [GNU GPL v2 polski](http://gnu.org.pl/text/licencja-gnu.html)
 *
 *  \section	author Autor
 *
 * - [Profil GitHub](https://github.com/Kuszki)
 * - [e-mail](l.drozdz@o2.pl)
 *
 */

#ifndef KLLIBS_GLOBAL_HPP
#define KLLIBS_GLOBAL_HPP

#include "libbuild.hpp"

#include "containers/kllist.hpp"
#include "containers/klmap.hpp"
#include "containers/klstring.hpp"
#include "containers/kltree.hpp"

#include "script/klbindings.hpp"
#include "script/klparser.hpp"
#include "script/klscript.hpp"
#include "script/klvariables.hpp"

#if defined(QT_VERSION)
#include "qt/klhighlighter.hpp"
#include "qt/klscripteditor.hpp"
#endif

#endif // KLLIBS_GLOBAL_HPP
