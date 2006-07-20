//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.11 2006-07-20 23:46:14 ntalex Exp $
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
  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style)\
{\
  wxSVGCanvasItem* canvasItem = CreateItem(element);\
  if (style == NULL)\
	style = &element->GetStyle();\
  if (style->GetDisplay() == wxCSS_VALUE_INLINE)\
  	DrawItem(*canvasItem, *matrix, *style, *element->GetOwnerSVGElement());\
  if (IsItemsCached())\
	element->SetCanvasItem(canvasItem);\
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
  wxSVGMatrix* matrix, const wxCSSStyleDeclaration* style)
{
  if (style == NULL)
	style = &element->GetStyle();
  wxSVGCanvasItem* canvasItem = CreateItem(element, style);
  if (style->GetDisplay() == wxCSS_VALUE_INLINE)
  	DrawItem(*canvasItem, *matrix, *style, *element->GetOwnerSVGElement());
  if (IsItemsCached())
	element->SetCanvasItem(canvasItem);
  else
	delete canvasItem;
}

void wxSVGCanvas::DrawCanvasText(wxSVGCanvasText& canvasText,
  wxSVGMatrix& matrix, const wxCSSStyleDeclaration& style, wxSVGSVGElement& svgElem)
{
  for (int i=0; i<(int)canvasText.m_chunks.Count(); i++)
  {
	wxSVGCanvasTextChunk& chunk = canvasText.m_chunks[i];
	chunk.style.SetFillRule(wxCSS_VALUE_EVENODD);
	wxSVGMatrix pathMatrix = matrix.Multiply(chunk.matrix);
	for (unsigned int j=0; j<chunk.chars.Count(); j++)
		DrawItem(*chunk.chars[j].path, pathMatrix, chunk.style, svgElem);
  }
}

unsigned int wxSVGCanvas::GetGradientStops(const wxSVGSVGElement& svgElem,
  const wxString& href, float opacity, const wxSVGElement*& refElem)
{
    refElem = NULL;
    if (href.length() == 0 || href[0] != wxT('#') || &svgElem == NULL)
      return 0;
    
    wxString tmp_href = href;
    tmp_href.Remove(0,1);
    refElem = (wxSVGSVGElement*) svgElem.GetElementById(tmp_href);

    // Gradient filling
    if (!refElem || (refElem->GetDtd() != wxSVG_LINEARGRADIENT_ELEMENT &&
                     refElem->GetDtd() != wxSVG_RADIALGRADIENT_ELEMENT))
      return 0;
      
    // Search for the most referenced gradient
    // (we assume that this is the one that contain stops)
    wxSVGElement* tmp;
    wxSVGLinearGradientElement* reference_gradient = (wxSVGLinearGradientElement*)refElem;
    do {
        tmp = NULL;
        wxString grad_href = reference_gradient->GetHref();
        if (grad_href.Length() > 1 && grad_href.GetChar(0) == wxT('#'))
        {
            grad_href.Remove(0,1);
            tmp = (wxSVGSVGElement*) svgElem.GetElementById(grad_href);
            if (tmp && tmp->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT)
                reference_gradient = (wxSVGLinearGradientElement*)tmp;
        }
    } while (tmp);
        
    //Count number of stop element
    wxSVGElement* stop_elem = (wxSVGElement*)reference_gradient->GetChildren();
    unsigned int stop_count = 0; 
    while (stop_elem)
    {
        if(stop_elem->GetDtd() == wxSVG_STOP_ELEMENT)
            stop_count++;
        stop_elem = (wxSVGElement*)stop_elem->GetNext();
    }
    stop_elem = (wxSVGElement*)reference_gradient->GetChildren();
    if (stop_count == 0)
      return 0;

    // Allocate enough stops
    AllocateGradientStops(stop_count);

    // Fill the stops
    int i = 0;
    while (stop_elem)
    {
        if (stop_elem->GetDtd() == wxSVG_STOP_ELEMENT)
        {
            SetStopValue(
                i++,
                ((wxSVGStopElement*)stop_elem)->GetOffset(),
                ((wxSVGStopElement*)stop_elem)->GetStopOpacity() * opacity,
                ((wxSVGStopElement*)stop_elem)->GetStopColor().GetRGBColor());
        }
        stop_elem = (wxSVGElement*)stop_elem->GetNext();
    }
    return stop_count;
}

void wxSVGCanvas::GetLinearGradientVector(wxSVGPoint& p1, wxSVGPoint& p2,
  const wxSVGLinearGradientElement& gradElem, wxSVGCanvasPath& path)
{
    p1.SetX(gradElem.GetX1().GetAnimVal());
	p1.SetY(gradElem.GetY1().GetAnimVal());
	p2.SetX(gradElem.GetX2().GetAnimVal());
	p2.SetY(gradElem.GetY2().GetAnimVal());
    
    if (gradElem.GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_UNKNOWN ||
        gradElem.GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_OBJECTBOUNDINGBOX)
    {
        wxSVGRect bbox = path.GetBBox();
        p1.SetX(bbox.GetX() + p1.GetX()*bbox.GetWidth());
        p1.SetY(bbox.GetY() + p1.GetY()*bbox.GetHeight());
        p2.SetX(bbox.GetX() + p2.GetX()*bbox.GetWidth());
        p2.SetY(bbox.GetY() + p2.GetY()*bbox.GetHeight());
    }
    
	// Compute gradient transformation matrix
    wxSVGMatrix lg_matrix;
	const wxSVGTransformList& transforms =  gradElem.GetGradientTransform().GetAnimVal();
	for (int i=0; i<(int)transforms.Count(); i++)
        lg_matrix = lg_matrix.Multiply(transforms[i].GetMatrix());
	
	p1 = p1.MatrixTransform(lg_matrix);
	p2 = p2.MatrixTransform(lg_matrix);
}


void wxSVGCanvas::GetRadialGradientTransform(wxSVGPoint& focus,
  wxSVGMatrix& matrix, const wxSVGRadialGradientElement& gradElem,
  wxSVGCanvasPath& path, bool scale)
{
    double r, cx, cy, fx, fy;
	r = gradElem.GetR().GetAnimVal();
	cx = gradElem.GetCx().GetAnimVal();
	cy = gradElem.GetCy().GetAnimVal();
	fx = gradElem.GetFx().GetAnimVal();
	fy = gradElem.GetFy().GetAnimVal();
    
    if (gradElem.GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_UNKNOWN ||
        gradElem.GetGradientUnits().GetAnimVal() == wxSVG_UNIT_TYPE_OBJECTBOUNDINGBOX)
    {
        wxSVGRect bbox = path.GetBBox();
        r = r*sqrt(bbox.GetWidth()*bbox.GetWidth() + bbox.GetHeight()*bbox.GetHeight());
        cx = bbox.GetX() + cx*bbox.GetWidth();
        cy = bbox.GetY() + cy*bbox.GetHeight();
        fx = bbox.GetX() + fx*bbox.GetWidth();
        fy = bbox.GetY() + fy*bbox.GetHeight();
    }

	// Compute gradient transformation matrix
	const wxSVGTransformList& transforms =  gradElem.GetGradientTransform().GetAnimVal();
	for (int i=0; i<(int)transforms.Count(); i++)
		matrix = matrix.Multiply(transforms[i].GetMatrix());
	
	// Apply Gradient parameters to transformation
	matrix = matrix.Translate(cx, cy); //.Scale(r);
    if (scale)
      matrix = matrix.Scale(r);
	
	// Change Focus reference to gradient reference
	focus.SetX((fx - cx) / r);
	focus.SetY((fy - cy) / r);
}
