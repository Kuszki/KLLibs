#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T21:23:53
#
#-------------------------------------------------

TARGET	=	KLLibs
TEMPLATE	=	lib

CONFIG	+=	c++11 shared

DEFINES	+=	KLLIBS_LIBRARY

SOURCES	+=	script/klscript.cpp \
			script/klvariables.cpp \
			script/klbindings.cpp \
			containers/klmap.cpp \
			containers/kllist.cpp \
			containers/klstring.cpp \
			containers/kltree.cpp \
			script/klparser.cpp

HEADERS	+=	KLLibs.hpp libbuild.hpp \
			script/klscript.hpp \
			script/klvariables.hpp \
			script/klbindings.hpp \
			containers/klmap.hpp \
			containers/kllist.hpp \
			containers/klstring.hpp \
			containers/kltree.hpp \
			script/klparser.hpp

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native

addons {

	QT		+=	widgets

	SOURCES	+=	qt/klhighlighter.cpp \
				qt/klscripteditor.cpp \
				qt/klscriptbinding.cpp \
				qt/klparserbinding.cpp

	HEADERS	+=	qt/klhighlighter.hpp \
				qt/klscripteditor.hpp \
				qt/klscriptbinding.hpp \
				qt/klparserbinding.hpp

	TRANSLATIONS	+=	kllibs_pl.ts

} else {

	QT		-=	gui core

}

unix {

	target.path = /usr/lib
	INSTALLS += target

}
