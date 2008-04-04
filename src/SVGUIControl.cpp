//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIControl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIControl.cpp,v 1.3 2008-04-04 16:14:18 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIControl.h"


SVGUIControl::SVGUIControl(wxSVGDocument* doc, wxEvtHandler* window):
	SVGUIElement(doc)
{
	m_window = window;
}

bool SVGUIControl::HitTest(wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	if (m_BackgroundElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	return false;
}

void SVGUIControl::Refresh()
{
	wxCommandEvent refresh_evt(wxEVT_COMMAND_ENTER, -1);
	refresh_evt.SetString(GetId());
	m_window->ProcessEvent(refresh_evt);
}

wxSVGRect SumBBox(wxSVGRect bbox1, wxSVGRect bbox2)
{
	if (bbox1.GetX() > bbox2.GetX())
	{
		bbox1.SetWidth(bbox1.GetWidth() + bbox1.GetX() - bbox2.GetX());
		bbox1.SetX(bbox2.GetX());
	}
	if (bbox1.GetY() > bbox2.GetY())
	{
		bbox1.SetHeight(bbox1.GetHeight() + bbox1.GetY() - bbox2.GetY());
		bbox1.SetY(bbox2.GetY());
	}
	if (bbox1.GetX() + bbox1.GetWidth() < bbox2.GetX() + bbox2.GetWidth())
		bbox1.SetWidth(bbox2.GetX() + bbox2.GetWidth() - bbox1.GetX());
	if (bbox1.GetY() + bbox1.GetHeight() < bbox2.GetY() + bbox2.GetHeight())
		bbox1.SetHeight(bbox2.GetY() + bbox2.GetHeight() - bbox1.GetY());	
	return bbox1;
}

wxSVGRect SVGUIControl::GetBBox()
{
	wxSVGRect res(0, 0, 0, 0);
	if (m_BackgroundElement)
    res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
	return res;
}

void SVGUIControl::OnLeftDown(wxMouseEvent& event)
{
	event.Skip();
}
		
void SVGUIControl::OnLeftUp(wxMouseEvent& event)
{
	event.Skip();
}

void SVGUIControl::OnMotion(wxMouseEvent& event)
{
	event.Skip();
}

void SVGUIControl::OnChar(wxKeyEvent& event)
{
	event.Skip();
}
