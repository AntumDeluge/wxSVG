//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITransform.cpp
// Purpose:     
// Author:      Jonathan Hurtrel
// Created:     2007/08/13
// RCS-ID:      $Id: SVGUITransform.cpp,v 1.2 2008-03-10 17:15:28 etisserant Exp $
// Copyright:   (c) Jonathan Hurtrel
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUITransform.h"
#include "SVGUIWindow.h"
#include <wxSVG/SVGCoordinates.h>


SVGUITransform::SVGUITransform(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
	m_initialised = false;
	m_angle = 0;
	m_x_pos = 0;
	m_y_pos = 0;
	m_x_scale = 1;
	m_y_scale = 1;
	m_pos = NULL;
	m_init_pos = NULL;
	m_last_cursor_position = NULL;
	m_MovingElement = NULL;
	SetName(wxT("Transform"));
}

void SVGUITransform::Initialize()
{
	//MoveElement(m_MovingElement, 50, 50);
	if (m_BackgroundElement && m_MovingElement)
	{
		wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
		wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
		if (!m_init_pos)
			m_init_pos = new wxSVGPoint();
		m_init_pos->SetX(background_bbox.GetX());
		m_init_pos->SetY(background_bbox.GetY());
		m_x_pos = background_bbox.GetX()+(background_bbox.GetWidth()/2)-(moving_bbox.GetWidth()/2);
		m_y_pos = background_bbox.GetY()+(background_bbox.GetHeight()/2)-(moving_bbox.GetHeight()/2);
	}
	else if (m_MovingElement)
	{
		wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
		m_x_pos = moving_bbox.GetX();
		m_y_pos = moving_bbox.GetY();
	}
	//Scale(0.4,0.4);
	m_initialised = true;
}

#define UpdateBBox_macro(element)\
		if (element)\
		{\
		  if (res.IsEmpty())\
			res = wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT);\
	  	  else\
	  	  	res = SumBBox(res, wxSVGLocatable::GetElementResultBBox(element, wxSVG_COORDINATES_VIEWPORT));\
		}

wxSVGRect SVGUITransform::GetBBox()
{
	wxSVGRect res;
	bool empty = true;
	UpdateBBox_macro(m_MovingElement)
	UpdateBBox_macro(m_BackgroundElement)
	return res;
}

void SVGUITransform::Update_Elements()
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
	}
	if (m_MovingElement)
	{
		wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
		wxSVGPoint *rotat_center = new wxSVGPoint(moving_bbox.GetX()+moving_bbox.GetWidth()/2,moving_bbox.GetY()+moving_bbox.GetHeight()/2); 
		ResetElementMatrix(m_MovingElement);
		MoveElement(m_MovingElement, m_x_pos, m_y_pos);
		ScaleElement(m_MovingElement, m_x_scale, m_y_scale);
		RotateElement(m_MovingElement, m_angle, m_pos);
		
		
	}
}

void SVGUITransform::Move(double x, double y)
{
	m_x_pos = x;
	m_y_pos = y;
	if (!m_pos)
		m_pos = new wxSVGPoint();
	m_pos->SetX(x);
	m_pos->SetY(y);
	Update_Elements();
	Refresh();
}

void SVGUITransform::Scale(double x, double y)
{
	m_x_scale = x;
	m_y_scale = y;
	Update_Elements();
	Refresh();
}

void SVGUITransform::Rotate(double angle)
{
	m_angle = angle;
	Update_Elements();
	Refresh();
}

bool SVGUITransform::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (attrName == wxT("background_id"))
  	m_BackgroundElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("moving_id"))
  	m_MovingElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else 
    return false;
  return true;
}

void SVGUITransform::OnLeftDown(wxMouseEvent &event)
{
	m_last_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
	event.Skip();
}

void SVGUITransform::OnMotion(wxMouseEvent &event)
{
	if (m_last_cursor_position && m_MovingElement)
	{
		wxSVGPoint* new_cursor_position = new wxSVGPoint(event.GetX(), event.GetY());
		wxSVGRect moving_bbox = wxSVGLocatable::GetElementBBox(m_MovingElement);
		double move_x = new_cursor_position->GetX() + (moving_bbox.GetX() - m_last_cursor_position->GetX());
		double move_y = new_cursor_position->GetY() + (moving_bbox.GetY() - m_last_cursor_position->GetY());
		if (m_BackgroundElement)
		{
			wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement);
			if (move_x < background_bbox.GetX() || moving_bbox.GetX() < background_bbox.GetX())
			{
				move_x = background_bbox.GetX();
				//new_cursor_position->SetX(move_x);
			}
			else if (move_x + moving_bbox.GetWidth() > (background_bbox.GetX()+background_bbox.GetWidth())  || moving_bbox.GetX() + moving_bbox.GetWidth() > (background_bbox.GetX()+background_bbox.GetWidth()) )
			{				
				move_x = background_bbox.GetX()+background_bbox.GetWidth() - moving_bbox.GetWidth();
				//new_cursor_position->SetX(move_x);
			}
			if (move_y < background_bbox.GetY() || moving_bbox.GetY() < background_bbox.GetY())
			{				
				move_y = background_bbox.GetY();
				//new_cursor_position->SetY(move_y);
			}
			else if (move_y + moving_bbox.GetHeight() > (background_bbox.GetY()+background_bbox.GetHeight()) || moving_bbox.GetY() + moving_bbox.GetHeight() > (background_bbox.GetY()+background_bbox.GetHeight()) )
			{
				move_y = background_bbox.GetY()+background_bbox.GetHeight()-moving_bbox.GetHeight();
				//new_cursor_position->SetY(move_y);
			}
			
		}
		Move(move_x,move_y);
		Scale(0.4,0.4);
		//Rotate(90);
		wxScrollEvent evt(wxEVT_SCROLL_THUMBTRACK, SVGUIWindow::GetSVGUIID(GetName()));
		m_window->ProcessEvent(evt);
		m_last_cursor_position = new_cursor_position;
	}
	event.Skip();
}

void SVGUITransform::OnLeftUp(wxMouseEvent &event)
{
	m_last_cursor_position = NULL;
	wxScrollEvent evt(wxEVT_SCROLL_THUMBRELEASE, SVGUIWindow::GetSVGUIID(GetName()));
	m_window->ProcessEvent(evt);
	event.Skip();
}
