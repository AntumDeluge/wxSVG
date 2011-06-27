//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.13 2011-06-27 21:14:14 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvas.h"
#include <math.h>
#include <wx/log.h>

//////////////////////////////////////////////////////////////////////////////
////////////////////////// Create item functions /////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#define WX_SVG_CREATE_USING_PATH(elem_name)\
wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVG##elem_name##Element* element)\
{\
  wxSVGCanvasPath* canvasItem = CreateCanvasPath();\
  canvasItem->Init(*element);\
  return canvasItem;\
}

WX_SVG_CREATE_USING_PATH(Line)
WX_SVG_CREATE_USING_PATH(Polyline)
WX_SVG_CREATE_USING_PATH(Polygon)
WX_SVG_CREATE_USING_PATH(Rect)
WX_SVG_CREATE_USING_PATH(Circle)
WX_SVG_CREATE_USING_PATH(Ellipse)
WX_SVG_CREATE_USING_PATH(Path)

#define WX_SVG_CREATE(elem_name)\
wxSVGCanvasItem* wxSVGCanvas::CreateItem(wxSVG##elem_name##Element* element,\
 const wxCSSStyleDeclaration* style)\
{\
  wxSVGCanvas##elem_name* canvasItem = new wxSVGCanvas##elem_name;\
  canvasItem->Init(*element, style != NULL ? *style : (wxCSSStyleDeclaration&) element->GetStyle());\
  return canvasItem;\
}

WX_SVG_CREATE(Image)
WX_SVG_CREATE(Video)

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
WX_SVG_DRAW(Video)

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

wxSVGGradientElement* wxSVGCanvas::GetGradientElement(const wxSVGSVGElement& svgElem, const wxString& href) {
	if (href.length() == 0 || href[0] != wxT('#') || &svgElem == NULL)
	    return NULL;
	wxSVGGradientElement* elem = (wxSVGGradientElement*) svgElem.GetElementById(href.substr(1));
	return elem != NULL && (elem->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT
			|| elem->GetDtd() == wxSVG_RADIALGRADIENT_ELEMENT) ? elem : NULL;
}

unsigned int wxSVGCanvas::GetGradientStops(const wxSVGSVGElement& svgElem, wxSVGGradientElement* gradElem, float opacity) {
	if (gradElem == NULL)
		return 0;

	// Search for the most referenced gradient (we assume that this is the one that contain stops)
	while (true) {
		wxString grad_href = gradElem->GetHref();
		if (grad_href.length() <= 1 || grad_href.GetChar(0) != wxT('#'))
			break;
		wxSVGElement* tmp = (wxSVGSVGElement*) svgElem.GetElementById(grad_href.substr(1));
		if (tmp == NULL || (tmp->GetDtd() != wxSVG_LINEARGRADIENT_ELEMENT
				&& tmp->GetDtd() != wxSVG_RADIALGRADIENT_ELEMENT))
			break;
		gradElem = (wxSVGGradientElement*) tmp;
	}

	// Count number of stop element
	wxSVGStopElement* stop_elem = (wxSVGStopElement*) gradElem->GetChildren();
	unsigned int stop_count = 0;
	while (stop_elem) {
		if (stop_elem->GetDtd() == wxSVG_STOP_ELEMENT)
			stop_count++;
		stop_elem = (wxSVGStopElement*) stop_elem->GetNext();
	}
	if (stop_count == 0)
		return 0;

	// Allocate enough stops
	AllocateGradientStops(stop_count);
	
	// Fill the stops
	stop_elem = (wxSVGStopElement*) gradElem->GetChildren();
	int i = 0;
	while (stop_elem) {
		if (stop_elem->GetDtd() == wxSVG_STOP_ELEMENT)
			SetStopValue(i++, stop_elem->GetOffset(), stop_elem->GetStopOpacity() * opacity,
					stop_elem->GetStopColor().GetRGBColor());
		stop_elem = (wxSVGStopElement*) stop_elem->GetNext();
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
