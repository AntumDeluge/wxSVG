//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.15 2005-09-25 11:45:53 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGDocument.h"

#ifdef USE_RENDER_AGG
#include "agg/SVGCanvasAgg.h"
#define WX_SVG_CANVAS wxSVGCanvasAgg
#elif defined USE_RENDER_CAIRO
#include "cairo/SVGCanvasCairo.h"
#define WX_SVG_CANVAS wxSVGCanvasCairo
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
  m_scale = 1;
}

wxXmlElement* wxSVGDocument::CreateElement(const wxString& tagName)
{
  return CreateElementNS(wxT(""), tagName);
}

#include "SVGDocument_CreateElement.cpp"

wxSVGElement* wxSVGDocument::GetElementById(const wxString& id)
{
  return GetRootElement() ?
    (wxSVGElement*) GetRootElement()->GetElementById(id) : NULL;
}

void RenderChilds(wxSVGDocument* doc, wxSVGElement* parent,
  const wxSVGMatrix* parentMatrix, const wxCSSStyleDeclaration* parentStyle,
  wxSVGSVGElement* ownerSVGElement, wxSVGElement* viewportElement);

void RenderElement(wxSVGDocument* doc, wxSVGElement* elem,
  const wxSVGMatrix* parentMatrix, const wxCSSStyleDeclaration* parentStyle,
  wxSVGSVGElement* ownerSVGElement, wxSVGElement* viewportElement)
{
  wxSVGMatrix matrix(*parentMatrix);
  wxCSSStyleRef style(*parentStyle);
  elem->SetOwnerSVGElement(ownerSVGElement);
  elem->SetViewportElement(viewportElement);
  
  switch (elem->GetDtd())
  {
	case wxSVG_SVG_ELEMENT:
    {
	  wxSVGSVGElement* element = (wxSVGSVGElement*) elem;
	  // view box
	  wxSVGRect viewbox = element->GetViewBox().GetAnimVal();
      if (viewbox.GetWidth()>0 && viewbox.GetHeight()>0)
      {
        if (element->GetPreserveAspectRatio().GetAnimVal().GetAlign() == wxSVG_PRESERVEASPECTRATIO_NONE)
          matrix = matrix.ScaleNonUniform(
            element->GetWidth().GetAnimVal()/viewbox.GetWidth(),
            element->GetHeight().GetAnimVal()/viewbox.GetHeight());
        else
        {
          double scale = 1;
          scale = element->GetWidth().GetAnimVal()/viewbox.GetWidth();
    	  if (scale>element->GetHeight().GetAnimVal()/viewbox.GetHeight())
    	    scale = element->GetHeight().GetAnimVal()/viewbox.GetHeight();
  	      matrix = matrix.Scale(scale);
          if (viewbox.GetX()!=0 || viewbox.GetY()!=0)
    	    matrix = matrix.Translate(-viewbox.GetX(), -viewbox.GetY());
        }
      }
	  RenderChilds(doc, elem, &matrix, &style, element, element);
	  break;
    }
	case wxSVG_G_ELEMENT:
	{
	  wxSVGGElement* element = (wxSVGGElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  RenderChilds(doc, elem, &matrix, &style, ownerSVGElement, viewportElement);
	  break;
	}
	case wxSVG_LINE_ELEMENT:
	{
	  wxSVGLineElement* element = (wxSVGLineElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawLine(element, &matrix, &style);
	  break;
	}
	case wxSVG_POLYLINE_ELEMENT:
	{
	  wxSVGPolylineElement* element = (wxSVGPolylineElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawPolyline(element, &matrix, &style);
	  break;
	}
	case wxSVG_POLYGON_ELEMENT:
	{
	  wxSVGPolygonElement* element = (wxSVGPolygonElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawPolygon(element, &matrix, &style);
	  break;
	}
	case wxSVG_RECT_ELEMENT:
	{
	  wxSVGRectElement* element = (wxSVGRectElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawRect(element, &matrix, &style);
	  break;
	}
	case wxSVG_CIRCLE_ELEMENT:
	{
	  wxSVGCircleElement* element = (wxSVGCircleElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawCircle(element, &matrix, &style);
	  break;
	}
	case wxSVG_ELLIPSE_ELEMENT:
	{
	  wxSVGEllipseElement* element = (wxSVGEllipseElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawEllipse(element, &matrix, &style);
	  break;
	}
	case wxSVG_PATH_ELEMENT:
	{
	  wxSVGPathElement* element = (wxSVGPathElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawPath(element, &matrix, &style);
	  break;
	}
	case wxSVG_TSPAN_ELEMENT:
	  break;
	case wxSVG_TEXT_ELEMENT:
	{
	  wxSVGTextElement* element = (wxSVGTextElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawText(element, &matrix, &style);
	  break;
	}
	case wxSVG_IMAGE_ELEMENT:
	{
	  wxSVGImageElement* element = (wxSVGImageElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  doc->GetCanvas()->DrawImage(element, &matrix, &style);
	  break;
	}
	case wxSVG_USE_ELEMENT:
    {
      wxSVGUseElement* element = (wxSVGUseElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
      // test if visible
      wxSVGPoint point(element->GetX().GetAnimVal(), element->GetY().GetAnimVal());
      point = point.MatrixTransform(matrix);
      if (point.GetX() > doc->GetCanvas()->GetWidth() ||
          point.GetY() > doc->GetCanvas()->GetHeight())
        break;
      if (element->GetWidth().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN &&
          element->GetHeight().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
      {
        wxSVGPoint point(
          element->GetX().GetAnimVal() + element->GetWidth().GetAnimVal(),
          element->GetY().GetAnimVal() + element->GetHeight().GetAnimVal());
        point = point.MatrixTransform(matrix);
        if (point.GetX()<0 || point.GetY()<0)
          break;
      }
      // get ref element 
	  wxString href = element->GetHref();
	  if (href.length() == 0 || href[0] != wxT('#'))
	    break;
      href.Remove(0,1);
      wxSVGElement* refElem = doc->GetElementById(href);
      if (!refElem)
        break;
      
      // create shadow tree
      wxSVGGElement* gElem = new wxSVGGElement(doc);
      gElem->SetOwnerSVGElement(ownerSVGElement);
      gElem->SetViewportElement(viewportElement);
      gElem->SetTransform(element->GetTransform());
      gElem->SetStyle(element->GetStyle());
      gElem->Translate(element->GetX().GetAnimVal(), element->GetY().GetAnimVal());
      if (refElem->GetDtd() == wxSVG_SYMBOL_ELEMENT)
      {
        wxSVGSVGElement* svgElem = new wxSVGSVGElement(doc);
        if (element->GetWidth().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
          svgElem->SetWidth(element->GetWidth().GetAnimVal());
        else
          svgElem->SetWidth(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
        if (element->GetHeight().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
          svgElem->SetHeight(element->GetHeight().GetAnimVal());
        else
          svgElem->SetHeight(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
        svgElem->SetViewBox(((wxSVGSymbolElement*)refElem)->GetViewBox());
        svgElem->SetPreserveAspectRatio(((wxSVGSymbolElement*)refElem)->GetPreserveAspectRatio());
        wxSVGElement* child = (wxSVGElement*) refElem->GetChildren();
        while (child)
        {
       	  if (elem->GetType() == wxXML_ELEMENT_NODE)
       	    svgElem->AddChild(child->CloneNode());
          child = (wxSVGElement*) child->GetNext();
        }
        gElem->AddChild(svgElem);
      }
      else
        gElem->AddChild(refElem->CloneNode());
      // render
      RenderElement(doc, gElem, &matrix, &style, ownerSVGElement, viewportElement);
      // delete shadow tree
      delete gElem;
      break;
    }
	default:
	  break;
  }
}

void RenderChilds(wxSVGDocument* doc, wxSVGElement* parent,
  const wxSVGMatrix* parentMatrix, const wxCSSStyleDeclaration* parentStyle,
  wxSVGSVGElement* ownerSVGElement, wxSVGElement* viewportElement)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  while (elem)
  {
    if (elem->GetType() == wxXML_ELEMENT_NODE)
      RenderElement(doc, elem, parentMatrix, parentStyle,
        ownerSVGElement, viewportElement);
    elem = (wxSVGElement*) elem->GetNext();
  }
}

wxImage wxSVGDocument::Render(int width, int height, const wxRect* rect)
{
  if (!GetRootElement())
	return wxImage();
  
  wxSVGMatrix matrix;
  
  // render only rect if specified
  if (rect)
    matrix = matrix.Translate(-rect->x, -rect->y);
  
  if (GetRootElement()->GetWidth().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN ||
      GetRootElement()->GetHeight().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
  {
    wxSVGRect bbox = GetRootElement()->GetBBox();
    GetRootElement()->SetWidth(wxSVGLength(bbox.GetWidth()*0.95));
    GetRootElement()->SetHeight(wxSVGLength(bbox.GetHeight()*0.95));
	matrix = matrix.Translate(width*0.025, height*0.025);
  }
  
  if (width == -1 || height == -1)
  {
	width = (int) GetRootElement()->GetWidth().GetAnimVal();
	height = (int) GetRootElement()->GetHeight().GetAnimVal();
	if (width <= 0 || height <= 0)
	{
	  width = (int) GetRootElement()->GetViewBox().GetAnimVal().GetWidth();
	  height = (int) GetRootElement()->GetViewBox().GetAnimVal().GetHeight();
	}
  }
  
  if (GetRootElement()->GetWidth().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_PERCENTAGE)
  {
    wxSVGAnimatedLength l = GetRootElement()->GetWidth();
    l.GetBaseVal().ToViewportWidth(width);
    if (l.GetBaseVal() != ((const wxSVGAnimatedLength&) l).GetAnimVal())
      l.GetAnimVal().ToViewportWidth(width);
    GetRootElement()->SetWidth(l);
  }
  
  if (GetRootElement()->GetHeight().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_PERCENTAGE)
  {
    wxSVGAnimatedLength l = GetRootElement()->GetHeight();
    l.GetBaseVal().ToViewportHeight(height);
    if (l.GetBaseVal() != ((const wxSVGAnimatedLength&) l).GetAnimVal())
      l.GetAnimVal().ToViewportHeight(height);
    GetRootElement()->SetHeight(l);
  }
  
  // scale it to fit in
  m_scale = 1;
  if (GetRootElement()->GetWidth().GetAnimVal()>0 &&
      GetRootElement()->GetHeight().GetAnimVal()>0)
  {
	m_scale = width/GetRootElement()->GetWidth().GetAnimVal();
	if (m_scale > height/GetRootElement()->GetHeight().GetAnimVal())
	  m_scale = height/GetRootElement()->GetHeight().GetAnimVal();
    matrix = matrix.Scale(m_scale);
    
    width = (int)(m_scale*GetRootElement()->GetWidth().GetAnimVal());
    height = (int)(m_scale*GetRootElement()->GetHeight().GetAnimVal());
  }
  
  // render only rect if specified
  if (rect)
  {
    if (rect->width < width)
      width = rect->width;
    if (rect->height < height)
      height = rect->height;
  }
  
  // render
  m_canvas->Init(width, height);
  m_canvas->Clear();
  RenderElement(this, GetRootElement(), &matrix,
	&GetRootElement()->GetStyle(), NULL, NULL);
  
  return m_canvas->GetImage();
}

