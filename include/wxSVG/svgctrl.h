//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.h
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.h,v 1.1.1.1 2005-05-10 17:51:20 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef wxSVG_CTRL_H
#define wxSVG_CTRL_H

#include "SVGDocument.h"
#include <wx/control.h>
#include <wx/bitmap.h>

class wxSVGCtrl: public wxControl
{
  public:
    wxSVGCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
      const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
      long style = 0, const wxValidator& validator = wxDefaultValidator,
      const wxString& name = wxPanelNameStr);
      
    void SetSVG(wxSVGDocument* doc);
    wxSVGDocument* GetSVG() { return m_doc; }
    
    bool Load(const wxString& filename);
    void Update();
    
  protected:
    wxSVGDocument* m_doc;
    bool m_docDelete;
    wxBitmap m_buffer;
    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event) { Update(); }
  
  private:
	DECLARE_EVENT_TABLE()
};

#endif // wxSVG_CTRL_H
