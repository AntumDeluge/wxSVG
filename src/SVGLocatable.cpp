//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLocatable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLocatable.cpp,v 1.5.2.1 2005-08-29 15:00:52 etisserant Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLocatable.h"
#include "svg.h"
#include <math.h>
#include <wx/log.h>

wxSVGRect TransformRect(wxSVGRect rect, wxSVGMatrix& matrix)
{
  wxSVGPoint point1(rect.GetX(), rect.GetY());
  point1 = point1.MatrixTransform(matrix);
  wxSVGPoint point2(rect.GetX() + rect.GetWidth(), rect.GetY() + rect.GetHeight());
  point2 = point2.MatrixTransform(matrix);
  
  rect.SetX(point1.GetX() < point2.GetX() ? point1.GetX() : point2.GetX());
  rect.SetY(point1.GetY() < point2.GetY() ? point1.GetY() : point2.GetY());
  rect.SetWidth(point1.GetX() < point2.GetX() ? point2.GetX() - rect.GetX() : point1.GetX() - rect.GetX());
  rect.SetHeight(point1.GetY() < point2.GetY() ? point2.GetY() - rect.GetY() : point1.GetY() - rect.GetY());
  return rect;
}

#define GetElementBBox_macro(the_dtd, the_class)\
		case the_dtd:\
		{\
		  the_class* element = (the_class*) &elem;\
		  element->UpdateMatrix(matrix);\
		  elemBBox = TransformRect(element->GetBBox(), matrix);\
		  break;\
		}

wxSVGRect GetElementBBox(const wxSVGElement& elem)
{
    wxSVGRect elemBBox;
	if (elem.GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGMatrix matrix;
	  switch (elem.GetDtd())
	  {
		GetElementBBox_macro(wxSVG_LINE_ELEMENT, wxSVGLineElement)
		GetElementBBox_macro(wxSVG_POLYLINE_ELEMENT, wxSVGPolylineElement)
		GetElementBBox_macro(wxSVG_POLYGON_ELEMENT, wxSVGPolygonElement)
		GetElementBBox_macro(wxSVG_RECT_ELEMENT, wxSVGRectElement)
		GetElementBBox_macro(wxSVG_CIRCLE_ELEMENT, wxSVGCircleElement)
		GetElementBBox_macro(wxSVG_ELLIPSE_ELEMENT, wxSVGEllipseElement)
		GetElementBBox_macro(wxSVG_PATH_ELEMENT, wxSVGPathElement)
		GetElementBBox_macro(wxSVG_TEXT_ELEMENT, wxSVGTextElement)
		GetElementBBox_macro(wxSVG_G_ELEMENT, wxSVGGElement)
		case wxSVG_SVG_ELEMENT:
		{
		  wxSVGSVGElement* element = (wxSVGSVGElement*) &elem;
          elemBBox = element->GetBBox();
		  break;
		}
		default:
		  break;
	  }
	}
	return elemBBox;
}


void SumBBox(wxSVGElement* parent, wxSVGRect& bbox)
{
  bool empty = true;
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  for (; elem != NULL; elem = (wxSVGElement*) elem->GetNext())
  {
	  wxSVGRect elemBBox = GetElementBBox(*elem);
	  
	  if (empty)
	  {
		bbox = elemBBox;
		empty = false;
		continue;
	  }
	  
	  if (bbox.GetX() > elemBBox.GetX())
	  {
		bbox.SetWidth(bbox.GetWidth() + bbox.GetX() - elemBBox.GetX());
		bbox.SetX(elemBBox.GetX());
	  }
	  if (bbox.GetY() > elemBBox.GetY())
	  {
		bbox.SetHeight(bbox.GetHeight() + bbox.GetY() - elemBBox.GetY());
		bbox.SetY(elemBBox.GetY());
	  }
	  if (bbox.GetX() + bbox.GetWidth() < elemBBox.GetX() + elemBBox.GetWidth())
		bbox.SetWidth(elemBBox.GetX() + elemBBox.GetWidth() - bbox.GetX());
	  if (bbox.GetY() + bbox.GetHeight() < elemBBox.GetY() + elemBBox.GetHeight())
		bbox.SetHeight(elemBBox.GetY() + elemBBox.GetHeight() - bbox.GetY());
  }
}

wxSVGRect wxSVGLocatable::GetBBox(wxSVGElement* parent)
{
  wxSVGRect rect;
  SumBBox(parent, rect);
  return rect;
}

#define GetElementCTM_macro(the_dtd, the_class)\
		case the_dtd:\
		{\
		  wxSVGMatrix matrix = GetElementCTM(*(wxSVGElement*)(elem.GetParent()));\
		  the_class* element = (the_class*) &elem;\
		  element->UpdateMatrix(matrix);\
		  return matrix;\
		}

wxSVGMatrix GetElementCTM(const wxSVGElement& elem)
{
	if (&elem!=NULL && elem.GetType() == wxXML_ELEMENT_NODE)
	{
	  
	  switch (elem.GetDtd())
	  {
		GetElementCTM_macro(wxSVG_LINE_ELEMENT, wxSVGLineElement)
		GetElementCTM_macro(wxSVG_POLYLINE_ELEMENT, wxSVGPolylineElement)
		GetElementCTM_macro(wxSVG_POLYGON_ELEMENT, wxSVGPolygonElement)
		GetElementCTM_macro(wxSVG_RECT_ELEMENT, wxSVGRectElement)
		GetElementCTM_macro(wxSVG_CIRCLE_ELEMENT, wxSVGCircleElement)
		GetElementCTM_macro(wxSVG_ELLIPSE_ELEMENT, wxSVGEllipseElement)
		GetElementCTM_macro(wxSVG_PATH_ELEMENT, wxSVGPathElement)
		GetElementCTM_macro(wxSVG_TEXT_ELEMENT, wxSVGTextElement)
		GetElementCTM_macro(wxSVG_G_ELEMENT, wxSVGGElement)
		default:
		  break;
	  }
	}
	return wxSVGMatrix();
}

wxSVGMatrix wxSVGLocatable::GetCTM(wxSVGElement* obj)
{  
  return GetElementCTM(*obj);
}
wxSVGMatrix wxSVGLocatable::GetTransformToElement(const wxSVGElement& element)
{
  wxSVGMatrix res;
  return res;
}
