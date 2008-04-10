//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIControl.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIControl.h,v 1.5 2008-04-10 17:37:19 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_CONTROL_H
#define SVGUI_CONTROL_H

#include <wx/wx.h>
#include <wx/event.h>
#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include "SVGUIElement.h"


wxSVGRect SumBBox(wxSVGRect bbox1, wxSVGRect bbox2);

class SVGUIControl:
  public SVGUIElement
{
  protected:
    wxEvtHandler* m_window;
    
  public:
    SVGUIControl();
    SVGUIControl(wxSVGDocument* doc, wxEvtHandler* window);
    
    void Refresh();
    
    virtual bool HitTest(wxPoint pt);
    virtual wxSVGRect GetBBox();
    virtual void Update_Elements() = 0;

    virtual void OnLeftDown(wxMouseEvent& event);
    virtual void OnLeftUp(wxMouseEvent& event);
    virtual void OnMotion(wxMouseEvent& event);
    virtual void OnChar(wxKeyEvent& event);
};

#endif //SVGUI_CONTROL_H
