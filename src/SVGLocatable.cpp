//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLocatable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGLocatable.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLocatable.h"
#include "svg.h"

void UpdateBBox(wxSVGElement* parent, wxSVGRect& bbox, bool& empty)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  for (; elem != NULL; elem = (wxSVGElement*) elem->GetNext())
  {
	if (elem->GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGRect elemBBox;
	  switch (elem->GetDtd())
	  {
		case wxSVG_LINE_ELEMENT:
		  elemBBox = ((wxSVGLineElement*) elem)->GetBBox();
		  break;
		case wxSVG_POLYLINE_ELEMENT:
		  elemBBox = ((wxSVGPolylineElement*) elem)->GetBBox();
		  break;
		case wxSVG_POLYGON_ELEMENT:
		  elemBBox = ((wxSVGPolygonElement*) elem)->GetBBox();
		  break;
		case wxSVG_RECT_ELEMENT:
		  elemBBox = ((wxSVGRectElement*) elem)->GetBBox();
		  break;
		case wxSVG_CIRCLE_ELEMENT:
		  elemBBox = ((wxSVGCircleElement*) elem)->GetBBox();
		  break;
		case wxSVG_ELLIPSE_ELEMENT:
		  elemBBox = ((wxSVGEllipseElement*) elem)->GetBBox();
		  break;
		case wxSVG_PATH_ELEMENT:
		  elemBBox = ((wxSVGPathElement*) elem)->GetBBox();
		  break;
		
		case wxSVG_SVG_ELEMENT:
		case wxSVG_G_ELEMENT:
		  UpdateBBox(elem, bbox, empty);
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

wxSVGRect wxSVGLocatable::GetBBox()
{
  wxSVGRect rect;
  bool empty = true;
  UpdateBBox((wxSVGElement*) this, rect, empty);
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
