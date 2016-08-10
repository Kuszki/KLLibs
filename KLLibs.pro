# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
#                                                                         *
#  Light C++ library for AVR CPU or any other machine                     *
#  Copyright (C) 2015  Łukasz "Kuszki" Dróżdż  l.drozdz@openmailbox.org   *
#                                                                         *
#  This program is free software: you can redistribute it and/or modify   *
#  it under the terms of the GNU General Public License as published by   *
#  the  Free Software Foundation, either  version 3 of the  License, or   *
#  (at your option) any later version.                                    *
#                                                                         *
#  This  program  is  distributed  in the hope  that it will be useful,   *
#  but WITHOUT ANY  WARRANTY;  without  even  the  implied  warranty of   *
#  MERCHANTABILITY  or  FITNESS  FOR  A  PARTICULAR  PURPOSE.  See  the   *
#  GNU General Public License for more details.                           *
#                                                                         *
#  You should have  received a copy  of the  GNU General Public License   *
#  along with this program. If not, see http://www.gnu.org/licenses/.     *
#                                                                         *
# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

TARGET	=	kllibs
TEMPLATE	=	lib

CONFIG	+=	c++14

DEFINES	+=	KLLIBS_LIBRARY

SOURCES	+=	script/klscript.cpp \
			script/klvariables.cpp \
			script/klbindings.cpp \
			script/klparser.cpp \
			containers/klmap.cpp \
			containers/kllist.cpp \
			containers/klstring.cpp \
			containers/kltree.cpp

HEADERS	+=	KLLibs.hpp libbuild.hpp \
			script/klscript.hpp \
			script/klvariables.hpp \
			script/klbindings.hpp \
			script/klparser.hpp \
			containers/klmap.hpp \
			containers/kllist.hpp \
			containers/klstring.hpp \
			containers/kltree.hpp

QMAKE_CXXFLAGS	+=	-s -march=native -std=c++14

addons {

	QT		+=	widgets

	DEFINES	+=	USING_QT

	SOURCES	+=	qt/klhighlighter.cpp \
				qt/klscripteditor.cpp \
				qt/klscriptbinding.cpp \
				qt/klparserbinding.cpp

	HEADERS	+=	qt/klhighlighter.hpp \
				qt/klscripteditor.hpp \
				qt/klscriptbinding.hpp \
				qt/klparserbinding.hpp

	TRANSLATIONS	+=	translations/kllibs_pl.ts

} else {

	QT		-=	gui core

}

boost {

	DEFINES	+=	USING_BOOST

}

unix {

	target.path = /usr/lib
	INSTALLS += target

}
