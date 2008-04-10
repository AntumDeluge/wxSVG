//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUINoteBook.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/09/08
// RCS-ID:      $Id: SVGUINoteBook.h,v 1.3 2008-04-10 17:33:34 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_NOTEBOOK_H
#define SVGUI_NOTEBOOK_H

#include "SVGUIControl.h"
#include "SVGUINoteBookContent.h"
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>
#include <wx/notebook.h>

class SVGUINoteBook:
  public SVGUIControl
{
  protected:
    bool m_initialised;
    int m_current_page;
    SVGUINoteBookContent* m_current_content;
    wxSVGElement* m_GroupElement;
    wxSVGElement* m_ContentElement;
    
  public:
    SVGUINoteBook(wxSVGDocument* doc, wxEvtHandler* window);
    
    bool HitTest(wxPoint pt);
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& attrName, const wxString& attrValue);
    void Initialize();
    void Update_Elements();
    unsigned int GetCurrentPage(){return m_current_page;}
    void SetCurrentPage(int page);
    
    void OnLeftDown(wxMouseEvent &event);
    void OnMotion(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};

#endif //SVGUI_NOTEBOOK_H
