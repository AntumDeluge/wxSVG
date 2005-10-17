//////////////////////////////////////////////////////////////////////////////
// Name:        SVGEllipseElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGEllipseElement.cpp,v 1.4 2005-10-17 14:02:34 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGEllipseElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGEllipseElement::GetBBox()
{
  return wxSVGRect(GetCx().GetBaseVal() - GetRx().GetBaseVal(),
	GetCy().GetBaseVal() - GetRy().GetBaseVal(),
	2*GetRx().GetBaseVal(), 2*GetRy().GetBaseVal());
}

void wxSVGEllipseElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}
