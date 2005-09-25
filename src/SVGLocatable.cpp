//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLocatable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLocatable.cpp,v 1.6 2005-09-25 11:44:38 ntalex Exp $
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

#define GET_ELEMENT_BBOX(the_dtd, the_class)\
		case the_dtd:\
		{\
		  the_class* element = (the_class*) &elem;\
		  element->UpdateMatrix(matrix);\
		  elemBBox = TransformRect(element->GetBBox(), matrix);\
		  break;\
		}

wxSVGRect wxSVGLocatable::GetElementBBox(const wxSVGElement& elem)
{
    wxSVGRect elemBBox;
	if (elem.GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGMatrix matrix;
	  switch (elem.GetDtd())
	  {
		GET_ELEMENT_BBOX(wxSVG_LINE_ELEMENT, wxSVGLineElement)
		GET_ELEMENT_BBOX(wxSVG_POLYLINE_ELEMENT, wxSVGPolylineElement)
		GET_ELEMENT_BBOX(wxSVG_POLYGON_ELEMENT, wxSVGPolygonElement)
		GET_ELEMENT_BBOX(wxSVG_RECT_ELEMENT, wxSVGRectElement)
		GET_ELEMENT_BBOX(wxSVG_CIRCLE_ELEMENT, wxSVGCircleElement)
		GET_ELEMENT_BBOX(wxSVG_ELLIPSE_ELEMENT, wxSVGEllipseElement)
		GET_ELEMENT_BBOX(wxSVG_PATH_ELEMENT, wxSVGPathElement)
		GET_ELEMENT_BBOX(wxSVG_TEXT_ELEMENT, wxSVGTextElement)
        GET_ELEMENT_BBOX(wxSVG_IMAGE_ELEMENT, wxSVGImageElement)
		GET_ELEMENT_BBOX(wxSVG_G_ELEMENT, wxSVGGElement)
        GET_ELEMENT_BBOX(wxSVG_USE_ELEMENT, wxSVGUseElement)
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

void SumBBox(const wxSVGElement* parent, wxSVGRect& bbox)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  for (; elem != NULL; elem = (wxSVGElement*) elem->GetNext())
  {
	  wxSVGRect elemBBox = wxSVGLocatable::GetElementBBox(*elem);
      
      if (elemBBox.IsEmpty())
        continue;
	  
	  if (bbox.IsEmpty())
	  {
		bbox = elemBBox;
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

wxSVGRect wxSVGLocatable::GetChildrenBBox(const wxSVGElement& element)
{
  wxSVGRect rect;
  SumBBox(&element, rect);
  return rect;
}

wxSVGMatrix wxSVGLocatable::GetCTM()
{
  wxSVGMatrix res;
  return res;
}

wxSVGMatrix wxSVGLocatable::GetTransformToElement(const wxSVGElement& element)
{
  wxSVGMatrix res;
  return res;
}
