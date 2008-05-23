//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIContainer.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/05
// RCS-ID:      $Id: SVGUIContainer.h,v 1.6 2008-05-23 13:47:22 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_CONTAINER_H
#define SVGUI_CONTAINER_H

#include <wx/wx.h>
#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "SVGUIElement.h"
#include "SVGUITextCtrl.h"

class SVGUIContainer:
  public SVGUIElement
{
  protected:
    SVGUIElement* m_FocusedElement;
    
  public:
    SVGUIContainer(){};
    SVGUIContainer(wxSVGDocument* doc);
    wxString GetFocusedElement();
    wxString GetFocusedElementName();
    virtual void SetSelected(bool selected);
    virtual void Update_Elements();
    
    void Hide();
    void Show();
    void Enable();
    void Disable();
    
    virtual bool HitTest(wxPoint pt);
    virtual wxSVGRect GetBBox();

    virtual void OnLeftDown(wxMouseEvent& event);   
    virtual void OnLeftUp(wxMouseEvent& event);
    virtual void OnMotion(wxMouseEvent& event);
    virtual void OnChar(wxKeyEvent& event);
};

#endif //SVGUI_CONTAINER_H
