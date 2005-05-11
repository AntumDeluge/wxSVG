//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLocatable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLocatable.cpp,v 1.2 2005-05-11 20:07:09 ntalex Exp $
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
  rect.SetWidth(point1.GetX() < point2.GetX() ? point2.GetX() - point1.GetX() : point1.GetX() - point2.GetX());
  rect.SetWidth(point1.GetY() < point2.GetY() ? point2.GetY() - point1.GetY() : point1.GetY() - point2.GetY());
  return rect;
}

void UpdateBBox(wxSVGElement* parent, wxSVGRect& bbox, bool& empty, wxSVGMatrix& parentMatrix)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  for (; elem != NULL; elem = (wxSVGElement*) elem->GetNext())
  {
	if (elem->GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGMatrix matrix(parentMatrix);
	  wxSVGRect elemBBox;
	  switch (elem->GetDtd())
	  {
		case wxSVG_LINE_ELEMENT:
		{
		  wxSVGLineElement* element = (wxSVGLineElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_POLYLINE_ELEMENT:
		{
		  wxSVGPolylineElement* element = (wxSVGPolylineElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_POLYGON_ELEMENT:
		{
		  wxSVGPolygonElement* element = (wxSVGPolygonElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_RECT_ELEMENT:
		{
		  wxSVGRectElement* element = (wxSVGRectElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_CIRCLE_ELEMENT:
		{
		  wxSVGCircleElement* element = (wxSVGCircleElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_ELLIPSE_ELEMENT:
		{
		  wxSVGEllipseElement* element = (wxSVGEllipseElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		case wxSVG_PATH_ELEMENT:
		{
		  wxSVGPathElement* element = (wxSVGPathElement*) elem;
		  element->UpdateMatrix(matrix);
		  elemBBox = TransformRect(element->GetBBox(), matrix);
		  break;
		}
		
		case wxSVG_SVG_ELEMENT:
		  UpdateBBox(elem, bbox, empty, matrix);
		  continue;
		case wxSVG_G_ELEMENT:
		  ((wxSVGGElement*) elem)->UpdateMatrix(matrix);
		  UpdateBBox(elem, bbox, empty, matrix);
		  continue;
		default:
		  continue;
	  }
	  
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
}

wxSVGRect wxSVGLocatable::GetBBox(wxSVGElement* parent)
{
  wxSVGRect rect;
  bool empty = true;
  wxSVGMatrix matrix;
  UpdateBBox(parent, rect, empty, matrix);
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
