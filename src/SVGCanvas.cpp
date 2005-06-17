//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.4 2005-06-17 13:24:50 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvas.h"
#include <math.h>
#include <wx/log.h>

//////////////////////////////////////////////////////////////////////////////
////////////////////////// Create item functions /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGLineElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGPolylineElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGPolygonElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGRectElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGCircleElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGEllipseElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGPathElement* element)
{
  wxSVGCanvasPath* canvasPath = CreateCanvasPath();
  canvasPath->Init(*element);
  return canvasPath;
}

wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVGImageElement* element)
{
  wxSVGCanvasImage* canvasImage = new wxSVGCanvasImage;
  canvasImage->Init(*element);
  return canvasImage;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Draw functions ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define WX_SVG_DRAW(elem_name)\
void wxSVGCanvas::Draw##elem_name(wxSVG##elem_name##Element* element,\
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)\
{\
  wxSVGCanvasItem* canvasItem = CreateItem(element);\
  if (style == NULL)\
	style = (wxCSSStyleDeclaration*) &element->GetStyle();\
  DrawItem(*canvasItem, *matrix, *style);\
  if (IsItemsCached())\
  {\
	if (element->GetCanvasItem())\
	  delete element->GetCanvasItem();\
	element->SetCanvasItem(canvasItem);\
	element->SetScreenCTM(*matrix);\
  }\
  else\
	delete canvasItem;\
}

WX_SVG_DRAW(Line)
WX_SVG_DRAW(Polyline)
WX_SVG_DRAW(Polygon)
WX_SVG_DRAW(Rect)
WX_SVG_DRAW(Circle)
WX_SVG_DRAW(Ellipse)
WX_SVG_DRAW(Path)
WX_SVG_DRAW(Image)

void wxSVGCanvas::DrawText(wxSVGTextElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  if (style == NULL)
	style = (wxCSSStyleDeclaration*) &element->GetStyle();
  wxSVGCanvasItem* canvasItem = CreateItem(element, style);
  DrawItem(*canvasItem, *matrix, *style);
  if (IsItemsCached())
  {
	if (element->GetCanvasItem())
	  delete element->GetCanvasItem();
	element->SetCanvasItem(canvasItem);
	element->SetScreenCTM(*matrix);
  }
  else
	delete canvasItem;
}

