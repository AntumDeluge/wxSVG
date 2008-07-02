//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITextCtrl.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/29
// RCS-ID:      $Id: SVGUITextCtrl.h,v 1.6 2008-07-02 14:18:35 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_TEXTCTRL_H
#define SVGUI_TEXTCTRL_H

#include "SVGUIControl.h"
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

class SVGUITextCtrl:
  public SVGUIControl
{
  protected:
    bool m_initialised;
    bool m_fixed_size;
    long m_style;
    bool m_show_background;
    double m_background_scale;
    wxSVGPoint m_text_offset;
    wxSVGPoint m_text_size;
    wxSVGPoint m_cursor_offset;
    unsigned int m_cursor_position;
    wxSVGElement* m_TextElement;
    wxSVGElement* m_CursorElement;
    
    unsigned int RecursiveCursorSearch(wxSVGPoint& point, unsigned int start, unsigned int end);
    unsigned int AjustSize(unsigned int start, unsigned int end, wxSVGRect back, wxSVGRect text);
    void SetText(const wxString& value);
    
  public:
    SVGUITextCtrl(wxSVGDocument* doc, wxEvtHandler* window);
    ~SVGUITextCtrl();
    void Initialize();
    
    bool HitTest(const wxPoint pt);
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& attrName, const wxString& attrValue);
    wxString GetValue();
    void SetValue(const wxString& value);
    void SetStyle(long style) {m_style = style;}
    long GetStyle() {return m_style;}
    void SetSelected(bool selected);
    
    void AddCharInText(const wxString& character);
    void DelCharInText(const bool backward=false);
    void MoveCursor(const int direction);
    
    void FindCursorIndex(wxSVGPoint& point);
    void Update_Elements();
    
    void OnLeftDown(wxMouseEvent &event);
    void OnMotion(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
    void OnChar(wxKeyEvent &event);
};

#endif //SVGUI_TEXTCTRL_H
