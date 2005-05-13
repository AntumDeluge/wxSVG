
SUBDIRS = xml libart

noinst_LTLIBRARIES = libwxsvg.la
libwxsvg_la_SOURCES = SVGDocument.cpp svgctrl.cpp\
  SVGAnimationElement.cpp SVGCircleElement.cpp SVGEllipseElement.cpp\
  SVGFEGaussianBlurElement.cpp SVGFilterElement.cpp\
  SVGLineElement.cpp SVGLocatable.cpp SVGMarkerElement.cpp SVGPathElement.cpp\
  SVGPolylineElement.cpp SVGPolygonElement.cpp\
  SVGRectElement.cpp SVGSVGElement.cpp SVGTextContentElement.cpp\
  SVGAngle.cpp SVGColor.cpp GetSVGDocument.cpp SVGLength.cpp\
  SVGMatrix.cpp SVGPaint.cpp SVGPoint.cpp SVGTests.cpp SVGTransform.cpp\
  SVGStylable.cpp SVGTransformable.cpp\
  SVGElementInstanceList.cpp SVGLengthList.cpp SVGNumberList.cpp\
  SVGPathSegList.cpp SVGPointList.cpp SVGStringList.cpp SVGTransformList.cpp\
  Elements_SetAttribute.cpp CSSValue.cpp CSSStyleDeclaration.cpp\
  SVGCanvas.cpp SVGCanvasItem.cpp

libwxsvg_la_LIBADD = xml/libwxxml.la libart/libwxsvg_libart.la

INCLUDES = -I$(top_builddir)/include -I$(top_builddir)/include/wxSVG $(LIBART_CFLAGS)
