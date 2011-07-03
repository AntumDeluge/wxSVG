noinst_LTLIBRARIES = libwxsvg_cairo.la
libwxsvg_cairo_la_SOURCES = SVGCanvasCairo.cpp SVGCanvasPathCairo.cpp SVGCanvasTextCairo.cpp SVGCanvasImageCairo.cpp
libwxsvg_cairo_la_LIBADD = $(CAIRO_LIBS)

INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/include/wxSVG\
 $(CAIRO_CFLAGS) $(FREETYPE_CFLAGS)
