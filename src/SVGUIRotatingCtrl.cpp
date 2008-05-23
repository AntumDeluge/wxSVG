//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIRotatingCtrl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIRotatingCtrl.cpp,v 1.6 2008-05-23 13:47:53 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIRotatingCtrl.h"
#include "SVGUIWindow.h"
#include <wxSVG/SVGCoordinates.h>
#include <sys/time.h>

const double pi = 3.1415926;

SVGUIRotatingCtrl::SVGUIRotatingCtrl(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
  m_initialised = false;
  m_min_angle = 0;
  m_max_angle = 360;
  m_angle = 0;
  m_center = NULL;
  m_show_center = false;
  m_init_pos = NULL;
  m_last_cursor_position = NULL;
  m_RotatingElement = NULL;
  m_CenterElement = NULL;
  SetName(wxT("RotatingCtrl"));
}

void SVGUIRotatingCtrl::DefineLimits(double min_angle, double max_angle)
{
  m_min_angle = min_angle;
  m_max_angle = max_angle;
  if (m_angle < min_angle)
  {
    m_angle = min_angle;
    Update_Elements();
    Refresh();
  }
  if (m_angle > max_angle)
  {
    m_angle > max_angle;
    Update_Elements();
    Refresh();
  }
}

void SVGUIRotatingCtrl::SetAngle(double angle)
{
  m_angle = angle;
  if (m_min_angle != 0 || m_max_angle != 360)
  {
    if (m_angle < m_min_angle)
      m_angle = m_min_angle;
    if (m_angle > m_max_angle)
      m_angle = m_max_angle;
  }
  else
  {
    while (m_angle >= 360)
    {
      m_angle -= 360;
    }
    while (m_angle <= 0)
    {
      m_angle += 360;
    }
  }
  Update_Elements();
  Refresh();
}

void SVGUIRotatingCtrl::DefineCenter(wxSVGPoint* center)
{
  m_center = center;
  Update_Elements();
  Refresh();
}

wxSVGPoint* SVGUIRotatingCtrl::GetCenter()
{
  return m_center;
}

void SVGUIRotatingCtrl::Rotate(double angle)
{
  m_angle += angle;
  if (m_min_angle != 0 || m_max_angle != 360)
  {
    if (m_angle < m_min_angle)
      m_angle = m_min_angle;
    if (m_angle > m_max_angle)
      m_angle = m_max_angle;
  }
  else
  {
    while (m_angle >= 360)
    {
      m_angle -= 360;
    }
    while (m_angle <= 0)
    {
      m_angle += 360;
    }
  }
  
  Update_Elements();
  Refresh();
}

bool SVGUIRotatingCtrl::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("rotating_id"))
    m_RotatingElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("center_id"))
    m_CenterElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("show_center"))
    m_show_center = attrValue == wxT("true");
  else if (attrName == wxT("min_angle"))
    attrValue.ToDouble(&m_min_angle);
  else if (attrName == wxT("max_angle"))
    attrValue.ToDouble(&m_max_angle);
  else 
    return false;
  return true;
}

bool SVGUIRotatingCtrl::HitTest(wxPoint pt)
{
  if (!m_enable)
    return false;
  wxSVGRect rect(pt.x, pt.y, 1, 1);
  bool res=false;
  if (m_BackgroundElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  if (m_RotatingElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_RotatingElement, rect);
  return res;
}

#define UpdateBBox_macro(element)\
    if (element)\
    {\
      if (empty)\
        {\
      res = wxSVGLocatable::GetElementBBox(element);\
      empty = false;\
        }\
        else\
          res = SumBBox(res, wxSVGLocatable::GetElementBBox(element));\
    }

wxSVGRect SVGUIRotatingCtrl::GetBBox()
{
  wxSVGRect res;
  bool empty = true;
  UpdateBBox_macro(m_BackgroundElement)
  UpdateBBox_macro(m_RotatingElement)
  return res;
}

void SVGUIRotatingCtrl::Update_Elements()
{
  if (!m_initialised)
    Initialize();
  else if (m_init_pos)
  {
    if (m_CenterElement)
    {
      wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_CenterElement);
      if ((bbox.GetX() != m_init_pos->GetX()) ||(bbox.GetY() != m_init_pos->GetY()))
        Initialize();
    }
    else if (m_BackgroundElement)
    {
      wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
      if ((bbox.GetX() != m_init_pos->GetX()) ||(bbox.GetY() != m_init_pos->GetY()))
        Initialize();
    }
    else if (m_RotatingElement)
    {
      wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_RotatingElement);
      if ((bbox.GetX() + bbox.GetWidth() / 2 != m_init_pos->GetX()) ||(bbox.GetY() + bbox.GetHeight() / 2 != m_init_pos->GetY()))
        Initialize();
    }
  }
  if (m_RotatingElement)
  {
    ResetElementMatrix(m_RotatingElement);
    RotateElement(m_RotatingElement, m_angle, m_center);
  }
}

