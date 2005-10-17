//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTextElement.cpp
// Purpose:     svg text element
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGTextElement.cpp,v 1.2 2005-10-17 14:02:34 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTextElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGTextElement::GetBBox()
{
  if (!GetOwnerDocument())
    return wxSVGRect();
  wxSVGDocument* doc = (wxSVGDocument*) GetOwnerDocument();
  if (m_canvasItem == NULL)
	m_canvasItem = doc->GetCanvas()->CreateItem(this);
  wxSVGRect bbox = m_canvasItem->GetBBox();
  if (!doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return bbox;
}

void wxSVGTextElement::SetCanvasItem(wxSVGCanvasItem* canvasItem)
{
  if (m_canvasItem)
    delete m_canvasItem;
  m_canvasItem = canvasItem;
}
