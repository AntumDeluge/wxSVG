//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITextCtrl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/29
// RCS-ID:      $Id: SVGUITextCtrl.cpp,v 1.8 2008-05-23 13:47:53 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUITextCtrl.h"

wxString LocaleToUTF8(wxString str_utf8)
{
  wxCSConv conv(wxLocale::GetSystemEncodingName());
  return wxString(str_utf8.wc_str(conv),wxConvUTF8);
}

wxString UTF8ToLocale(wxString str_utf8)
{
  wxCSConv conv(wxLocale::GetSystemEncodingName());
  return wxString(str_utf8.wc_str(wxConvUTF8),conv);
}

SVGUITextCtrl::SVGUITextCtrl(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
  m_initialised = false;
  m_selected = false;
  m_style = 0;
  m_fixed_size = true;
  m_show_background = false;
  m_background_scale = 1.0;
  m_cursor_position = 0;
  m_TextElement = NULL;
  m_CursorElement = NULL;
  SetName(wxT("TextCtrl"));
}

bool SVGUITextCtrl::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("text_id"))
    m_TextElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("cursor_id"))
    m_CursorElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("show_background"))
    m_show_background = attrValue == wxT("true");
  else if (attrName == wxT("align"))
  {
    if (attrValue == wxT("left"))
      m_style |= wxALIGN_LEFT;
    else if (attrValue == wxT("right"))
      m_style |= wxALIGN_RIGHT;
    else if (attrValue == wxT("center"))
      m_style |= wxALIGN_CENTER;
  }
  else 
    return false;
  return true;
}

void SVGUITextCtrl::SetSelected(bool selected)
{
  if (selected != m_selected)
  {
    m_selected = selected;
    Update_Elements();
    Refresh();
  }
}

bool SVGUITextCtrl::HitTest(const wxPoint pt)
{
  if (!m_enable)
    return false;
  wxSVGRect rect(pt.x, pt.y, 1, 1);
  if (m_BackgroundElement)
    return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
  else if (m_TextElement)
    return m_doc->GetRootElement()->CheckIntersection(*m_TextElement, rect);
  return false;
}

wxSVGRect SVGUITextCtrl::GetBBox()
{
  wxSVGRect res(0, 0, 0, 0);
    
  if (m_BackgroundElement)
    res = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
  else if (m_TextElement)
    res = wxSVGLocatable::GetElementBBox(m_TextElement);
  return res;
}

void SVGUITextCtrl::Initialize()
{
  if (m_BackgroundElement && m_TextElement) {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    wxSVGRect text_bbox = wxSVGLocatable::GetElementBBox(m_TextElement);
    m_text_offset.SetX(text_bbox.GetX() - background_bbox.GetX());
    m_text_offset.SetY(text_bbox.GetY() - background_bbox.GetY());
  }
  m_initialised = true;
}

void SVGUITextCtrl::Update_Elements()
{
  if (!m_initialised)
    Initialize();
  if (m_selected) {
    SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_CursorElement, wxCSS_VALUE_INLINE);
  }
  else {
    if (!m_show_background)
      SetDisplay(m_BackgroundElement, wxCSS_VALUE_NONE);
    else
      SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
    SetDisplay(m_CursorElement, wxCSS_VALUE_NONE);
  }
  
  if (m_BackgroundElement && !m_fixed_size) {
    ResetElementMatrix(m_BackgroundElement);
    ScaleElement(m_BackgroundElement, m_background_scale, 1);
  }
  
  if (m_BackgroundElement && m_TextElement) {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    wxSVGRect text_bbox = wxSVGLocatable::GetElementBBox(m_TextElement);
    double Xposition = background_bbox.GetX();
    double Yposition = background_bbox.GetY() + (background_bbox.GetHeight() - text_bbox.GetHeight()) / 2;
    if (m_style && wxALIGN_RIGHT)
      Xposition += background_bbox.GetWidth() - m_text_offset.GetX() - text_bbox.GetWidth();
    else if (m_style && wxALIGN_CENTER)
      Xposition += (background_bbox.GetWidth() - text_bbox.GetWidth()) / 2;
    else
      Xposition += m_text_offset.GetX();
    
    MoveElement(m_TextElement, Xposition, Yposition);
    
    if (m_selected && m_CursorElement) {
      wxSVGRect cursor_bbox = wxSVGLocatable::GetElementBBox(m_CursorElement);
      wxString label = GetValue();
      unsigned int nb_chars = label.Length();
      
      if (m_cursor_position == 0)
        Xposition -= cursor_bbox.GetWidth();
      else if (m_cursor_position == nb_chars)
        Xposition += ((wxSVGTextElement*)m_TextElement)->GetComputedTextLength();
      else
      {
        wxSVGPoint end_char_before = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(m_cursor_position - 1);
        wxSVGPoint start_char_after = ((wxSVGTextElement*)m_TextElement)->GetStartPositionOfChar(m_cursor_position);
        Xposition = (end_char_before.GetX() + start_char_after.GetX() - cursor_bbox.GetWidth()) / 2; 
      }
      Yposition += (text_bbox.GetHeight() - cursor_bbox.GetHeight()) / 2;
      
      MoveElement(m_CursorElement, Xposition, Yposition);
    }
  }
}

wxString SVGUITextCtrl::GetValue()
{
  wxString res = wxT("");
  if (m_TextElement) {
    wxSVGElement* n = (wxSVGElement*)m_TextElement->GetChildren();
    if (n && n->GetType() == wxSVGXML_ELEMENT_NODE)
    {
      wxSVGElement* text = (wxSVGElement*)n->GetChildren();
      if (text && text->GetType() == wxSVGXML_TEXT_NODE)
         res = UTF8ToLocale(text->GetContent());
    }
  }
  return res;
}

