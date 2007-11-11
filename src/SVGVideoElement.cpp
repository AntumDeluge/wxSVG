//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/11/23
// RCS-ID:      $Id: SVGVideoElement.cpp,v 1.3 2007-11-11 20:05:46 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGVideoElement.h"
#include "SVGCanvas.h"
#include <wx/log.h>

wxSVGRect wxSVGVideoElement::GetBBox(wxSVG_COORDINATES coordinates)
{
  wxSVGRect bbox = wxSVGRect(GetX().GetAnimVal(), GetY().GetAnimVal(),
	GetWidth().GetAnimVal(), GetHeight().GetAnimVal());
  if (coordinates != wxSVG_COORDINATES_USER)
    bbox.MatrixTransform(GetMatrix(coordinates));
  return bbox;
}

wxSVGRect wxSVGVideoElement::GetResultBBox(wxSVG_COORDINATES coordinates)
{
  return GetBBox(coordinates);
}

void wxSVGVideoElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}

double wxSVGVideoElement::GetDuration()
{
  WX_SVG_CREATE_M_CANVAS_ITEM
  double duration = ((wxSVGCanvasVideo*)m_canvasItem)->GetDuration();
  WX_SVG_CLEAR_M_CANVAS_ITEM
  return duration;
}
