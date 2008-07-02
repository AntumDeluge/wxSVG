//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIScrollBar.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIScrollBar.cpp,v 1.8 2008-07-02 14:18:33 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIScrollBar.h"
#include "SVGUIWindow.h"

SVGUIScrollBar::SVGUIScrollBar(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
  m_initialised = false;
  m_position = 0;
  m_thumb = 0;
  m_range = 0;
  m_last_cursor_position = NULL;
  m_ThumbBackElement = NULL;
  m_ThumbMiddleElement = NULL;
  m_UpArrowElement = NULL;
  m_DownArrowElement = NULL;
  SetName(wxT("ScrollBar"));
}

SVGUIScrollBar::~SVGUIScrollBar() {
  m_last_cursor_position = NULL;
  m_ThumbBackElement = NULL;
  m_ThumbMiddleElement = NULL;
  m_UpArrowElement = NULL;
  m_DownArrowElement = NULL;
}

bool SVGUIScrollBar::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("orientation"))
  {
    if (attrValue == wxT("vertical"))
    {
      m_direction.SetX(0);
      m_direction.SetY(1);
    }
    else if (attrValue == wxT("horizontal"))
    {
      m_direction.SetX(1);
      m_direction.SetY(0);
    }
  }
  else if (attrName == wxT("thumb_back_id"))
    m_ThumbBackElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("thumb_middle_id"))
    m_ThumbMiddleElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("uparrow_id"))
    m_UpArrowElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("downarrow_id"))
    m_DownArrowElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else 
    return false;
  return true;
}

