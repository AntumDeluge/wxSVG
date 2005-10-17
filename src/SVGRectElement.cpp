//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRectElement.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGRectElement.cpp,v 1.3 2005-10-17 14:02:34 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGRectElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGRectElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}

void wxSVGRectElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}
