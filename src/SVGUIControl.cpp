//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIControl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIControl.cpp,v 1.1 2007-08-31 08:56:15 gusstdie Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIControl.h"

BEGIN_EVENT_TABLE(SVGUIControlBase, wxEvtHandler)
  EVT_LEFT_DOWN	(SVGUIControlBase::OnLeftDown)
  EVT_LEFT_UP	(SVGUIControlBase::OnLeftUp)
  EVT_MOTION	(SVGUIControlBase::OnMotion)
  EVT_CHAR  (SVGUIControlBase::OnChar)
END_EVENT_TABLE()

IMPLEMENT_ABSTRACT_CLASS(SVGUIControlBase, wxObject)

SVGUIControlBase::SVGUIControlBase()
{
}


SVGUIControlBase::SVGUIControlBase(wxSVGDocument* doc, wxEvtHandler* window):
	SVGUIElement(doc)
{
	m_window = window;
	m_enable = true;
}

bool SVGUIControlBase::HitTest(wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	if (m_BackgroundElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	return false;
}

void SVGUIControlBase::Refresh()
{
	wxCommandEvent refresh_evt(wxEVT_COMMAND_ENTER, -1);
	refresh_evt.m_commandString = GetId();
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


wxSVGRect SVGUIControlBase::GetBBox()
{
	wxSVGRect res(0, 0, 0, 0);
	if (m_BackgroundElement)
        res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
	return res;
}


void SVGUIControlBase::SendMouseEvent(wxMouseEvent& event)
{
	((wxEvtHandler*)this)->ProcessEvent(event);
}

void SVGUIControlBase::SendKeyEvent(wxKeyEvent& event)
{
	((wxEvtHandler*)this)->ProcessEvent(event);
}

void SVGUIControlBase::SendNotebookEvent(wxNotebookEvent& event)
{
	((wxEvtHandler*)this)->ProcessEvent(event);
}

void SVGUIControlBase::SendScrollEvent(wxScrollEvent& event)
{
	((wxEvtHandler*)this)->ProcessEvent(event);
}

void SVGUIControlBase::SendCommandEvent(wxCommandEvent& event)
{
	((wxEvtHandler*)this)->ProcessEvent(event);
}

void SVGUIControlBase::OnLeftDown(wxMouseEvent& event)
{
	wxLogError(wxT("SVGUIControlBase %s OnLeftDown\n"), GetId().c_str());
	event.Skip();
}
		
void SVGUIControlBase::OnLeftUp(wxMouseEvent& event)
{
	wxLogError(wxT("SVGUIControlBase %s OnLeftUp\n"), GetId().c_str());
	event.Skip();
}

void SVGUIControlBase::OnMotion(wxMouseEvent& event)
{
	wxLogError(wxT("SVGUIControlBase %s OnMotion\n"), GetId().c_str());
	event.Skip();
}

void SVGUIControlBase::OnChar(wxKeyEvent& event)
{
	wxLogError(wxT("SVGUIControlBase %s OnKeyDown\n"), GetId().c_str());
	event.Skip();
}



IMPLEMENT_ABSTRACT_CLASS(SVGUIControl, SVGUIControlBase)
SVGUIControl::SVGUIControl()
{
	m_enable = true;
}

SVGUIControl::SVGUIControl(wxSVGDocument* doc, wxEvtHandler* window):SVGUIControlBase(doc,window)
{
 	m_window = window;
}
