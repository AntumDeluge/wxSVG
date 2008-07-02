//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIButton.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIButton.cpp,v 1.9 2008-07-02 14:18:33 etisserant Exp $
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
  m_toggle = false;
  m_state = false;
  SetName(wxT("Button"));
}

SVGUIButton::~SVGUIButton() {
  m_SelectedElement = NULL;
  m_UnselectedElement = NULL;
}

bool SVGUIButton::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("selected_id"))
    m_SelectedElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("unselected_id"))
    m_UnselectedElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("toggle"))
    m_toggle = attrValue.compare(wxT("true")) == 0;
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

void SVGUIButton::SetToggle(bool toggle)
{
  m_state = toggle;
  m_up = !m_state;
  Update_Elements();
  Refresh();
}

bool SVGUIButton::GetToggle()
{
  return m_state;
}

void SVGUIButton::OnLeftDown(wxMouseEvent &event)
{
  if (m_toggle)
    m_up = m_state;
  else
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
    if ((m_toggle && (m_state && !over || !m_state && over)) ||
        (!m_toggle && m_up && over))
    {
      m_up = false;
      Update_Elements();
      Refresh();
    }
    else if ((m_toggle && (m_state && over || !m_state && !over)) ||
             (!m_toggle && !m_up && !over))
    {
      m_up = true;
      Update_Elements();
      Refresh();
    }
  }
}

void SVGUIButton::OnLeftUp(wxMouseEvent &event)
{
  if (m_toggle)
  {
    if (m_state && m_up)
    {
      m_state = false;
      Update_Elements();
      wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, m_svguiid);
      m_window->ProcessEvent(evt);
      Refresh();
    }
    else if (!m_state && !m_up)
    {
      m_state = true;
      Update_Elements();
      wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, m_svguiid);
      m_window->ProcessEvent(evt);
      Refresh();
    }
  }
  else if (!m_up)
  {
    m_up = true;
    Update_Elements();
    wxCommandEvent evt(wxEVT_COMMAND_BUTTON_CLICKED, m_svguiid);
    m_window->ProcessEvent(evt);
    Refresh();
  }
}
