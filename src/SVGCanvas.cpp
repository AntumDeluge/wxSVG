//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.2 2005-05-25 12:16:41 ntalex Exp $
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

//////////////////////////////////////////////////////////////////////////////
////////////////////////////// Draw functions ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void wxSVGCanvas::DrawLine(wxSVGLineElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawPolyline(wxSVGPolylineElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawPolygon(wxSVGPolygonElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawRect(wxSVGRectElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawCircle(wxSVGCircleElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawEllipse(wxSVGEllipseElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawPath(wxSVGPathElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  wxSVGCanvasItem* canvasItem = CreateItem(element);
  if (style == NULL)
	style = &element->GetStyle();
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

void wxSVGCanvas::DrawText(wxSVGTextElement* element,
  wxSVGMatrix* matrix, wxCSSStyleDeclaration* style)
{
  if (style == NULL)
	style = &element->GetStyle();
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
