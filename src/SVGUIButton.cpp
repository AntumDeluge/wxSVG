//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIButton.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIButton.cpp,v 1.1 2007-08-31 08:56:15 gusstdie Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIButton.h"
#include "SVGUIWindow.h"

SVGUIButton::SVGUIButton(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
	m_up = true;
	m_SelectedElement = NULL;
	m_UnselectedElement = NULL;
	m_enable = true;
	SetName(wxT("Button"));
}

bool SVGUIButton::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (attrName == wxT("background_id"))
  	m_BackgroundElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("selected_id"))
  	m_SelectedElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("unselected_id"))
  	m_UnselectedElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else 
    return false;
  return true;
}

bool SVGUIButton::HitTest(wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	if (m_BackgroundElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	else if (m_up && m_UnselectedElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_UnselectedElement, rect);
	else if (!m_up && m_SelectedElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_SelectedElement, rect);
	return false;
}

wxSVGRect SVGUIButton::GetBBox()
{
	wxSVGRect res(0, 0, 0, 0);
    if (m_BackgroundElement)
		res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
	else if (m_up && m_UnselectedElement)
		res = wxSVGLocatable::GetElementResultBBox(m_UnselectedElement, wxSVG_COORDINATES_VIEWPORT);
	else if (!m_up && m_SelectedElement)
		res = wxSVGLocatable::GetElementResultBBox(m_SelectedElement, wxSVG_COORDINATES_VIEWPORT);
	return res;
}

void SVGUIButton::Update_Elements()
{
	if (m_up)
	{
		SetDisplay(m_SelectedElement, wxCSS_VALUE_NONE);
		SetDisplay(m_UnselectedElement, wxCSS_VALUE_INLINE);
	}
	else
	{
		SetDisplay(m_SelectedElement, wxCSS_VALUE_INLINE);
		SetDisplay(m_UnselectedElement, wxCSS_VALUE_NONE);
	}
}

void SVGUIButton::OnLeftDown(wxMouseEvent &event)
{
	m_up = false;
	Update_Elements();
	Refresh();
}

void SVGUIButton::OnMotion(wxMouseEvent &event)
{
	if (event.LeftIsDown())
	{
		wxPoint pt(event.GetX(), event.GetY());
		bool over = HitTest(pt);
		if (m_up && over)
		{
			m_up = false;
			Update_Elements();
			Refresh();
		}
		else if (!m_up && !over)
		{
			m_up = true;
			Update_Elements();
			Refresh();
		}
	}
}

void SVGUIButton::OnLeftUp(wxMouseEvent &event)
{
	if (!m_up)
	{
		m_up = true;
		Update_Elements();
		wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, SVGUIWindow::GetSVGUIID(GetId()));
		m_window->ProcessEvent(evt);
		Refresh();
	}
}
