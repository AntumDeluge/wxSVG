//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.cpp
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.cpp,v 1.3.2.1 2005-09-01 13:00:53 lbessard Exp $
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

wxSVGCtrl::wxSVGCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos,
 const wxSize& size, long style, const wxValidator& validator, const wxString& name):
  wxControl(parent, id, pos, size, style, validator, name)
{
  m_doc = NULL;
  m_docDelete = false;
}

void wxSVGCtrl::SetSVG(wxSVGDocument* doc)
{
  if (m_doc && m_docDelete)
    delete m_doc;
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

void wxSVGCtrl::Update()
{
  if (!m_doc)
    return;
  if (m_FitToFrame){
	  int w, h;
	  GetClientSize(&w, &h);
	  m_buffer = wxBitmap(m_doc->Render(w, h));
  } else {
	  m_buffer = wxBitmap(m_doc->Render(-1, -1));
  }
  Refresh();
}

void wxSVGCtrl::UpdateArea(wxSVGRect area, float scale)
{
  if (!m_doc)
    return;
  wxBitmap temp;
  if (m_FitToFrame){
	  int w, h;
	  GetClientSize(&w, &h);
	  temp = wxBitmap(m_doc->Render(w, h, &area));
  } else {
	  temp = wxBitmap(m_doc->Render(-1, -1, &area));
  }
  wxMemoryDC temp_dc;
  temp_dc.SelectObject(temp);
  wxMemoryDC m_buffer_dc;
  m_buffer_dc.SelectObject(m_buffer);
  wxPoint point((int)(area.GetX() * scale), (int)(area.GetY() * scale));
  wxSize size((int)((area.GetX() + area.GetWidth()) * scale) - point.x + 2, 
  	(int)((area.GetY() + area.GetHeight()) * scale) - point.y + 2);
  m_buffer_dc.Blit(point, size, &temp_dc, point);
  Refresh();
}

void wxSVGCtrl::OnPaint(wxPaintEvent& event)
{
  if (!m_buffer.Ok())
    return;
  wxPaintDC dc(this);
  dc.DrawBitmap(m_buffer, 0, 0);
}
