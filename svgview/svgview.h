//////////////////////////////////////////////////////////////////////////////
// Name:        test.h
// Purpose:     Widget to display svg documents using svg library
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: svgview.h,v 1.1.1.1 2005-05-10 17:51:05 ntalex Exp $
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

class MainFrame: public wxFrame
{
  public:
    MainFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
      const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

  protected:
    wxSVGCtrl* m_svgCtrl;
    void OnOpen(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif //SVGVIEW_H
