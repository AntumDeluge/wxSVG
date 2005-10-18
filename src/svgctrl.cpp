//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.cpp
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.cpp,v 1.6 2005-10-18 16:39:44 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "svgctrl.h"
#include <wx/wx.h>

BEGIN_EVENT_TABLE(wxSVGCtrl, wxControl)
  EVT_PAINT(wxSVGCtrl::OnPaint)
  EVT_SIZE(wxSVGCtrl::OnResize)
  EVT_ERASE_BACKGROUND(wxSVGCtrl::OnEraseBackground)
END_EVENT_TABLE()

DEFINE_EVENT_TYPE(EVT_COMMAND_SVGCTRL_CHANGED)

wxSVGCtrl::wxSVGCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
 const wxSize& size, long style, const wxValidator& validator, const wxString& name):
  wxControl(parent, id, pos, size, style, validator, name)
{
  m_doc = NULL;
  m_docDelete = false;
  m_repaint = false;
}

wxSVGCtrl::~wxSVGCtrl()
{
  Clear();
}

void wxSVGCtrl::Clear()
{
  if (m_doc && m_docDelete)
  {
    delete m_doc;
    m_doc = NULL;
    m_docDelete = false;
  }
}

void wxSVGCtrl::SetSVG(wxSVGDocument* doc)
{
  Clear();
  m_doc = doc;
}

bool wxSVGCtrl::Load(const wxString& filename)
{
  if (!m_doc)
  {
    m_doc = new wxSVGDocument;
    m_docDelete = true;
  }
  
  if (!m_doc->Load(filename))
    return false;
  
  Update();
  return true;
}

void wxSVGCtrl::Refresh(bool eraseBackground, const wxRect* rect)
{
  m_repaint = true;
  if (rect && !m_repaintRect.IsEmpty())
  {
    int x2 = wxMax(m_repaintRect.x+m_repaintRect.width, rect->x+rect->width);
    int y2 = wxMax(m_repaintRect.y+m_repaintRect.height, rect->y+rect->height);
    m_repaintRect.x = wxMin(m_repaintRect.x, rect->x);
    m_repaintRect.y = wxMin(m_repaintRect.y, rect->y);
    m_repaintRect.width = x2 - m_repaintRect.x;
    m_repaintRect.height = y2 - m_repaintRect.y;
  }
  else
    m_repaintRect = rect ? *rect : wxRect();
  
  wxControl::Refresh(false, rect);
}

void wxSVGCtrl::Refresh(const wxSVGRect* rect)
{
  if (!rect || rect->IsEmpty())
    return;
  wxRect winRect((int)(rect->GetX()*GetScale()), (int)(rect->GetY()*GetScale()),
    (int)(rect->GetWidth()*GetScale()), (int)(rect->GetHeight()*GetScale()));
  RefreshRect(winRect);
}

void wxSVGCtrl::SendChangedEvent()
{
  wxCommandEvent evt(EVT_COMMAND_SVGCTRL_CHANGED, this->GetId());
  GetEventHandler()->ProcessEvent(evt);
}

void wxSVGCtrl::OnPaint(wxPaintEvent& event)
{
  if (!m_doc)
    return;
  
  if (m_repaint)
  {
    int w = -1, h = -1;
    if (m_FitToFrame)
      GetClientSize(&w, &h);
    
    //wxDateTime time = wxDateTime::UNow();
    
    if (!m_repaintRect.IsEmpty() &&
        (m_repaintRect.width < 2*m_buffer.GetWidth()/3 ||
         m_repaintRect.height < 2*m_buffer.GetHeight()/3))
    {
      m_repaintRect.x = wxMax(m_repaintRect.x, 0);
      m_repaintRect.y = wxMax(m_repaintRect.y, 0);
      wxSVGRect rect(m_repaintRect.x/GetScale(), m_repaintRect.y/GetScale(),
       m_repaintRect.width/GetScale(), m_repaintRect.height/GetScale());
      wxBitmap bitmap = m_doc->Render(w, h, &rect);
      wxMemoryDC dc;
      dc.SelectObject(m_buffer);
      dc.DrawBitmap(bitmap, m_repaintRect.x, m_repaintRect.y);
    }
    else
      m_buffer = wxBitmap(m_doc->Render(w, h));
    
    m_repaintRect = wxRect();
    
    //wxLogError(wxDateTime::UNow().Subtract(time).Format(wxT("draw buffer %l ms")));
  }
  
  wxPaintDC dc(this);

#ifdef __WXMSW__
  int w = GetClientSize().GetWidth();
  int h = GetClientSize().GetHeight();
  dc.SetPen(wxPen(wxColour(), 0, wxTRANSPARENT));
  dc.DrawRectangle(m_buffer.GetWidth(), 0, w-m_buffer.GetWidth(), h);
  dc.DrawRectangle(0, m_buffer.GetHeight(), m_buffer.GetWidth(), h-m_buffer.GetHeight());
#endif
  
  dc.DrawBitmap(m_buffer, 0, 0);
}

double wxSVGCtrl::GetScale() const
{
  if (m_doc)
    return m_doc->GetScale();
  return 1;
}
