//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTextElement.cpp
// Purpose:     svg text element
// Author:      Alex Thuering
// Created:     2005/05/10
// RCS-ID:      $Id: SVGTextElement.cpp,v 1.1.2.2 2005-09-29 14:53:22 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTextElement.h"
#include "SVGCanvas.h"

wxSVGRect wxSVGTextElement::GetBBox()
{
  if (m_canvasItem == NULL)
	m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
  wxSVGMatrix matrix = GetCTM();
  wxSVGRect bbox = m_canvasItem->GetBBox(matrix);
  if (!m_doc->GetCanvas()->IsItemsCached())
  {
	delete m_canvasItem;
	m_canvasItem = NULL;
  }
  return bbox;
}

long wxSVGTextElement::GetNumberOfChars()
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	long number = ((wxSVGCanvasText*)m_canvasItem)->GetNumberOfChars();
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return number;
}

double wxSVGTextElement::GetComputedTextLength()
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	double length = ((wxSVGCanvasText*)m_canvasItem)->GetComputedTextLength();
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return length;
}

double wxSVGTextElement::GetSubStringLength(unsigned long charnum, unsigned long nchars)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	double length = ((wxSVGCanvasText*)m_canvasItem)->GetSubStringLength(charnum, nchars);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return length;
}

wxSVGPoint wxSVGTextElement::GetStartPositionOfChar(unsigned long charnum)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	wxSVGPoint position = ((wxSVGCanvasText*)m_canvasItem)->GetStartPositionOfChar(charnum);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return position;
}

wxSVGPoint wxSVGTextElement::GetEndPositionOfChar(unsigned long charnum)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	wxSVGPoint position = ((wxSVGCanvasText*)m_canvasItem)->GetEndPositionOfChar(charnum);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return position;
}

wxSVGRect wxSVGTextElement::GetExtentOfChar(unsigned long charnum)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	wxSVGRect extent = ((wxSVGCanvasText*)m_canvasItem)->GetExtentOfChar(charnum);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return extent;
}

double wxSVGTextElement::GetRotationOfChar(unsigned long charnum)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	double rotation = ((wxSVGCanvasText*)m_canvasItem)->GetRotationOfChar(charnum);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return rotation;
}

long wxSVGTextElement::GetCharNumAtPosition(const wxSVGPoint& point)
{
	if (m_canvasItem == NULL)
		m_canvasItem = m_doc->GetCanvas()->CreateItem(this);
	long charnum = ((wxSVGCanvasText*)m_canvasItem)->GetCharNumAtPosition(point);
	if (!m_doc->GetCanvas()->IsItemsCached())
  	{
		delete m_canvasItem;
		m_canvasItem = NULL;
  	}
  	return charnum;
}