bool SVGUIScrollBar::HitTest(wxPoint pt)
{
  if (!m_enable)
    return false;
  wxSVGRect rect(pt.x, pt.y, 1, 1);
  bool res=false;
  if (m_BackgroundElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  if (m_ThumbBackElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_ThumbBackElement, rect);
  if (m_ThumbMiddleElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_ThumbMiddleElement, rect);
  if (m_UpArrowElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_UpArrowElement, rect);
  if (m_DownArrowElement)
    res |= m_doc->GetRootElement()->CheckIntersection(*m_DownArrowElement, rect);
  return res;
}

#define UpdateBBox_macro(element)\
    if (element)\
    {\
      if (res.IsEmpty())\
        res = wxSVGLocatable::GetElementBBox(element);\
      else\
        res = SumBBox(res, wxSVGLocatable::GetElementBBox(element));\
    }

wxSVGRect SVGUIScrollBar::GetBBox()
{
  wxSVGRect res;
  UpdateBBox_macro(m_BackgroundElement)
  UpdateBBox_macro(m_ThumbBackElement)
  UpdateBBox_macro(m_ThumbMiddleElement)
  UpdateBBox_macro(m_UpArrowElement)
  UpdateBBox_macro(m_DownArrowElement)
  return res;
}

void SVGUIScrollBar::Init_ScrollBar(unsigned int position, unsigned int thumb, unsigned int range)
{
  m_position = position;
  m_thumb = thumb;
  m_range = range;
  if (!m_initialised)
    Initialize();
  m_position_size.SetX(0);
  m_position_size.SetY(0);
  if (range > 0)
  {
    if (m_BackgroundElement)
    {
      wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
      if (m_direction.GetX() > 0)
        m_position_size.SetX((background_bbox.GetWidth() - 2 * m_offset.GetX()) / (float)m_range);
      if (m_direction.GetY() > 0)
        m_position_size.SetY((background_bbox.GetHeight() - 2 * m_offset.GetY()) / (float)m_range);
    }
    else if (m_UpArrowElement && m_DownArrowElement)
    {
      wxSVGRect uparrow_bbox = wxSVGLocatable::GetElementBBox(m_UpArrowElement);
      wxSVGRect downarrow_bbox = wxSVGLocatable::GetElementBBox(m_DownArrowElement);
      if (m_direction.GetX() > 0)
        m_position_size.SetX((downarrow_bbox.GetX() + downarrow_bbox.GetWidth() - uparrow_bbox.GetX() - 2 * m_offset.GetX()) / (float)m_range);
      if (m_direction.GetY() > 0)
        m_position_size.SetY((downarrow_bbox.GetY() + downarrow_bbox.GetHeight() - uparrow_bbox.GetY() - 2 * m_offset.GetY()) / (float)m_range);
    }
  }
  Update_Elements();
  Refresh();
}

void SVGUIScrollBar::Update_Elements()
{
  if (!m_initialised)
    Initialize();
    
  if (m_thumb < m_range && m_ThumbBackElement)
  {
    ResetElementMatrix(m_ThumbBackElement);
    double Xposition = 0;
    double Yposition = 0;
    double Xscale = 1;
    double Yscale = 1;
    wxSVGRect thumb_bbox = wxSVGLocatable::GetElementBBox(m_ThumbBackElement);
    double new_Xposition = 0;
    double new_Yposition = 0;
    double new_width = thumb_bbox.GetWidth();
    double new_height = thumb_bbox.GetHeight();
    if (m_position_size.GetX() > 0)
    {
      new_Xposition = m_position_size.GetX() * (float)m_position;
      new_width = m_position_size.GetX() * (float)m_thumb;
    }
    if (m_position_size.GetY() > 0)
    {
      new_Yposition = m_position_size.GetY() * (float)m_position;
      new_height = m_position_size.GetY() * (float)m_thumb;
    }
    Xscale = new_width / thumb_bbox.GetWidth();
    Yscale = new_height / thumb_bbox.GetHeight();
    if (m_BackgroundElement)
    {
      wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
      Xposition = background_bbox.GetX() + m_offset.GetX() + new_Xposition;
      Yposition = background_bbox.GetY() + m_offset.GetY() + new_Yposition;
    }
    else if (m_UpArrowElement && m_DownArrowElement)
    {
      wxSVGRect uparrow_bbox = wxSVGLocatable::GetElementBBox(m_UpArrowElement);
      Xposition = uparrow_bbox.GetX() + uparrow_bbox.GetWidth() + m_offset.GetX() + new_Xposition;
      Yposition = uparrow_bbox.GetY() + uparrow_bbox.GetHeight() + m_offset.GetY() + new_Yposition;
    }
    MoveElement(m_ThumbBackElement, Xposition, Yposition);
    ScaleElement(m_ThumbBackElement, Xscale, Yscale);
    if (m_ThumbMiddleElement)
    {
      wxSVGRect middle_bbox = wxSVGLocatable::GetElementBBox(m_ThumbMiddleElement);
      Xposition += (new_width - middle_bbox.GetWidth()) / 2;
      Yposition += (new_height - middle_bbox.GetHeight()) / 2;
      
      MoveElement(m_ThumbMiddleElement, Xposition, Yposition);
    }
    SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_ThumbBackElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_ThumbMiddleElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_UpArrowElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_DownArrowElement, wxCSS_VALUE_INLINE);
  }
  else
  {
    SetDisplay(m_BackgroundElement, wxCSS_VALUE_NONE);
    SetDisplay(m_ThumbBackElement, wxCSS_VALUE_NONE);
    SetDisplay(m_ThumbMiddleElement, wxCSS_VALUE_NONE);
    SetDisplay(m_UpArrowElement, wxCSS_VALUE_NONE);
    SetDisplay(m_DownArrowElement, wxCSS_VALUE_NONE);
  }
}

void SVGUIScrollBar::Initialize()
{
  if (m_BackgroundElement && m_ThumbBackElement)
  {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    wxSVGRect thumb_bbox = wxSVGLocatable::GetElementBBox(m_ThumbBackElement);
    m_offset.SetX(thumb_bbox.GetX() - background_bbox.GetX());
    m_offset.SetY(thumb_bbox.GetY() - background_bbox.GetY());
  }
  else if (m_UpArrowElement && m_DownArrowElement && m_ThumbBackElement)
  {
    wxSVGRect thumb_bbox = wxSVGLocatable::GetElementBBox(m_ThumbBackElement);
    wxSVGRect uparrow_bbox = wxSVGLocatable::GetElementBBox(m_UpArrowElement);
    m_offset.SetX(thumb_bbox.GetX() - uparrow_bbox.GetX() - uparrow_bbox.GetWidth());
    m_offset.SetY(thumb_bbox.GetY() - uparrow_bbox.GetY() - uparrow_bbox.GetHeight());
  }
  m_initialised = true;
}

void SVGUIScrollBar::MoveThumbByUnit(int direction)
{
  int new_position = m_position + direction;
  if (new_position < 0)
    m_position = 0;
  else if (new_position > (int)m_range - (int)m_thumb)
    m_position = m_range - m_thumb;
  else
    m_position = new_position;
  Update_Elements();
  Refresh();
}

