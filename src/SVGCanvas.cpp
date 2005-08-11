//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvas.cpp
// Purpose:     wxSVGCanvas - Base class for SVG renders (backends)
// Author:      Alex Thuering
// Created:     2005/05/04
// RCS-ID:      $Id: SVGCanvas.cpp,v 1.4.2.2 2005-08-11 16:35:46 etisserant Exp $
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
  if (style->GetDisplay() == wxCSS_VALUE_INLINE)\
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
  if (style->GetDisplay() == wxCSS_VALUE_INLINE)
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

unsigned int wxSVGCanvas::GetGradientStops(const wxString& href, float overall_opacity, const wxSVGElement*& refElem)
{
    if (href.length() != 0 && href[0] == wxT('#'))
    {
    	wxString tmp_href = href;
    	tmp_href.Remove(0,1);
        refElem = m_document->GetElementById(tmp_href);

        // Linear gradient filling
        if (refElem && (refElem->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT || refElem->GetDtd() == wxSVG_RADIALGRADIENT_ELEMENT))
        {
        	// Search for the most referenced gradient
        	// (we assume that this is the one that contain stops)
        	// TODO move that code to SVGCanvas
        	wxSVGElement* tmp;
        	wxSVGLinearGradientElement* reference_gradient = (wxSVGLinearGradientElement*)refElem;
        	do{
        		tmp = NULL;
        		wxString grad_href = reference_gradient->GetHref();
        		if(	grad_href.length() != 0 && grad_href[0] == wxT('#')) 
		        	grad_href.Remove(0,1);
        			tmp=m_document->GetElementById(grad_href);
        			if(tmp && tmp->GetDtd() == wxSVG_LINEARGRADIENT_ELEMENT)
        				reference_gradient = (wxSVGLinearGradientElement*)tmp;
        	}while(tmp);
        	
        	//Count number of stop element
        	wxSVGElement* stop_elem = (wxSVGElement*)reference_gradient->GetChildren();
        	unsigned int stop_count = 0; 
        	while(stop_elem)
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
        	while(stop_elem)
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

void wxSVGCanvas::GetLinearGradientVector(double& x1, double& y1, double& x2, double& y2, double affine[6], const wxSVGLinearGradientElement& gradElem)
{
	double fx1, fy1, fx2, fy2;

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
	
	double lg_affine[6];
	lg_affine[0] = lg_matrix.GetA();
	lg_affine[1] = lg_matrix.GetB();
	lg_affine[2] = lg_matrix.GetC();
	lg_affine[3] = lg_matrix.GetD();
	lg_affine[4] = lg_matrix.GetE();
	lg_affine[5] = lg_matrix.GetF();
	
	
	// Apply gradient transformation matrix to linear grad vector
	int lg_x1 = gradElem.GetX1().GetBaseVal();
	int lg_x2 = gradElem.GetX2().GetBaseVal();
	int lg_y1 = gradElem.GetY1().GetBaseVal();
	int lg_y2 = gradElem.GetY2().GetBaseVal();
	fx1 = lg_x1 * lg_affine[0] + lg_y1 * lg_affine[2] + lg_affine[4];
	fy1 = lg_x1 * lg_affine[1] + lg_y1 * lg_affine[3] + lg_affine[5];
	fx2 = lg_x2 * lg_affine[0] + lg_y2 * lg_affine[2] + lg_affine[4];
	fy2 = lg_x2 * lg_affine[1] + lg_y2 * lg_affine[3] + lg_affine[5];

	/* compute [xy][12] in pixel space */
	x1 = fx1 * affine[0] + fy1 * affine[2] + affine[4];
	y1 = fx1 * affine[1] + fy1 * affine[3] + affine[5];
	x2 = fx2 * affine[0] + fy2 * affine[2] + affine[4];
	y2 = fx2 * affine[1] + fy2 * affine[3] + affine[5];	
}
