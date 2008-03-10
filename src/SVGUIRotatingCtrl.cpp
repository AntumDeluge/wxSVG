//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUIRotatingCtrl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: SVGUIRotatingCtrl.cpp,v 1.2 2008-03-10 17:15:28 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUIRotatingCtrl.h"
#include "SVGUIWindow.h"
#include <wxSVG/SVGCoordinates.h>
#include <sys/time.h>

const double pi = 3.1415926;

SVGUIRotatingCtrl::SVGUIRotatingCtrl(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
	m_initialised = false;
	m_min_angle = 0;
	m_max_angle = 360;
	m_angle = 0;
	m_x_center = -1;
	m_y_center = -1;
	m_center = NULL;
	m_init_pos = NULL;
	m_last_cursor_position = NULL;
	m_RotatingElement = NULL;
	SetName(wxT("RotatingCtrl"));
}

void SVGUIRotatingCtrl::DefineLimits(double min_angle, double max_angle)
{
	m_min_angle = min_angle;
	m_max_angle = max_angle;
	if (m_angle < min_angle)
	{
		m_angle = min_angle;
		Update_Elements();
		Refresh();
	}
	if (m_angle > max_angle)
	{
		m_angle > max_angle;
		Update_Elements();
		Refresh();
	}
}

void SVGUIRotatingCtrl::SetAngle(double angle)
{
	m_angle = angle;
	if (m_min_angle != 0 && m_max_angle != 360)
	{
		if (m_angle < m_min_angle)
			m_angle = m_min_angle;
		if (m_angle > m_max_angle)
			m_angle = m_max_angle;
	}
	else
	{
		while (m_angle >= 360)
		{
			m_angle -= 360;
		}
		while (m_angle <= 0)
		{
			m_angle += 360;
		}
	}
	Update_Elements();
	Refresh();
}

void SVGUIRotatingCtrl::DefineCenter(wxSVGPoint* center)
{
	m_center = center;
	Update_Elements();
	Refresh();
}

wxSVGPoint* SVGUIRotatingCtrl::GetCenter()
{
	return m_center;
}

void SVGUIRotatingCtrl::Rotate(double angle)
{
	m_angle += angle;
	if (m_min_angle != 0 || m_max_angle != 360)
	{
		if (m_angle < m_min_angle)
			m_angle = m_min_angle;
		if (m_angle > m_max_angle)
			m_angle = m_max_angle;
	}
	else
	{
		while (m_angle >= 360)
		{
			m_angle -= 360;
		}
		while (m_angle <= 0)
		{
			m_angle += 360;
		}
	}
	
	Update_Elements();
	Refresh();
}

bool SVGUIRotatingCtrl::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (attrName == wxT("background_id"))
  	m_BackgroundElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("rotating_id"))
  	m_RotatingElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("min_angle"))
  	attrValue.ToDouble(&m_min_angle);
  else if (attrName == wxT("max_angle"))
  	attrValue.ToDouble(&m_max_angle);
  else if (attrName == wxT("x_center"))
  	attrValue.ToDouble(&m_x_center);
  else if (attrName == wxT("y_center"))
  	attrValue.ToDouble(&m_y_center);
  else 
    return false;
  return true;
}

bool SVGUIRotatingCtrl::HitTest(wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	bool res=false;
	if (m_BackgroundElement)
		res |= m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	if (m_RotatingElement)
		res |= m_doc->GetRootElement()->CheckIntersection(*m_RotatingElement, rect);
	return res;
}

#define UpdateBBox_macro(element)\
		if (element)\
		{\
		  if (empty)\
	  	  {\
			res = wxSVGLocatable::GetElementBBox(element);\
			empty = false;\
	  	  }\
	  	  else\
	  	  	res = SumBBox(res, wxSVGLocatable::GetElementBBox(element));\
		}

wxSVGRect SVGUIRotatingCtrl::GetBBox()
{
	wxSVGRect res;
	bool empty = true;
	UpdateBBox_macro(m_BackgroundElement)
	UpdateBBox_macro(m_RotatingElement)
	return res;
}

