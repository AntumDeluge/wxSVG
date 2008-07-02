//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITransform.h
// Purpose:     
// Author:      Jonathan Hurtrel
// Created:     2007/08/13
// RCS-ID:      $Id: SVGUITransform.h,v 1.5 2008-07-02 14:18:35 etisserant Exp $
// Copyright:   (c) Jonathan Hurtrel
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_TRANSFORM_H
#define SVGUI_TRANSFORM_H

#include "SVGUIControl.h"
#include <wxSVG/SVGTransformable.h>
#include <math.h>
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/SVGLocatable.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

class SVGUITransform:
  public SVGUIControl
{
  protected:
    bool m_initialised;
    double m_angle;
    double m_x_pos;
    double m_y_pos;
    double m_x_scale;
    double m_y_scale;
    wxSVGPoint* m_last_cursor_position;
    wxSVGRect* m_moving_zone;
    
    wxSVGElement* m_MovingElement;
    
  public:
    SVGUITransform(wxSVGDocument* doc, wxEvtHandler* window);
    ~SVGUITransform();
    void Initialize();
    
    bool HitTest(wxPoint pt);
    wxSVGRect GetBBox();
    bool SetAttribute(const wxString& attrName, const wxString& attrValue);
    void Update_Elements();
    
    double GetX(){return m_x_pos;}
    double GetY(){return m_y_pos;}
    double GetXScale(){return m_y_scale;}
    double GetYScale(){return m_y_scale;}
    double GetAngle(){return m_angle;}
    void Move(double x, double y);
    void Scale(double x, double y);
    void Rotate(double angle);
    
    void OnLeftDown(wxMouseEvent &event);
    void OnMotion(wxMouseEvent &event);
    void OnLeftUp(wxMouseEvent &event);
};

#endif //SVGUI_TRANSFORM_H
