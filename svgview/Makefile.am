bin_PROGRAMS = svgview
svgview_SOURCES = svgview.cpp
svgview_LDADD = ../src/libwxsvg.la

INCLUDES = -I$(top_builddir)/include

run: $(bin_PROGRAMS)
	./$(bin_PROGRAMS)
