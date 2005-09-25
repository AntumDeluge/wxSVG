//////////////////////////////////////////////////////////////////////////////
// Name:        svgctrl.h
// Purpose:     svg control widget
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgctrl.h,v 1.4 2005-09-25 11:31:25 ntalex Exp $
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
      
	void SetFitToFrame(bool fit) { m_FitToFrame = fit; }
    double GetScale() const;

    void SetSVG(wxSVGDocument* doc);
    wxSVGDocument* GetSVG() { return m_doc; }
    /** clears SVGCtrl (deletes svg document) */
    void Clear();
    
    /** loads svg file */
    bool Load(const wxString& filename);
    /** renders svg and repaints window */
    void Refresh(bool eraseBackground = true, const wxRect* rect = NULL);
    /** Causes a SVGCtrl-changed-event to be generated and repaints window */
    void Update(const wxRect* rect = NULL) { SendChangedEvent(); Refresh(true, rect); }
    /** Redraws the contents of the given rectangle:
        only the area inside it will be repainted. */
    void UpdateRect(const wxRect& rect) { Update(&rect); }
    
  protected:
    wxSVGDocument* m_doc;
    bool m_docDelete;
    bool m_repaint;
    wxRect m_repaintRect;
    wxBitmap m_buffer;
	bool m_FitToFrame;
    
    void SendChangedEvent();

    void OnPaint(wxPaintEvent& event);
    void OnResize(wxSizeEvent& event) { Refresh(); }
    void OnEraseBackground(wxEraseEvent &event) {}
  
  private:
	DECLARE_EVENT_TABLE()
};

BEGIN_DECLARE_EVENT_TYPES()
  DECLARE_EVENT_TYPE(EVT_COMMAND_SVGCTRL_CHANGED, 3500)
END_DECLARE_EVENT_TYPES()
  
#define EVT_SVGCTRL_CHANGED(id, fn)\
 DECLARE_EVENT_TABLE_ENTRY(EVT_COMMAND_SVGCTRL_CHANGED, id, wxID_ANY,\
 (wxObjectEventFunction) (wxEventFunction) (wxCommandEventFunction)& fn, NULL),

#endif // wxSVG_CTRL_H
