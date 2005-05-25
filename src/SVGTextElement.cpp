//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTextElement.cpp
// Purpose:     svg text element
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGTextElement.cpp,v 1.1 2005-05-25 12:18:10 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTextElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGTextElement::GetBBox()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  wxSVGRect bbox = m_canvasItem->GetBBox();
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return bbox;
}