unsigned int SVGUITextCtrl::AjustSize(unsigned int start, unsigned int end, wxSVGRect back, wxSVGRect text)
{
  if (end - start <= 1)
    return start;
  else
  {
    unsigned int middle = (start + end) / 2;
    double text_size = ((wxSVGTextElement*)m_TextElement)->GetSubStringLength(0, middle);
    if (text_size + 2 * m_text_offset.GetX() > back.GetWidth())
      return AjustSize(start, middle, back, text);
    else
      return AjustSize(middle, end, back, text);
  }
}

void SVGUITextCtrl::SetText(const wxString& value)
{
  if (m_TextElement)
  {
    ((wxSVGTextElement*)m_TextElement)->SetCanvasItem(NULL);
    wxSVGElement* n = (wxSVGElement*)m_TextElement->GetChildren();
    wxSVGElement* text = NULL;
    if (n && n->GetType() == wxSVGXML_ELEMENT_NODE)
    {
      text = (wxSVGElement*)n->GetChildren();
      if (text && text->GetType() == wxSVGXML_TEXT_NODE)
         text->SetContent(LocaleToUTF8(value));
    }
    if (m_cursor_position > value.Length())
      m_cursor_position = value.Length();
  }
}

void SVGUITextCtrl::SetValue(const wxString& value)
{
  SetText(value);
  if (m_BackgroundElement)
  {
    wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
    wxSVGRect text_bbox = wxSVGLocatable::GetElementBBox(m_TextElement);
    if (m_fixed_size) {
      if (text_bbox.GetWidth() + 2 * m_text_offset.GetX() > background_bbox.GetWidth())
        SetText(value.Left(AjustSize(0, value.Length(), background_bbox, text_bbox)));
    }
    else
      m_background_scale *= (m_text_offset.GetX() * 2 + text_bbox.GetWidth()) / background_bbox.GetWidth();
  }
  Update_Elements();
  Refresh();
}

void SVGUITextCtrl::AddCharInText(const wxString& character)
{
  wxString label = GetValue();
  wxString before_cursor = label.Left(m_cursor_position);
  wxString after_cursor = label.Right(label.Length() - m_cursor_position);
  wxString new_label = before_cursor + character + after_cursor;
  m_cursor_position += 1;
  SetValue(new_label);
  Update_Elements();
  Refresh();
}

void SVGUITextCtrl::DelCharInText(const bool backward)
{
  wxString label = GetValue();
  if (label != wxT(""))
  {
    if (backward)
    {
      if (m_cursor_position > 0)
      {
        wxString before_cursor = label.Left(m_cursor_position - 1);
        wxString after_cursor = label.Right(label.Length() - m_cursor_position);
        label = before_cursor + after_cursor;
        if (m_cursor_position > 0)
          m_cursor_position -= 1;
      }
    }
    else
    {
      if (m_cursor_position < label.Length())
      {
        wxString before_cursor = label.Left(m_cursor_position);
        wxString after_cursor = label.Right(label.Length() - m_cursor_position - 1);
        label = before_cursor + after_cursor;
      }
    }
    SetValue(label);
  }
  Update_Elements();
  Refresh();
}

void SVGUITextCtrl::MoveCursor(const int direction)
{
  wxString label = GetValue();
  if (m_cursor_position + direction >= 0 && m_cursor_position + direction <= label.Length())
  {
    m_cursor_position += direction;
    Update_Elements();
    Refresh();
  }
}

unsigned int SVGUITextCtrl::RecursiveCursorSearch(wxSVGPoint& point, unsigned int start, unsigned int end)
{
  if (start == end)
  {
    wxSVGRect char_bbox = ((wxSVGTextElement*)m_TextElement)->GetExtentOfChar(start);
    double middle_position = char_bbox.GetX() + char_bbox.GetWidth() / 2;
    if (point.GetX() <= middle_position)
      return start;
    else
      return start + 1;
  }
  else
  {
    int middle = (end + start) / 2;
    wxSVGPoint end_middle = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(middle);
    if (point.GetX() <= end_middle.GetX())
      return RecursiveCursorSearch(point, start, middle);
    else
      return RecursiveCursorSearch(point, middle + 1, end); 
  }
}

void SVGUITextCtrl::FindCursorIndex(wxSVGPoint& point)
{
  wxString label = GetValue();
  
  if (m_TextElement)
    if (label.Length() > 0)
      m_cursor_position = RecursiveCursorSearch(point, 0, label.Length()-1);
    else
      m_cursor_position = 0;
}

void SVGUITextCtrl::OnLeftDown(wxMouseEvent &event)
{
  wxSVGPoint point(event.GetX(), event.GetY());
  FindCursorIndex(point);
  Update_Elements();
  Refresh();
  event.Skip();
}

void SVGUITextCtrl::OnMotion(wxMouseEvent &event)
{
  event.Skip();
}

void SVGUITextCtrl::OnLeftUp(wxMouseEvent &event)
{
  event.Skip();
}

void SVGUITextCtrl::OnChar(wxKeyEvent &event)
{
  int keycode = event.GetKeyCode();
  if (keycode == WXK_LEFT)
    MoveCursor(-1);
  else if (keycode == WXK_RIGHT)
    MoveCursor(1);
  else if (keycode == WXK_DELETE)
    DelCharInText(false);
  else if (keycode == WXK_BACK)
    DelCharInText(true);
  else if (keycode < 0 || keycode >= 32 && keycode <= 255)
    AddCharInText(wxChar(keycode));
  event.Skip();
}
