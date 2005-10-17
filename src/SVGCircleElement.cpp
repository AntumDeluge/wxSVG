//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCircleElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGCircleElement.cpp,v 1.4 2005-10-17 14:02:34 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCircleElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGCircleElement::GetBBox()
{
  return wxSVGRect(GetCx().GetBaseVal() - GetR().GetBaseVal(),
	GetCy().GetBaseVal() - GetR().GetBaseVal(),
	2*GetR().GetBaseVal(), 2*GetR().GetBaseVal());
}

void wxSVGCircleElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}
