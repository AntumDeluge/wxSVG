//////////////////////////////////////////////////////////////////////////////
// Name:        svgview.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     15/01/2005
// RCS-ID:      $Id: svgview.cpp,v 1.2 2005-05-25 12:25:03 ntalex Exp $
// Copyright:   (c) Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma implementation
#pragma interface
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#ifndef __WXMSW__
#include <locale.h>
#endif

#include "svgview.h"

//////////////////////////////////////////////////////////////////////////////
///////////////////////////////  Application /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(SVGViewApp)

bool SVGViewApp::OnInit()
{
#ifndef __WXMSW__
  setlocale(LC_NUMERIC, "C");
#endif
  //wxLog::SetActiveTarget(new wxLogStderr);		
  
  new MainFrame(NULL, wxT("SVG Viewer"),
	wxDefaultPosition, wxSize(500, 400));
  return true;
}

//////////////////////////////////////////////////////////////////////////////
////////////////////////////////  MainFrame //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
  EVT_MENU(wxID_EXIT, MainFrame::OnExit)
END_EVENT_TABLE()

MainFrame::MainFrame(wxWindow *parent, const wxString& title, const wxPoint& pos,
  const wxSize& size, long style): wxFrame(parent, wxID_ANY, title, pos, size, style)
{
	// Make a menubar
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, _T("&Open..."));
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, _T("&Close"));
    
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, _T("&File"));
    SetMenuBar(menuBar);

    m_svgCtrl = new wxSVGCtrl(this);
	if (wxTheApp->argc > 1)
	  m_svgCtrl->Load(wxTheApp->argv[1]);
	else
	  m_svgCtrl->Load(_T("tiger.svg"));
	Show(true);
}

void MainFrame::OnOpen(wxCommandEvent& event)
{
  wxString filename = wxFileSelector(_T("Choose a file to open"),
    _T(""), _T(""), _T(""), _T("SVG files (*.svg)|*.svg|All files (*.*)|*.*"));
  if (!filename.empty())
    m_svgCtrl->Load(filename);
}

void MainFrame::OnExit( wxCommandEvent& WXUNUSED(event) )
{
  Close(true);
}

