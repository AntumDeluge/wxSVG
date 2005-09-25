//////////////////////////////////////////////////////////////////////////////
// Name:        SVGImageElement.cpp
// Purpose:     SVG image element
// Author:      Alex Thuering
// Created:     2005/05/30
// RCS-ID:      $Id: SVGImageElement.cpp,v 1.3 2005-09-25 11:36:25 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGImageElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGImageElement::GetBBox()
{
  return wxSVGRect(GetX().GetBaseVal(), GetY().GetBaseVal(),
	GetWidth().GetBaseVal(), GetHeight().GetBaseVal());
}

int wxSVGImageElement::GetDefaultWidth()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  int res = ((wxSVGCanvasImage*)m_canvasItem)->GetDefaultWidth();
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return res;
}

int wxSVGImageElement::GetDefaultHeight()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  int res = ((wxSVGCanvasImage*)m_canvasItem)->GetDefaultHeight();
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return res;
}

void wxSVGImageElement::SetDefaultSize()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  SetWidth(((wxSVGCanvasImage*)m_canvasItem)->GetDefaultWidth());
  SetHeight(((wxSVGCanvasImage*)m_canvasItem)->GetDefaultHeight());
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
}