void SVGUIRotatingCtrl::Update_Elements()
{
	if (!m_initialised)
		Initialize();
	else if (m_init_pos)
	{
		if (m_BackgroundElement)
		{
			wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
			if ((bbox.GetX() != m_init_pos->GetX()) ||(bbox.GetY() != m_init_pos->GetY()))
				Initialize();
		}
		else if (m_RotatingElement)
		{
			wxSVGRect bbox = wxSVGLocatable::GetElementBBox(m_RotatingElement);
			if ((bbox.GetX() + bbox.GetWidth() / 2 != m_init_pos->GetX()) ||(bbox.GetX() + bbox.GetHeight() / 2 != m_init_pos->GetY()))
				Initialize();
		}
	}
	if (m_RotatingElement)
	{
		ResetElementMatrix(m_RotatingElement);
		RotateElement(m_RotatingElement, m_angle, m_center);
	}
}

void SVGUIRotatingCtrl::Initialize()
{
	if (m_x_center != -1 && m_y_center != -1)
		m_center = new wxSVGPoint(m_x_center, m_y_center);
	else if (m_BackgroundElement)
	{
		wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
		m_center = new wxSVGPoint(background_bbox.GetX() + background_bbox.GetWidth() / 2, background_bbox.GetY() + background_bbox.GetHeight() / 2);
		m_init_pos = new wxSVGPoint(background_bbox.GetX(),background_bbox.GetY());
	}
	else if (m_RotatingElement)
	{
		wxSVGRect rotating_bbox = wxSVGLocatable::GetElementBBox(m_RotatingElement);
		m_center = new wxSVGPoint(rotating_bbox.GetX() + rotating_bbox.GetWidth() / 2, rotating_bbox.GetY() + rotating_bbox.GetHeight() / 2);
		m_init_pos = m_center;
	}
	if (m_RotatingElement){
		wxSVGRect rotating_bbox = wxSVGLocatable::GetElementBBox(m_RotatingElement);
		InitElementMatrix(m_RotatingElement);
	}
	m_initialised = true;
}

void SVGUIRotatingCtrl::OnLeftDown(wxMouseEvent &event)
{
	m_last_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
	event.Skip();
}

void SVGUIRotatingCtrl::OnMotion(wxMouseEvent &event)
{
	if (m_last_cursor_position)
	{
		wxSVGPoint* new_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
		wxSVGPoint last_vector = wxSVGPoint(m_last_cursor_position->GetX() - m_center->GetX(), m_last_cursor_position->GetY() - m_center->GetY());
		wxSVGPoint new_vector = wxSVGPoint(new_cursor_position->GetX() - m_center->GetX(), new_cursor_position->GetY() - m_center->GetY());
		double last_vector_length = sqrt(last_vector.GetX() * last_vector.GetX() + last_vector.GetY() * last_vector.GetY());
		double new_vector_length = sqrt(new_vector.GetX() * new_vector.GetX() + new_vector.GetY() * new_vector.GetY());
		double scalar_product = last_vector.GetX() * new_vector.GetX() + last_vector.GetY() * new_vector.GetY();
		double vectorial_product = last_vector.GetX() * new_vector.GetY() - last_vector.GetY() * new_vector.GetX();
		double cosinus = scalar_product / (last_vector_length * new_vector_length);
		if (cosinus > 1)
			cosinus = 1;
		if (cosinus < -1)
			cosinus = -1;
		double angle = acos(cosinus) / pi * 180;
		if (vectorial_product > 0)
		{
			angle = -angle;
		}
		Rotate(angle);
		wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, SVGUIWindow::GetSVGUIID(GetName()));
		m_window->ProcessEvent(evt);
		m_last_cursor_position = new_cursor_position;
		
		wxSVGRect rect = m_doc->GetRootElement()->GetBBox();
		Refresh();
		//wxSVGRect rect = GetBBox();
		
	}
	Update_Elements();
	
	
	event.Skip();
}

void SVGUIRotatingCtrl::OnLeftUp(wxMouseEvent &event)
{
	m_last_cursor_position = NULL;
	wxScrollEvent evt(wxEVT_SCROLL_THUMBRELEASE, SVGUIWindow::GetSVGUIID(GetName()));
	m_window->ProcessEvent(evt);
	event.Skip();
}
