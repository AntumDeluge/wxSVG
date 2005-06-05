//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.8 2005-06-05 19:10:49 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGDocument.h"

#ifdef USE_RENDER_AGG
#include "agg/SVGCanvasAgg.h"
#define WX_SVG_CANVAS wxSVGCanvasAgg
#else // USE_RENDER_LIBART
#include "libart/SVGCanvasLibart.h"
#define WX_SVG_CANVAS wxSVGCanvasLibart
#endif

#include <wx/log.h>

wxSVGDocument::~wxSVGDocument()
{
  delete m_canvas;
}

void wxSVGDocument::Init()
{
  m_canvas = new WX_SVG_CANVAS;
}

wxXmlElement* wxSVGDocument::CreateElement(const wxString& tagName)
{
  return CreateElementNS(wxT(""), tagName);
}

#include "SVGDocument_CreateElement.cpp"

void RenderChilds(wxSVGCanvas* canvas, wxSVGElement* parent,
  wxSVGMatrix* parentMatrix, wxCSSStyleDeclaration* parentStyle)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  while (elem)
  {
	if (elem->GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGMatrix matrix(*parentMatrix);
	  wxCSSStyleDeclaration style(*parentStyle);
	  
	  switch (elem->GetDtd())
	  {
		case wxSVG_SVG_ELEMENT:
		  RenderChilds(canvas, elem, &matrix, &style);
		  break;
		case wxSVG_G_ELEMENT:
		{
		  wxSVGGElement* element = (wxSVGGElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  RenderChilds(canvas, elem, &matrix, &style);
		  break;
		}
		case wxSVG_LINE_ELEMENT:
		{
		  wxSVGLineElement* element = (wxSVGLineElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawLine(element, &matrix, &style);
		  break;
		}
		case wxSVG_POLYLINE_ELEMENT:
		{
		  wxSVGPolylineElement* element = (wxSVGPolylineElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPolyline(element, &matrix, &style);
		  break;
		}
		case wxSVG_POLYGON_ELEMENT:
		{
		  wxSVGPolygonElement* element = (wxSVGPolygonElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPolygon(element, &matrix, &style);
		  break;
		}
		case wxSVG_RECT_ELEMENT:
		{
		  wxSVGRectElement* element = (wxSVGRectElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawRect(element, &matrix, &style);
		  break;
		}
		case wxSVG_CIRCLE_ELEMENT:
		{
		  wxSVGCircleElement* element = (wxSVGCircleElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawCircle(element, &matrix, &style);
		  break;
		}
		case wxSVG_ELLIPSE_ELEMENT:
		{
		  wxSVGEllipseElement* element = (wxSVGEllipseElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawEllipse(element, &matrix, &style);
		  break;
		}
		case wxSVG_PATH_ELEMENT:
		{
		  wxSVGPathElement* element = (wxSVGPathElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPath(element, &matrix, &style);
		  break;
		}
		case wxSVG_TSPAN_ELEMENT:
		  break;
		case wxSVG_TEXT_ELEMENT:
		{
		  wxSVGTextElement* element = (wxSVGTextElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawText(element, &matrix, &style);
		  break;
		}
		case wxSVG_IMAGE_ELEMENT:
		{
		  wxSVGImageElement* element = (wxSVGImageElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawImage(element, &matrix, &style);
		  break;
		}
		default:
		  break;
	  }
	}
	elem = (wxSVGElement*) elem->GetNext();
  }
}

wxImage wxSVGDocument::Render(int width, int height)
{
  if (!GetRootElement())
	return wxImage();
  
  wxSVGMatrix matrix;
  
  // view box
  wxSVGRect viewbox = GetRootElement()->GetViewBox();
  if (viewbox.GetWidth()<=0 && viewbox.GetHeight()<=0)
    viewbox = wxSVGRect(0, 0, GetRootElement()->GetWidth(), GetRootElement()->GetHeight());
  if (viewbox.GetWidth()<=0 || viewbox.GetHeight()<=0)
  {
	viewbox = GetRootElement()->GetBBox();
	matrix = matrix.Scale(0.95).Translate(width*0.025, height*0.025);
  }
  
  // scale it to fit in
  float scale = 1;
  if (width == -1 || height == -1)
  {
	width = (int) GetRootElement()->GetWidth();
	height = (int) GetRootElement()->GetHeight();
	if (width <= 0 || height <= 0)
	{
	  width = (int) viewbox.GetWidth();
	  height = (int) viewbox.GetHeight();
	}
  }
  else if (viewbox.GetWidth()>0 && viewbox.GetHeight()>0)
  {
	scale = width/viewbox.GetWidth();
	if (scale>height/viewbox.GetHeight())
	  scale = height/viewbox.GetHeight();
  }
  matrix = matrix.Scale(scale);
  if (viewbox.GetX()!=0 || viewbox.GetY()!=0)
	matrix = matrix.Translate(-viewbox.GetX(), -viewbox.GetY());
  
  // render
  wxImage image(width, height);
  m_canvas->SetImage(&image);
  m_canvas->Clear();
  RenderChilds(m_canvas, GetRootElement(), &matrix, &GetRootElement()->GetStyle());
  
  return image;
}

