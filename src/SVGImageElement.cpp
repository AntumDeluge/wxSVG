//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/05/30
// RCS-ID:      $Id: SVGImageElement.cpp,v 1.5 2006-01-08 12:44:30 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGImageElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGImageElement::GetBBox(wxSVG_COORDINATES coordinates)
{
  wxSVGRect bbox = wxSVGRect(GetX().GetAnimVal(), GetY().GetAnimVal(),
	GetWidth().GetAnimVal(), GetHeight().GetAnimVal());
  if (coordinates != wxSVG_COORDINATES_USER)
    bbox.MatrixTransform(GetMatrix(coordinates));
  return bbox;
}

wxSVGRect wxSVGImageElement::GetResultBBox(wxSVG_COORDINATES coordinates)
{
  return GetBBox(coordinates);
}

void wxSVGImageElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}

int wxSVGImageElement::GetDefaultWidth()
{
  if (!GetOwnerDocument())
    return 0;
  wxSVGDocument* doc = (wxSVGDocument*) GetOwnerDocument();
  if (m_canvasItem == NULL)
    m_canvasItem = doc->GetCanvas()->CreateItem(this);
  int res = ((wxSVGCanvasImage*)m_canvasItem)->GetDefaultWidth();
  if (!doc->GetCanvas()->IsItemsCached())
  {
    delete m_canvasItem;
    m_canvasItem = NULL;
  }
  return res;
}

int wxSVGImageElement::GetDefaultHeight()
{
  if (!GetOwnerDocument())
    return 0;
  wxSVGDocument* doc = (wxSVGDocument*) GetOwnerDocument();
  if (m_canvasItem == NULL)
	m_canvasItem = doc->GetCanvas()->CreateItem(this);
  int res = ((wxSVGCanvasImage*)m_canvasItem)->GetDefaultHeight();
  if (!doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return res;
}

void wxSVGImageElement::SetDefaultSize()
{
  if (!GetOwnerDocument())
    return;
  wxSVGDocument* doc = (wxSVGDocument*) GetOwnerDocument();
  if (m_canvasItem == NULL)
	m_canvasItem = doc->GetCanvas()->CreateItem(this);
  SetWidth(((wxSVGCanvasImage*)m_canvasItem)->GetDefaultWidth());
  SetHeight(((wxSVGCanvasImage*)m_canvasItem)->GetDefaultHeight());
  if (!doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
}
