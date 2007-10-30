//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITextCtrl.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/29
// RCS-ID:      $Id: SVGUITextCtrl.h,v 1.2 2007-10-30 21:59:22 etisserant Exp $
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
		bool m_selected;
		bool m_fixed_size;
		bool m_show_background;
		long m_style;
		float m_offset_ctm_x;
		float m_offset_ctm_y;
		wxSVGPoint m_offset;
		wxSVGRect m_last_bbox;
		unsigned int m_cursor_position;
		wxSVGElement* m_TextElement;
		wxSVGElement* m_CursorElement;
		
		unsigned int RecursiveCursorSearch(wxSVGPoint& point, unsigned int start, unsigned int end);
		unsigned int AjustSize(unsigned int start, unsigned int end, wxSVGRect back, wxSVGRect text);
		
	public:
		SVGUITextCtrl(wxSVGDocument* doc, wxEvtHandler* window);
		void Initialize();
		
		bool HitTest(const wxPoint pt);
		wxSVGRect GetBBox();
		bool SetAttribute(const wxString& attrName, const wxString& attrValue);
		wxString GetValue();
		void SetText(const wxString& value);
		void SetValue(const wxString& value);
		void SetStyle(long style) {m_style = style;}
		long GetStyle() {return m_style;}
		void SetSelected(const bool selected);
		bool IsSelected(){return m_selected;}
		
		void AddCharInText(const wxString& character);
		void DelCharInText(const bool backward=false);
		void MoveCursor(const int direction);
		
		void FindCursorIndex(wxSVGPoint& point);
		void Update_Elements();
		void Update_Cursor();
		
		void OnLeftDown(wxMouseEvent &event);
		void OnMotion(wxMouseEvent &event);
		void OnLeftUp(wxMouseEvent &event);
		void OnChar(wxKeyEvent &event);
};

#endif //SVGUI_TEXTCTRL_H