void SVGUIScrollBar::MoveThumbByPage(int direction)
{
  if (m_thumb > 1)
    MoveThumbByUnit(direction * (m_thumb - 1));
  else
    MoveThumbByUnit(direction);
}

void SVGUIScrollBar::SetThumbPosition(int position)
{
  if (position < 0)
    m_position = 0;
  else if (position > (int)m_range - (int)m_thumb)
    m_position = m_range - m_thumb;
  else
    m_position = position;
  Update_Elements();
  Refresh();
}

void SVGUIScrollBar::OnLeftDown(wxMouseEvent &event)
{
  wxSVGRect rect(event.GetX(), event.GetY(), 1, 1);
  if (m_UpArrowElement && m_doc->GetRootElement()->CheckIntersection(*m_UpArrowElement, rect))
  {
    MoveThumbByUnit(-1);
    wxScrollEvent evt(wxEVT_SCROLL_LINEUP, m_svguiid);
    m_window->ProcessEvent(evt);
  }
  else if (m_DownArrowElement && m_doc->GetRootElement()->CheckIntersection(*m_DownArrowElement, rect))
  {
    MoveThumbByUnit(1);
    wxScrollEvent evt(wxEVT_SCROLL_LINEDOWN, m_svguiid);
    m_window->ProcessEvent(evt);
  }
  else if (m_BackgroundElement && m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect))
  {
    if (m_ThumbBackElement && m_doc->GetRootElement()->CheckIntersection(*m_ThumbBackElement, rect))
      m_last_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
    else if (m_ThumbBackElement)
    {
      wxSVGRect thumb_bbox = wxSVGLocatable::GetElementBBox(m_ThumbBackElement);
      if (m_direction.GetX() > 0)
      {
        if (event.GetX() < thumb_bbox.GetX())
        {
          MoveThumbByPage(-1);
          wxScrollEvent evt(wxEVT_SCROLL_PAGEUP, m_svguiid);
          m_window->ProcessEvent(evt);
        }
        else if (event.GetX() > thumb_bbox.GetX() + thumb_bbox.GetWidth())
        {
          MoveThumbByPage(1);
          wxScrollEvent evt(wxEVT_SCROLL_PAGEDOWN, m_svguiid);
          m_window->ProcessEvent(evt);
        }
      }
      else if (m_direction.GetY() > 0)
      {
        if (event.GetY() < thumb_bbox.GetY())
        {
          MoveThumbByPage(-1);
          wxScrollEvent evt(wxEVT_SCROLL_PAGEUP, m_svguiid);
          m_window->ProcessEvent(evt);
        }
        else if (event.GetY() > thumb_bbox.GetY() + thumb_bbox.GetHeight())
        {
          MoveThumbByPage(1);
          wxScrollEvent evt(wxEVT_SCROLL_PAGEDOWN, m_svguiid);
          m_window->ProcessEvent(evt);
        }
      }
    }
  }
  event.Skip();
}

void SVGUIScrollBar::OnMotion(wxMouseEvent &event)
{
  if (m_last_cursor_position)
  {
    int dp = 0;
    if (m_direction.GetX() > 0)
    {
      dp = (int)((event.GetX() - m_last_cursor_position->GetX()) / m_position_size.GetX());
      if (abs(dp) > 0)
      {
        MoveThumbByUnit(dp);
        wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, m_svguiid);
        m_window->ProcessEvent(evt);
        m_last_cursor_position->SetX(m_last_cursor_position->GetX() + m_position_size.GetX() * dp);
      }
    }
    if (m_direction.GetY() > 0)
    {
      dp = (int)((event.GetY() - m_last_cursor_position->GetY()) / m_position_size.GetY());
      if (abs(dp) > 0)
      {
        MoveThumbByUnit(dp);
        wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, m_svguiid);
        m_window->ProcessEvent(evt);
        m_last_cursor_position->SetY(m_last_cursor_position->GetY() + m_position_size.GetY() * dp);
      }
    }
  }
  event.Skip();
}

void SVGUIScrollBar::OnLeftUp(wxMouseEvent &event)
{
  m_last_cursor_position = NULL;
  wxScrollEvent evt(wxEVT_SCROLL_THUMBRELEASE, m_svguiid);
  m_window->ProcessEvent(evt);
  event.Skip();
}
