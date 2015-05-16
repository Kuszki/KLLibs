#-------------------------------------------------
#
# Project created by QtCreator 2015-04-23T21:23:53
#
#-------------------------------------------------

QT		-=	gui

TARGET	=	KLLibs
TEMPLATE	=	lib

CONFIG	+=	c++11

DEFINES	+=	KLLIBS_LIBRARY

SOURCES	+=	script/klscript.cpp \
			script/klvariables.cpp \
			script/klbindings.cpp \
			containers/klmap.cpp \
			containers/kllist.cpp \
			containers/klstring.cpp \
			containers/kltree.cpp \
			script/klparser.cpp

HEADERS	+=	KLLibs.hpp \
			script/klscript.hpp \
			script/klvariables.hpp \
			script/klbindings.hpp \
			containers/klmap.hpp \
			containers/kllist.hpp \
			containers/klstring.hpp \
			containers/kltree.hpp \
			script/klparser.hpp

QMAKE_CXXFLAGS	+=	-s -fomit-frame-pointer -march=native

unix
{
	target.path = /usr/lib
	INSTALLS += target
}
