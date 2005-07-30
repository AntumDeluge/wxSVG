if USE_RENDER_AGG
RENDER_DIR = agg
RENDER_LIB = agg/libwxsvg_agg.la
else
RENDER_DIR = libart
RENDER_LIB = libart/libwxsvg_libart.la
RENDER_CFLAGS = $(LIBART_CFLAGS)
endif

SUBDIRS = xml freetype $(RENDER_DIR)

noinst_LTLIBRARIES = libwxsvg.la
libwxsvg_la_SOURCES = SVGDocument.cpp svgctrl.cpp\
  SVGAnimationElement.cpp SVGCircleElement.cpp SVGEllipseElement.cpp\
  SVGFEGaussianBlurElement.cpp SVGFilterElement.cpp SVGImageElement.cpp\
  SVGLineElement.cpp SVGLocatable.cpp SVGMarkerElement.cpp SVGPathElement.cpp\
  SVGPolylineElement.cpp SVGPolygonElement.cpp\
  SVGRectElement.cpp SVGSVGElement.cpp SVGTextContentElement.cpp\
  SVGAngle.cpp SVGColor.cpp GetSVGDocument.cpp SVGLength.cpp\
  SVGMatrix.cpp SVGPaint.cpp SVGPoint.cpp SVGTests.cpp SVGTransform.cpp\
  SVGStylable.cpp SVGTextElement.cpp SVGTransformable.cpp\
  SVGElementInstanceList.cpp SVGLengthList.cpp SVGNumberList.cpp\
  SVGPathSegList.cpp SVGPointList.cpp SVGStringList.cpp\
  SVGTextElement.cpp SVGTransformList.cpp\
  Elements_SetAttribute.cpp CSSValue.cpp CSSStyleDeclaration.cpp\
  SVGCanvas.cpp SVGCanvasItem.cpp NodeList.cpp

libwxsvg_la_LIBADD = xml/libwxxml.la freetype/libwxsvg_freetype.la\
 $(RENDER_LIB)

INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/include/wxSVG\
 $(RENDER_CFLAGS)
