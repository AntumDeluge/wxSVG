//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIElement.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/18
// RCS-ID:      $Id: SVGUIElement.h,v 1.10 2008-07-02 14:18:35 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_ELEMENT_H
#define SVGUI_ELEMENT_H

#include <wx/wx.h>
#include <wxSVGXML/svgxml.h>
#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "ElementTransform.h"

void SetDisplay(wxSVGElement* elem, wxCSS_VALUE value);

class SVGUIElement:
  public wxSvgXmlElement
{
  protected:
    wxSVGDocument* m_doc;
    wxSVGElement* m_BackgroundElement;
    bool m_enable;
    bool m_visible;
    bool m_selected;
    int m_svguiid;
    
  public:
    SVGUIElement(){};
    SVGUIElement(wxSVGDocument* doc);
    ~SVGUIElement();
    wxString GetId();
    wxString GetName();
    
    virtual void Initialize(){Update_Elements();}
    virtual bool HitTest(wxPoint pt) = 0;
    virtual wxSVGRect GetBBox() = 0;
    virtual void Update_Elements() = 0;
    virtual void SetSelected(bool selected) {m_selected = selected;}
    virtual bool IsSelected(){return m_selected;}
    bool IsVisible(){return m_visible;}
    bool IsEnabled(){return m_enable;}
    
    void Hide();
    void Show();
    void Enable(){m_enable = true;}
    void Disable(){m_enable = false;}
    
    virtual void AddProperty(const wxString& name, const wxString& value);
    virtual bool SetAttribute(const wxString& attrName, const wxString& attrValue); 

    virtual void OnLeftDown(wxMouseEvent& event) = 0;   
    virtual void OnLeftUp(wxMouseEvent& event) = 0;
    virtual void OnMotion(wxMouseEvent& event) = 0;
    virtual void OnChar(wxKeyEvent& event) = 0;
};

#endif //SVGUI_ELEMENT_H
