//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIButton.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIButton.h,v 1.4 2008-04-10 17:37:19 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_BUTTON_H
#define SVGUI_BUTTON_H

#include "SVGUIControl.h"
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

class SVGUIButton:
  public SVGUIControl
{
  protected:
    bool m_up;
    bool m_state;
    bool m_toggle;
    wxSVGElement* m_SelectedElement;
    wxSVGElement* m_UnselectedElement;
  
  public:
    SVGUIButton(wxSVGDocument* doc, wxEvtHandler* window);
    
    bool HitTest(wxPoint pt);
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& attrName, const wxString& attrValue);
    void Update_Elements();
    
    void SetToggle(bool toggle);
    bool GetToggle();
    
    void OnLeftDown(wxMouseEvent &event);
    void OnMotion(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
};

#endif //SVGUI_BUTTON_H
