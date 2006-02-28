//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.25 2006-02-28 17:00:36 ntalex Exp $
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

void RenderChilds(wxSVGDocument* doc, wxSVGElement* parent, const wxSVGRect* rect,
  const wxSVGMatrix* parentMatrix, const wxCSSStyleDeclaration* parentStyle,
  wxSVGSVGElement* ownerSVGElement, wxSVGElement* viewportElement);

void RenderElement(wxSVGDocument* doc, wxSVGElement* elem, const wxSVGRect* rect,
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
      if (element->GetWidth().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
        ((wxSVGAnimatedLength&)element->GetWidth()).SetAnimVal(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
      if (element->GetHeight().GetAnimVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
        ((wxSVGAnimatedLength&)element->GetHeight()).SetAnimVal(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
      element->UpdateMatrix(matrix);
      if (rect && element->GetParent())
      {
        wxSVGRect rect2 = *rect;
        wxSVGElement* parent = (wxSVGElement*) element->GetParent();
        wxSVGTransformable* transformable =
           wxSVGTransformable::GetSVGTransformable(*parent);
        if (transformable)
          rect2 = rect2.MatrixTransform(transformable->GetCTM().Inverse());
        RenderChilds(doc, elem, &rect2, &matrix, &style, element, element);
      }
      else
	    RenderChilds(doc, elem, rect, &matrix, &style, element, element);
	  break;
    }
	case wxSVG_G_ELEMENT:
	{
	  wxSVGGElement* element = (wxSVGGElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  RenderChilds(doc, elem, rect, &matrix, &style, ownerSVGElement, viewportElement);
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
    case wxSVG_VIDEO_ELEMENT:
	{
	  wxSVGVideoElement* element = (wxSVGVideoElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
	  wxSVGGElement* gElem = new wxSVGGElement();
      gElem->SetOwnerSVGElement(ownerSVGElement);
      gElem->SetViewportElement(viewportElement);
      gElem->SetStyle(element->GetStyle());
      wxSVGRectElement* rectElem = new wxSVGRectElement();
      rectElem->SetX(element->GetX().GetAnimVal());
      rectElem->SetY(element->GetY().GetAnimVal());
      rectElem->SetWidth(element->GetWidth().GetAnimVal());
      rectElem->SetHeight(element->GetHeight().GetAnimVal());
      rectElem->SetFill(wxSVGPaint(0,0,0));
      gElem->AppendChild(rectElem);
      wxSVGTextElement* textElem = new wxSVGTextElement;
      textElem->SetX((double)element->GetX().GetAnimVal());
      textElem->SetY(element->GetY().GetAnimVal() + (double)element->GetHeight().GetAnimVal()/10);
      textElem->SetFontSize((double)element->GetHeight().GetAnimVal()/15);
      textElem->SetFill(wxSVGPaint(255,255,255));
      textElem->SetStroke(wxSVGPaint(255,255,255));
      textElem->AddChild(new wxXmlNode(wxXML_TEXT_NODE, wxT(""),
        wxT(" [") + element->GetHref() + wxT("]")));
      gElem->AppendChild(textElem);
      
      // render
      RenderElement(doc, gElem, rect, &matrix, &style, ownerSVGElement, viewportElement);
      // delete shadow tree
      delete gElem;
	  break;
	}
	case wxSVG_USE_ELEMENT:
    {
      wxSVGUseElement* element = (wxSVGUseElement*) elem;
	  element->UpdateMatrix(matrix);
	  style.Add(element->GetStyle());
      // test if visible
      if (element->GetWidth().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN &&
          element->GetHeight().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
      {
        if (rect && !ownerSVGElement->CheckIntersection(*elem, *rect))
          break;
        wxSVGPoint point(
          element->GetX().GetAnimVal() + element->GetWidth().GetAnimVal(),
          element->GetY().GetAnimVal() + element->GetHeight().GetAnimVal());
        point = point.MatrixTransform(matrix);
        if (point.GetX()<0 || point.GetY()<0)
          break;
      }
      // get ref element 
	  wxString href = element->GetHref();
	  if (href.length() == 0 || href.GetChar(0) != wxT('#'))
	    break;
      href.Remove(0,1);
      wxSVGElement* refElem =
        (wxSVGElement*) ownerSVGElement->GetElementById(href);
      if (!refElem)
        break;
      
      // create shadow tree
      wxSVGGElement* gElem = new wxSVGGElement();
      gElem->SetOwnerSVGElement(ownerSVGElement);
      gElem->SetViewportElement(viewportElement);
      gElem->SetStyle(element->GetStyle());
      if (element->GetX().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
        gElem->Translate(element->GetX().GetAnimVal(), element->GetY().GetAnimVal());
      if (refElem->GetDtd() == wxSVG_SYMBOL_ELEMENT ||
          refElem->GetDtd() == wxSVG_SVG_ELEMENT)
      {
        wxSVGSVGElement* svgElem;
        if (refElem->GetDtd() == wxSVG_SVG_ELEMENT)
          svgElem = (wxSVGSVGElement*) refElem->CloneNode();
        else
        {
          svgElem = new wxSVGSVGElement();
          wxXmlElement* child = refElem->GetChildren();
          while (child)
          {
       	    svgElem->AddChild(child->CloneNode());
            child = child->GetNext();
          }
          svgElem->SetViewBox(((wxSVGSymbolElement*)refElem)->GetViewBox());
          svgElem->SetPreserveAspectRatio(((wxSVGSymbolElement*)refElem)->GetPreserveAspectRatio());
        }
        if (element->GetWidth().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
          svgElem->SetWidth(element->GetWidth().GetAnimVal());
        if (element->GetHeight().GetAnimVal().GetUnitType() != wxSVG_LENGTHTYPE_UNKNOWN)
          svgElem->SetHeight(element->GetHeight().GetAnimVal());
        gElem->AddChild(svgElem);
      }
      else
        gElem->AddChild(refElem->CloneNode());
      // render
      RenderElement(doc, gElem, rect, &matrix, &style, ownerSVGElement, viewportElement);
      // delete shadow tree
      delete gElem;
      break;
    }
	default:
	  break;
  }
}

void RenderChilds(wxSVGDocument* doc, wxSVGElement* parent, const wxSVGRect* rect,
  const wxSVGMatrix* parentMatrix, const wxCSSStyleDeclaration* parentStyle,
  wxSVGSVGElement* ownerSVGElement, wxSVGElement* viewportElement)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  while (elem)
  {
    if (elem->GetType() == wxXML_ELEMENT_NODE)
    {
      //if (!rect || ownerSVGElement->CheckIntersection(*elem, *rect))
        RenderElement(doc, elem, rect, parentMatrix, parentStyle,
          ownerSVGElement, viewportElement);
    }
    elem = (wxSVGElement*) elem->GetNext();
  }
}

wxImage wxSVGDocument::Render(int width, int height, const wxSVGRect* rect)
{
  if (!GetRootElement())
	return wxImage();
  
  m_screenCTM = wxSVGMatrix();
  
  if (GetRootElement()->GetWidth().GetBaseVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
    GetRootElement()->SetWidth(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
  if (GetRootElement()->GetHeight().GetBaseVal().GetUnitType() == wxSVG_LENGTHTYPE_UNKNOWN)
    GetRootElement()->SetHeight(wxSVGLength(wxSVG_LENGTHTYPE_PERCENTAGE, 100));
  
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
    m_screenCTM = m_screenCTM.Scale(m_scale);
    
    width = (int)(m_scale*GetRootElement()->GetWidth().GetAnimVal());
    height = (int)(m_scale*GetRootElement()->GetHeight().GetAnimVal());
  }
  
  // render only rect if specified
  if (rect && !rect->IsEmpty())
  {
    m_screenCTM = m_screenCTM.Translate(-rect->GetX(), -rect->GetY());
    if (rect->GetWidth()*GetScale() < width)
      width = (int) (rect->GetWidth()*GetScale());
    if (rect->GetHeight()*GetScale() < height)
      height = (int) (rect->GetHeight()*GetScale());
  }
  
  // render
  m_canvas->Init(width, height);
  m_canvas->Clear();
  RenderElement(this, GetRootElement(), rect, &m_screenCTM,
	&GetRootElement()->GetStyle(), NULL, NULL);
  
  return m_canvas->GetImage();
}