void SVGUIRotatingCtrl::Initialize()
{
  if (m_CenterElement)
  {
    wxSVGRect center_bbox = wxSVGLocatable::GetElementBBox(m_CenterElement);
    m_center = new wxSVGPoint(center_bbox.GetX() + center_bbox.GetWidth() / 2, center_bbox.GetY() + center_bbox.GetHeight() / 2);
    if (m_show_center)
      SetDisplay(m_CenterElement, wxCSS_VALUE_INLINE);
    else
      SetDisplay(m_CenterElement, wxCSS_VALUE_NONE);
    m_init_pos = new wxSVGPoint(center_bbox.GetX(), center_bbox.GetY());
  }
  else if (m_BackgroundElement)
  {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    m_center = new wxSVGPoint(background_bbox.GetX() + background_bbox.GetWidth() / 2, background_bbox.GetY() + background_bbox.GetHeight() / 2);
    m_init_pos = new wxSVGPoint(background_bbox.GetX(), background_bbox.GetY());
  }
  else if (m_RotatingElement)
  {
    wxSVGRect rotating_bbox = wxSVGLocatable::GetElementBBox(m_RotatingElement);
    m_center = new wxSVGPoint(rotating_bbox.GetX() + rotating_bbox.GetWidth() / 2, rotating_bbox.GetY() + rotating_bbox.GetHeight() / 2);
    m_init_pos = new wxSVGPoint(rotating_bbox.GetX() + rotating_bbox.GetWidth() / 2, rotating_bbox.GetY() + rotating_bbox.GetHeight() / 2);
  }
  m_initialised = true;
}

void SVGUIRotatingCtrl::OnLeftDown(wxMouseEvent &event)
{
  m_last_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
  event.Skip();
}

void SVGUIRotatingCtrl::OnMotion(wxMouseEvent &event)
{
  if (m_last_cursor_position)
  {
    wxSVGPoint last_vector = wxSVGPoint(m_last_cursor_position->GetX() - m_center->GetX(), m_last_cursor_position->GetY() - m_center->GetY());
    wxSVGPoint new_vector = wxSVGPoint(event.GetX() - m_center->GetX(), event.GetY() - m_center->GetY());
    double last_vector_length = sqrt(last_vector.GetX() * last_vector.GetX() + last_vector.GetY() * last_vector.GetY());
    double new_vector_length = sqrt(new_vector.GetX() * new_vector.GetX() + new_vector.GetY() * new_vector.GetY());
    double scalar_product = last_vector.GetX() * new_vector.GetX() + last_vector.GetY() * new_vector.GetY();
    double vectorial_product = last_vector.GetX() * new_vector.GetY() - last_vector.GetY() * new_vector.GetX();
    double cosinus = scalar_product / (last_vector_length * new_vector_length);
    if (cosinus > 1)
      cosinus = 1;
    if (cosinus < -1)
      cosinus = -1;
    double angle = acos(cosinus) / pi * 180;
    if (vectorial_product > 0)
    {
      angle = -angle;
    }
    if (m_min_angle != 0 || m_max_angle != 360)
    {
      if (m_angle + angle < m_min_angle)
        angle = m_min_angle - m_angle;
      if (m_angle + angle > m_max_angle)
        angle = m_max_angle - m_angle;
    }
    Rotate(angle);
    wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, m_svguiid);
    m_window->ProcessEvent(evt);
    m_last_cursor_position = new wxSVGPoint(m_center->GetX() + last_vector.GetX() * cos(angle * pi / 180.0) + last_vector.GetY() * sin(angle * pi / 180.0),
                                            m_center->GetY() - last_vector.GetX() * sin(angle * pi / 180.0) + last_vector.GetY() * cos(angle * pi / 180.0));
    
  }
  Update_Elements();
  Refresh();
  
  event.Skip();
}

void SVGUIRotatingCtrl::OnLeftUp(wxMouseEvent &event)
{
  m_last_cursor_position = NULL;
  wxScrollEvent evt(wxEVT_SCROLL_THUMBRELEASE, m_svguiid);
  m_window->ProcessEvent(evt);
  event.Skip();
}
