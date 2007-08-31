//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIControl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/18
// RCS-ID:      $Id: SVGUIElement.cpp,v 1.2 2007-08-31 13:38:52 gusstdie Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIElement.h"

//IMPLEMENT_ABSTRACT_CLASS(SVGUIElement, wxObject)


#define SetElementDisplay_macro(the_dtd, the_class)\
		case the_dtd:\
		{\
          ((the_class*) elem)->SetDisplay(value);\
          break;\
		}

void SetDisplay(wxSVGElement* elem, wxCSS_VALUE value)
{
	if (elem)
	{
		switch (elem->GetDtd())
		{
			SetElementDisplay_macro(wxSVG_LINE_ELEMENT, wxSVGLineElement)
			SetElementDisplay_macro(wxSVG_POLYLINE_ELEMENT, wxSVGPolylineElement)
			SetElementDisplay_macro(wxSVG_POLYGON_ELEMENT, wxSVGPolygonElement)
			SetElementDisplay_macro(wxSVG_RECT_ELEMENT, wxSVGRectElement)
			SetElementDisplay_macro(wxSVG_CIRCLE_ELEMENT, wxSVGCircleElement)
			SetElementDisplay_macro(wxSVG_ELLIPSE_ELEMENT, wxSVGEllipseElement)
			SetElementDisplay_macro(wxSVG_PATH_ELEMENT, wxSVGPathElement)
			SetElementDisplay_macro(wxSVG_TEXT_ELEMENT, wxSVGTextElement)
			SetElementDisplay_macro(wxSVG_G_ELEMENT, wxSVGGElement)
            default:
              break;
		}
	}
}



SVGUIElement::SVGUIElement()
{
	m_doc = NULL;
	m_BackgroundElement = NULL;
	m_enable = true;
}


SVGUIElement::SVGUIElement(wxSVGDocument* doc)
{
	m_doc = doc;
	m_BackgroundElement = NULL;
}

wxString SVGUIElement::GetId()
{
	return GetAttribute(wxT("id"));
}

wxString SVGUIElement::GetName()
{
	return GetAttribute(wxT("name"));
}

void SVGUIElement::AddProperty(const wxString& name, const wxString& value)
{
	SetAttribute(name, value);
	wxSvgXmlElement::AddProperty(name, value);
}

bool SVGUIElement::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (attrName == wxT("background_id"))
  	m_BackgroundElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else 
    return false;
  return true;
}

void SVGUIElement::Hide()
{
	if (m_BackgroundElement)
		SetDisplay(m_BackgroundElement, wxCSS_VALUE_NONE);
	Disable();
}

void SVGUIElement::Show()
{
	if (m_BackgroundElement)
		SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
	Enable();
}
