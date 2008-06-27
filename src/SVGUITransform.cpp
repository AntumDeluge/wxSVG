//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITransform.cpp
// Purpose:     
// Author:      Jonathan Hurtrel
// Created:     2007/08/13
// RCS-ID:      $Id: SVGUITransform.cpp,v 1.7 2008-06-27 17:07:09 etisserant Exp $
// Copyright:   (c) Jonathan Hurtrel
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUITransform.h"
#include "SVGUIWindow.h"
#include <wxSVG/SVGCoordinates.h>


SVGUITransform::SVGUITransform(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
  m_initialised = false;
  m_angle = 0;
  m_x_pos = 0;
  m_y_pos = 0;
  m_x_scale = 1;
  m_y_scale = 1;
  m_moving_zone = NULL;
  m_last_cursor_position = NULL;
  m_MovingElement = NULL;
  SetName(wxT("Transform"));
}

void SVGUITransform::Initialize()
{
  if (m_BackgroundElement && m_MovingElement)
  {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
    m_moving_zone = new wxSVGRect(background_bbox.GetX(), background_bbox.GetY(), background_bbox.GetWidth(), background_bbox.GetHeight());
    //m_x_pos = (m_moving_zone->GetWidth() - moving_bbox.GetWidth()) / 2;
    //m_y_pos = (m_moving_zone->GetHeight() - moving_bbox.GetHeight()) / 2;
  }
  else if (m_MovingElement)
  {
    wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
    m_moving_zone = new wxSVGRect(moving_bbox.GetX(), moving_bbox.GetY(), 0, 0);
  }
  m_initialised = true;
}

bool SVGUITransform::HitTest(wxPoint pt)
{
  if (!m_enable)
    return false;
  wxSVGRect rect(pt.x, pt.y, 1, 1);
  if (m_BackgroundElement)
    return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  else if (m_MovingElement)
    return m_doc->GetRootElement()->CheckIntersection(*m_MovingElement, rect);
  return false;
}

wxSVGRect SVGUITransform::GetBBox()
{
  wxSVGRect res(0, 0, 0, 0);
  if (m_BackgroundElement)
    res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
  else if (m_MovingElement)
    res = wxSVGLocatable::GetElementResultBBox(m_MovingElement, wxSVG_COORDINATES_VIEWPORT);
  return res;
}

void SVGUITransform::Update_Elements()
{
  if (!m_initialised)
    Initialize();
  else if (m_moving_zone)
  {
    if (m_BackgroundElement && m_MovingElement)
    {
      wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
      if ((bbox.GetX() != m_moving_zone->GetX()) ||(bbox.GetY() != m_moving_zone->GetY()))
        Initialize();
    }
    else if (m_MovingElement)
    {
      wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
      if ((bbox.GetX() != m_moving_zone->GetX()) ||(bbox.GetY() != m_moving_zone->GetY()))
        Initialize();
    }
  }
  if (m_MovingElement)
  {
    wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
    wxSVGPoint *rotate_center = new wxSVGPoint(moving_bbox.GetX() + moving_bbox.GetWidth() / 2, moving_bbox.GetY() + moving_bbox.GetHeight() / 2); 
    ResetElementMatrix(m_MovingElement);
    RotateElement(m_MovingElement, m_angle, rotate_center);
    ScaleElement(m_MovingElement, m_x_scale, m_y_scale);
    MoveElement(m_MovingElement, m_moving_zone->GetX() + m_x_pos, m_moving_zone->GetY() + m_y_pos);
  }
}

void SVGUITransform::Move(double x, double y)
{
  m_x_pos = x;
  m_y_pos = y;
  Update_Elements();
  Refresh();
}

void SVGUITransform::Scale(double x, double y)
{
  m_x_scale = x;
  m_y_scale = y;
  Update_Elements();
  Refresh();
}

void SVGUITransform::Rotate(double angle)
{
  m_angle = angle;
  Update_Elements();
  Refresh();
}

bool SVGUITransform::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("moving_id"))
    m_MovingElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else 
    return false;
  return true;
}

void SVGUITransform::OnLeftDown(wxMouseEvent &event)
{
  wxSVGRect rect(event.GetX(), event.GetY(), 1, 1);
  if (m_MovingElement && m_doc->GetRootElement()->CheckIntersection(*m_MovingElement, rect))
    m_last_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
  event.Skip();
}

void SVGUITransform::OnMotion(wxMouseEvent &event)
{
  if (m_last_cursor_position && m_MovingElement)
  {
    wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
    double move_x = event.GetX() - m_last_cursor_position->GetX();
    double move_y = event.GetY() - m_last_cursor_position->GetY();
    if (m_BackgroundElement)
    {
      wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
      if (m_x_pos + move_x < 0)
        move_x = -m_x_pos;
      else if (m_x_pos + move_x + moving_bbox.GetWidth() > m_moving_zone->GetWidth())
        move_x = m_moving_zone->GetWidth() - moving_bbox.GetWidth() - m_x_pos;
      if (m_y_pos + move_y < 0)
        move_y = -m_y_pos;
      else if (m_y_pos + move_y + moving_bbox.GetHeight() > m_moving_zone->GetHeight())
        move_y = m_moving_zone->GetHeight() - moving_bbox.GetHeight() - m_y_pos;
    }
    Move(m_x_pos + move_x, m_y_pos + move_y);
    wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, m_svguiid);
    m_window->ProcessEvent(evt);
    m_last_cursor_position = new wxSVGPoint(m_last_cursor_position->GetX() + move_x, m_last_cursor_position->GetY() + move_y);
  }
  event.Skip();
}

void SVGUITransform::OnLeftUp(wxMouseEvent &event)
{
  m_last_cursor_position = NULL;
  wxScrollEvent evt(wxEVT_SCROLL_THUMBRELEASE, m_svguiid);
  m_window->ProcessEvent(evt);
  event.Skip();
}
