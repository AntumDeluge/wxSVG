//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.7 2006-01-07 20:44:09 ntalex Exp $
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
	DrawItem(*chunk.path, pathMatrix, chunk.style, svgElem);
  }
}

unsigned int wxSVGCanvas::GetGradientStops(const wxSVGSVGElement& svgElem,
  const wxString& href, float overall_opacity, const wxSVGElement*& refElem)
{
    if (href.length() != 0 && href[0] == wxT('#'))
    {
        wxString tmp_href = href;
        tmp_href.Remove(0,1);
        refElem = (wxSVGSVGElement*) svgElem.GetElementById(tmp_href);
  
        // Linear gradient filling
        if (refElem && (refElem->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT ||
            refElem->GetDtd() == wxSVG_RADIALGRADIENT_ELEMENT))
        {
            // Search for the most referenced gradient
            // (we assume that this is the one that contain stops)
            wxSVGElement* tmp;
            wxSVGLinearGradientElement* reference_gradient = (wxSVGLinearGradientElement*)refElem;
            do {
                tmp = NULL;
                wxString grad_href = reference_gradient->GetHref();
                if(	grad_href.Length() > 1 && grad_href[0] == wxT('#'))
                {
                    grad_href.Remove(0,1);
                    tmp = (wxSVGSVGElement*) svgElem.GetElementById(grad_href);
                    if (tmp && tmp->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT)
                        reference_gradient = (wxSVGLinearGradientElement*)tmp;
                }
            } while(tmp);
            
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
  
            // Allocate enough stops
            AllocateGradientStops(stop_count);
  
            // Fill the stops
            int i = 0;
            while (stop_elem)
            {
                if(stop_elem->GetDtd() == wxSVG_STOP_ELEMENT)
                {
                    SetStopValue(
                        i++,
                        ((wxSVGStopElement*)stop_elem)->GetOffset(),
                        ((wxSVGStopElement*)stop_elem)->GetStopOpacity() * overall_opacity,
                        ((wxSVGStopElement*)stop_elem)->GetStopColor().GetRGBColor());
                }
                stop_elem = (wxSVGElement*)stop_elem->GetNext();
            }
            return stop_count;
        }
    }
    return 0;
}

void wxSVGCanvas::GetLinearGradientVector(wxSVGPoint& p1, wxSVGPoint& p2, wxSVGMatrix& matrix, const wxSVGLinearGradientElement& gradElem)
{

//					TODO : implement gradientUnits parsing in SVGGradientElement setattribute
//					double affine[6];
//					if (rlg->obj_bbox) {
//						affine[0] = ctx->x1 - ctx->x0;
//						affine[1] = 0.;		
//						affine[2] = 0.;
//						affine[3] = ctx->y1 - ctx->y0;
//						affine[4] = ctx->x0;
//						affine[5] = ctx->y0;
//				
//					} else {
//						for (i = 0; i < 6; i++)
//							affine[i] = ctx->affine[i];
//					}

	// Compute gradient transformation matrix
	wxSVGMatrix lg_matrix;
	const wxSVGTransformList& transforms =  gradElem.GetGradientTransform().GetBaseVal();
	for (int i=0; i<(int)transforms.Count(); i++)
		lg_matrix = lg_matrix.Multiply(transforms[i].GetMatrix());

	p1.SetX(gradElem.GetX1().GetBaseVal());
	p1.SetY(gradElem.GetY1().GetBaseVal());
	p2.SetX(gradElem.GetX2().GetBaseVal());
	p2.SetY(gradElem.GetY2().GetBaseVal());
	p1 = p1.MatrixTransform(lg_matrix);
	p2 = p2.MatrixTransform(lg_matrix);
	p1 = p1.MatrixTransform(matrix);
	p2 = p2.MatrixTransform(matrix); 
}


void wxSVGCanvas::GetRadialGradientTransform(wxSVGPoint& Focus, wxSVGMatrix& matrix, const wxSVGRadialGradientElement& gradElem)
{
//					TODO : implement gradientUnits parsing in SVGGradientElement setattribute

	// Compute gradient transformation matrix
	wxSVGMatrix rg_matrix;
	const wxSVGTransformList& transforms =  gradElem.GetGradientTransform().GetBaseVal();
	for (int i=0; i<(int)transforms.Count(); i++)
		rg_matrix = rg_matrix.Multiply(transforms[i].GetMatrix());
	
	// Apply Gradient parameters to transformation
	double R, Cx, Cy, Fx, Fy;
	R = (wxSVGLength)gradElem.GetR();
	Cx = (wxSVGLength)gradElem.GetCx();
	Cy = (wxSVGLength)gradElem.GetCy();
	Fx = (wxSVGLength)gradElem.GetFx();
	Fy = (wxSVGLength)gradElem.GetFy();
	matrix = matrix.Multiply(rg_matrix).Translate(Cx,Cy).Scale(R);
	
	// Change Focus reference to gradient reference
	Focus.SetX((Fx - Cx) / R);
	Focus.SetY((Fy - Cy) / R);

}
