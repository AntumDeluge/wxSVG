//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.12.2.1 2005-08-10 15:00:52 etisserant Exp $
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
  m_canvas = new WX_SVG_CANVAS(this);
}

wxXmlElement* wxSVGDocument::CreateElement(const wxString& tagName)
{
  return CreateElementNS(wxT(""), tagName);
}

#include "SVGDocument_CreateElement.cpp"

wxSVGElement* GetElemById(wxSVGElement* parent, const wxString& id)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  while (elem)
  {
	if (elem->GetType() == wxXML_ELEMENT_NODE &&
        elem->GetId() == id)
      return elem;
    if (elem->GetDtd() == wxSVG_G_ELEMENT ||
        elem->GetDtd() == wxSVG_DEFS_ELEMENT ||
        elem->GetDtd() == wxSVG_SYMBOL_ELEMENT)
    {
      wxSVGElement* elem2 = GetElemById(elem, id);
      if (elem2 != NULL)
        return elem2;
    }
    elem = (wxSVGElement*) elem->GetNext();
  }
  return NULL;
}

wxSVGElement* wxSVGDocument::GetElementById(const wxString& id)
{
  return GetRootElement() ? GetElemById(GetRootElement(), id) : NULL;
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
	  wxString href = element->GetHref();
	  if (href.length() == 0 || href[0] != wxT('#'))
	    break;
      href.Remove(0,1);
      wxSVGElement* refElem = doc->GetElementById(href);
      if (!refElem)
        break;
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
      RenderElement(doc, gElem, &matrix, &style, ownerSVGElement, viewportElement);
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

wxImage wxSVGDocument::Render(int width, int height)
{
  if (!GetRootElement())
	return wxImage();
  
  wxSVGMatrix matrix;
  
  if (GetRootElement()->GetWidth().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN ||
      GetRootElement()->GetHeight().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
  {
    wxSVGRect bbox = GetRootElement()->GetBBox();
    GetRootElement()->SetWidth(wxSVGLength(bbox.GetWidth()));
    GetRootElement()->SetHeight(wxSVGLength(bbox.GetHeight()));
	matrix = matrix.Scale(0.95).Translate(width*0.025, height*0.025);
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
  if (GetRootElement()->GetWidth().GetAnimVal()>0 &&
      GetRootElement()->GetHeight().GetAnimVal()>0)
  {
    double scale = 1;
	scale = width/GetRootElement()->GetWidth().GetAnimVal();
	if (scale > height/GetRootElement()->GetHeight().GetAnimVal())
	  scale = height/GetRootElement()->GetHeight().GetAnimVal();
    matrix = matrix.Scale(scale);
  }
  
  // render
  wxImage image(width, height);
  m_canvas->SetImage(&image);
  m_canvas->Clear();
  RenderElement(this, GetRootElement(), &matrix,
	&GetRootElement()->GetStyle(), NULL, NULL);
  
  return image;
}

