//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIRotatingCtrl.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/08/18
// RCS-ID:      $Id: SVGUIRotatingCtrl.h,v 1.2 2007-10-30 21:59:22 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef SVGUI_ROTATINGCTRL_H
#define SVGUI_ROTATINGCTRL_H

#include "SVGUIControl.h"
#include <wxSVG/SVGTransformable.h>
#include <math.h>
#include <wxSVG/svg.h>
#include <wx/string.h>
#include <wxSVG/SVGElement.h>
#include <wxSVG/SVGLocatable.h>
#include <wxSVG/CSSValue.h>
#include <wx/wx.h>

class SVGUIRotatingCtrl:
	public SVGUIControl
{
	protected:
		bool m_initialised;
		wxSVGPoint* m_center;
		wxSVGPoint* m_last_cursor_position;
		wxSVGPoint* m_init_pos;
		double m_angle;
		double m_x_center;
		double m_y_center;
		double m_min_angle;
		double m_max_angle;
		wxSVGElement* m_RotatingElement;
	
	public:
		SVGUIRotatingCtrl(wxSVGDocument* doc, wxEvtHandler* window);
		void Initialize();
		
		double GetAngle(){return m_angle;}
		double GetMinAngle(){return m_min_angle;}
		double GetMaxAngle(){return m_max_angle;}
		void SetAngle(double angle);
		void DefineCenter(wxSVGPoint* center);
		wxSVGPoint* GetCenter();
		void DefineLimits(double min_angle, double max_angle);
		void Rotate(double angle);
		
		bool HitTest(wxPoint pt);
		wxSVGRect GetBBox();
		bool SetAttribute(const wxString& attrName, const wxString& attrValue);
		void Update_Elements();
		
		void OnLeftDown(wxMouseEvent &event);
		void OnMotion(wxMouseEvent &event);
		void OnLeftUp(wxMouseEvent &event);
};

#endif //SVGUI_ROTATINGCTRL_H
