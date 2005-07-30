//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.h
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.h,v 1.3 2005-07-30 12:09:05 etisserant Exp $
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
      
	void SetFitToFrame(bool fit) { m_FitToFrame = fit; }

    void SetSVG(wxSVGDocument* doc);
    wxSVGDocument* GetSVG() { return m_doc; }
    
    bool Load(const wxString& filename);
    void Update();
    
  protected:
    wxSVGDocument* m_doc;
    bool m_docDelete;
    wxBitmap m_buffer;
	bool m_FitToFrame;


    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event) { Update(); }
    void OnEraseBackground(wxEraseEvent &event) {}
  
  private:
	DECLARE_EVENT_TABLE()
};

#endif // wxSVG_CTRL_H
