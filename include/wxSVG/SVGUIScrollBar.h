//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIScrollBar.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/18
// RCS-ID:      $Id: SVGUIScrollBar.h,v 1.3 2008-04-10 17:37:19 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_SCROLLBAR_H
#define SVGUI_SCROLLBAR_H

#include "SVGUIControl.h"
#include <math.h>
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

class SVGUIScrollBar:
  public SVGUIControl
{
  protected:
    bool m_initialised;
    wxSVGPoint m_direction;
    wxSVGPoint m_position_size;
    wxSVGPoint m_offset;
    wxSVGPoint* m_last_cursor_position;
    unsigned int m_position;
    unsigned int m_thumb;
    unsigned int m_range;
    wxSVGElement* m_ThumbBackElement;
    wxSVGElement* m_ThumbMiddleElement;
    wxSVGElement* m_UpArrowElement;
    wxSVGElement* m_DownArrowElement;
  
  public:
    SVGUIScrollBar(wxSVGDocument* doc, wxEvtHandler* window);
    void Init_ScrollBar(unsigned int position, unsigned int thumb, unsigned int range);
    void Initialize();
    unsigned int GetThumbPosition(){return m_position;}
    unsigned int GetThumbSize(){return m_thumb;}
    unsigned int GetRange(){return m_range;}
    
    void MoveThumbByUnit(int direction);
    void MoveThumbByPage(int direction);
    void SetThumbPosition(int position);
    
    bool HitTest(wxPoint pt);
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& attrName, const wxString& attrValue);
    void Update_Elements();
    
    void OnLeftDown(wxMouseEvent &event);
    void OnMotion(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
};

#endif //SVGUI_SCROLLBAR_H
