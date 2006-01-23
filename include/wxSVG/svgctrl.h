//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.h
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.h,v 1.8 2006-01-23 09:13:12 ntalex Exp $
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
    ~wxSVGCtrl();
      
    void SetFitToFrame(bool fit = true) { m_fitToFrame = fit; }
    double GetScale() const;
    wxSVGMatrix GetScreenCTM() const;

    void SetSVG(wxSVGDocument* doc);
    wxSVGDocument* GetSVG() { return m_doc; }
    /** clears SVGCtrl (deletes svg document) */
    void Clear();
    
    /** loads svg file */
    bool Load(const wxString& filename);
    /** renders svg and repaints window */
    void Refresh(bool eraseBackground = true, const wxRect* rect = NULL);
    void Refresh(const wxSVGRect* rect);
    /** Redraws the contents of the given rectangle:
        only the area inside it will be repainted. */
    void RefreshRect(const wxRect& rect) { Refresh(true, &rect); }
    void RefreshRect(const wxSVGRect& rect) { Refresh(&rect); }
    
  protected:
    wxSVGDocument* m_doc;
    bool m_docDelete;
    bool m_repaint;
    wxRect m_repaintRect;
    wxBitmap m_buffer;
    bool m_fitToFrame;
    
    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event) { Refresh(); }
    void OnEraseBackground(wxEraseEvent &event) {}
  
  private:
    DECLARE_EVENT_TABLE()
};

#endif // wxSVG_CTRL_H
