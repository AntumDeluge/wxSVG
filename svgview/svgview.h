//////////////////////////////////////////////////////////////////////////////
// Name:        test.h
// Purpose:     Widget to display svg documents using svg library
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgview.h,v 1.4 2005-12-24 19:08:40 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGVIEW_H
#define SVGVIEW_H

#include <wx/wx.h>
#include <wxSVG/svgctrl.h>

class SVGViewApp: public wxApp
{
  public:
    bool OnInit();
};

class MySVGCanvas: public wxSVGCtrl
{
public:
    MySVGCanvas(wxWindow* parent);
    void OnMouseLeftUp(wxMouseEvent & event);
	void SetShowHitPopup(bool);

private:
	bool m_ShowHitPopup;
    DECLARE_EVENT_TABLE()
};

class MainFrame: public wxFrame
{
  public:
    MainFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
      const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

  protected:
    MySVGCanvas* m_svgCtrl;
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void Fit(wxCommandEvent& event);
    void Hittest(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif //SVGVIEW_H
