//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITransform.h
// Purpose:     
// Author:      Jonathan Hurtrel
// Created:     2007/08/13
// RCS-ID:      $Id: SVGUITransform.h,v 1.1 2007-08-31 08:56:17 gusstdie Exp $
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

#ifdef WXMAKINGDLL_WXSVG
    #define WXDLLIMPEXP_WXSVG WXEXPORT
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_WXSVG WXIMPORT
#else // not making nor using DLL
    #define WXDLLIMPEXP_WXSVG
#endif

class WXDLLIMPEXP_WXSVG SVGUITransform:
	public SVGUIControl
{
	protected:
		bool m_initialised;
		double m_angle;
		double m_x_pos;
		double m_y_pos;
		wxSVGPoint* m_pos;
		double m_x_scale;
		double m_y_scale;
		double m_init_pos_x;
		double m_init_pos_y;
		wxSVGPoint* m_last_cursor_position;
		wxSVGPoint* m_init_pos;
				
		wxSVGElement* m_MovingElement;
		
	public:
		SVGUITransform(wxSVGDocument* doc, wxEvtHandler* window);
		void Initialize();
		double GetX(){return m_x_pos;}
		double GetY(){return m_y_pos;}
		double GetXScale(){return m_y_scale;}
		double GetYScale(){return m_y_scale;}
		double GetAngle(){return m_angle;}
		void Move(double x, double y);
		void Scale(double x, double y);
		void Rotate(double angle);
		
		bool SetAttribute(const wxString& attrName, const wxString& attrValue);
		
		void OnLeftDown(wxMouseEvent &event);
		void OnMotion(wxMouseEvent &event);
		void OnLeftUp(wxMouseEvent &event);
		
		wxSVGRect GetBBox();
		void Update_Elements();
};

#endif //SVGUI_TRANSFORM_H
