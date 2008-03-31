//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUINoteBook.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/09/08
// RCS-ID:      $Id: SVGUINoteBook.cpp,v 1.3 2008-03-31 16:54:41 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUINoteBook.h"
#include "SVGUIWindow.h"

SVGUINoteBook::SVGUINoteBook(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
	m_current_page = 0;
	m_GroupElement = NULL;
	m_Page0Element = NULL;
	m_Page1Element = NULL;
	m_ContentElement = NULL;
	m_Page0Content = NULL;
	m_Page1Content = NULL;
	m_enable = true;
	SetName(wxT("NoteBook"));
}

bool SVGUINoteBook::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("group_id"))
  	m_GroupElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("page0_id"))
  	m_Page0Element = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("page1_id"))
  	m_Page1Element = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("content_id"))
  {
    m_ContentElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  	SetDisplay(m_ContentElement, wxCSS_VALUE_NONE);
  }
  else if (attrName == wxT("page0_content"))
  	m_Page0Content = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("page1_content"))
  	m_Page1Content = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else
    return false;
  return true;
}

bool SVGUINoteBook::HitTest(wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	bool res=false;
	if (m_BackgroundElement)
		res |= m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	if (m_Page0Element)
		res |= m_doc->GetRootElement()->CheckIntersection(*m_Page0Element, rect);
	if (m_Page1Element)
		res |= m_doc->GetRootElement()->CheckIntersection(*m_Page1Element, rect);
	return res;
}

#define UpdateBBox_macro(element)\
		if (element)\
		{\
		  if (res.IsEmpty())\
			res = wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT);\
	  	  else\
	  	  	res = SumBBox(res, wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT));\
		}

wxSVGRect SVGUINoteBook::GetBBox()
{
	wxSVGRect res;
	UpdateBBox_macro(m_BackgroundElement)
	UpdateBBox_macro(m_Page0Element)
	UpdateBBox_macro(m_Page1Element)
	return res;
}

void SVGUINoteBook::Update_Elements()
{
	if (!m_current_page)
	{
		m_current_page =0;
		wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_ContentElement, wxSVG_COORDINATES_VIEWPORT);
		MoveElement(m_Page0Content, rect.GetX(),rect.GetY());
	}
	if (m_current_page == 0)
	{
		m_GroupElement->SetChildren((wxSvgXmlElement*)m_Page1Element);
		m_Page1Element->SetNext((wxSvgXmlElement*)m_BackgroundElement);
		m_BackgroundElement->SetNext((wxSvgXmlElement*)m_Page0Element);
		m_Page0Element->SetNext(NULL);
	}
	else if (m_current_page == 1)
	{
		m_GroupElement->SetChildren((wxSvgXmlElement*)m_Page0Element);
		m_Page0Element->SetNext((wxSvgXmlElement*)m_BackgroundElement);
		m_BackgroundElement->SetNext((wxSvgXmlElement*)m_Page1Element);
		m_Page1Element->SetNext(NULL);
	}
}

void SVGUINoteBook::RefreshContentSize(double scale)
{
	wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_ContentElement, wxSVG_COORDINATES_VIEWPORT);
	wxPoint point((int)(rect.GetX()*scale),(int)(rect.GetY()*scale));
	wxSize size((int)(rect.GetWidth()*scale),(int)(rect.GetHeight()*scale));
}

void SVGUINoteBook::SetCurrentPage(int page)
{
	if (page >= 0 && page <= 1)
	{
		MoveTabs(page);
	}
}

void SVGUINoteBook::MoveTabs(int new_page)
{
	if (m_current_page == new_page)
		return;
	
	if (m_current_page == 1)
	{
		wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_Page1Content, wxSVG_COORDINATES_VIEWPORT);
		MoveElement(m_Page1Content, (-1)*(rect.GetWidth()+50),(-1)*(rect.GetHeight()+50));
	}
	else if (m_current_page == 0)
	{
		wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_Page0Content, wxSVG_COORDINATES_VIEWPORT);
		MoveElement(m_Page0Content, (-1)*(rect.GetWidth()+50),(-1)*(rect.GetHeight()+50));
	}

	wxSVGRect rect = wxSVGLocatable::GetElementResultBBox(m_ContentElement, wxSVG_COORDINATES_VIEWPORT);
	if (new_page == 0)
	{
		MoveElement(m_Page0Content, rect.GetX(),rect.GetY());
		m_current_page = 0;
	}
	else if (new_page == 1)
	{
		MoveElement(m_Page1Content, rect.GetX(),rect.GetY());
		m_current_page = 1;
	}
	Update_Elements();
	Refresh();
}


void SVGUINoteBook::OnLeftDown(wxMouseEvent &event)
{
	wxSVGRect rect(event.GetX(), event.GetY(), 1, 1);
	if (m_Page0Element && m_doc->GetRootElement()->CheckIntersection(*m_Page0Element, rect) && m_current_page != 0)
	{
		MoveTabs(0);
		wxCommandEvent evt(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, SVGUIWindow::GetSVGUIID(GetName()));
		m_window->ProcessEvent(evt);
	}
	else if (m_Page1Element && m_doc->GetRootElement()->CheckIntersection(*m_Page1Element, rect) && m_current_page != 1)
	{
		MoveTabs(1);
		wxCommandEvent evt(wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, SVGUIWindow::GetSVGUIID(GetName()));
		m_window->ProcessEvent(evt);
	}
	event.Skip();
}



void SVGUINoteBook::OnLeftUp(wxMouseEvent& event)
{
	event.Skip();
}

void SVGUINoteBook::OnMotion(wxMouseEvent& event)
{
	event.Skip();
}
