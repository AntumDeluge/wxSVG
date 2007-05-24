SUBDIRS = expat

noinst_LTLIBRARIES = libwxsvgxml.la
libwxsvgxml_la_SOURCES = svgxml.cpp svgxmlhelpr.cpp
libwxsvgxml_la_LIBADD = expat/libexpat.la

INCLUDES = -I$(top_builddir)/include/wxSVGXML
