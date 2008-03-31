//////////////////////////////////////////////////////////////////////////////
// Name:        SVGUITextCtrl.cpp
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/29
// RCS-ID:      $Id: SVGUITextCtrl.cpp,v 1.4 2008-03-31 16:54:41 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGUITextCtrl.h"

wxString LocaleToUTF8(wxString str_utf8)
{
	wxCSConv conv(wxLocale::GetSystemEncodingName());
	return wxString(str_utf8.wc_str(conv),wxConvUTF8);
}

wxString UTF8ToLocale(wxString str_utf8)
{
	wxCSConv conv(wxLocale::GetSystemEncodingName());
	return wxString(str_utf8.wc_str(wxConvUTF8),conv);
}

SVGUITextCtrl::SVGUITextCtrl(wxSVGDocument* doc, wxEvtHandler* window): SVGUIControl(doc, window)
{
	m_initialised = false;
	m_selected = false;
	m_fixed_size = true;
	m_show_background = false;
	m_style = 0;
	m_last_bbox = wxSVGRect(0,0,-1,-1);
	m_cursor_position = 0;
	m_TextElement = NULL;
	m_CursorElement = NULL;
	m_enable = true;
	SetName(wxT("TextCtrl"));
}

bool SVGUITextCtrl::SetAttribute(const wxString& attrName, const wxString& attrValue)
{
  if (SVGUIElement::SetAttribute(attrName, attrValue))
    return true;
  else if (attrName == wxT("text_id"))
  	m_TextElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("cursor_id"))
    m_CursorElement = (wxSVGElement*)m_doc->GetElementById(attrValue);
  else if (attrName == wxT("show_background"))
  {
  	if (attrValue == wxT("true"))
  		m_show_background = true;
  	else if (attrValue == wxT("false"))
  		m_show_background = false;
  }
  else if (attrName == wxT("align"))
  {
  	if (attrValue == wxT("left"))
  		m_style |= wxALIGN_LEFT;
  	else if (attrValue == wxT("right"))
  		m_style |= wxALIGN_RIGHT;
  	else if (attrValue == wxT("center"))
  		m_style |= wxALIGN_CENTER;
  }
  else 
    return false;
  return true;
}

void SVGUITextCtrl::SetSelected(const bool selected)
{
	if (selected != m_selected)
	{
		m_selected = selected;
		Update_Elements();
		wxCommandEvent refresh_evt(wxEVT_COMMAND_ENTER, -1);
		refresh_evt.SetString(GetId());
		m_window->ProcessEvent(refresh_evt);
	}
}

bool SVGUITextCtrl::HitTest(const wxPoint pt)
{
	if (!m_enable)
		return false;
	wxSVGRect rect(pt.x, pt.y, 1, 1);
	if (m_BackgroundElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_BackgroundElement, rect);
	else if (m_TextElement)
		return m_doc->GetRootElement()->CheckIntersection(*m_TextElement, rect);
    return false;
}

wxSVGRect SVGUITextCtrl::GetBBox()
{
	wxSVGRect res(0, 0, 0, 0);
    
	if (m_BackgroundElement)
		res = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
	else if (m_TextElement)
		res = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
	
    if (m_last_bbox.GetWidth() == -1 && m_last_bbox.GetHeight() == -1)
		m_last_bbox = res;
	else
		res = SumBBox(res, m_last_bbox);
    
	return res;
}

void SVGUITextCtrl::Update_Elements()
{
	if (m_selected)
	{
		SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
		SetDisplay(m_CursorElement, wxCSS_VALUE_INLINE);
	}
	else
	{
		if (!m_show_background)
			SetDisplay(m_BackgroundElement, wxCSS_VALUE_NONE);
		else
			SetDisplay(m_BackgroundElement, wxCSS_VALUE_INLINE);
		SetDisplay(m_CursorElement, wxCSS_VALUE_NONE);
	}
}

void SVGUITextCtrl::Initialize()
{
	if (m_BackgroundElement && m_TextElement)
	{
    wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
		wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
    m_offset.SetX(text_bbox.GetX() - background_bbox.GetX());
    m_offset.SetY(text_bbox.GetY() - background_bbox.GetY());
    InitElementMatrix(m_TextElement);
        
    if (m_style && wxALIGN_RIGHT)
		{
	  		double Xposition = background_bbox.GetX()+background_bbox.GetWidth() - m_offset.GetX() - text_bbox.GetWidth();
	  		Xposition = background_bbox.GetX();
	      double Yposition = background_bbox.GetY()+background_bbox.GetHeight()/2-text_bbox.GetHeight()/2;
	      MoveElement(m_TextElement, Xposition, Yposition);
	      wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
	      m_offset_ctm_x = background_bbox.GetX() - text_bbox.GetX()+4;
				m_offset_ctm_y = background_bbox.GetY() - text_bbox.GetY();
				MoveElement(m_TextElement, Xposition+text_bbox.GetWidth()/2, Yposition-text_bbox.GetHeight()/2);
				
		}
        
	}
	m_initialised = true;
}

wxString SVGUITextCtrl::GetValue()
{
	wxString res = wxT("");
	if (m_TextElement)
	{
    	wxSVGElement* n = (wxSVGElement*)m_TextElement->GetChildren();
    	if (n && n->GetType() == wxSVGXML_ELEMENT_NODE)
    	{
			wxSVGElement* text = (wxSVGElement*)n->GetChildren();
        	if (text && text->GetType() == wxSVGXML_TEXT_NODE)
           		res = UTF8ToLocale(text->GetContent());
    	}
	}
	return res;
}

unsigned int SVGUITextCtrl::AjustSize(unsigned int start, unsigned int end, wxSVGRect back, wxSVGRect text)
{
	if (end - start <= 1)
		return start;
	else
	{
		unsigned int middle = (start + end) / 2;
		double text_size = ((wxSVGTextElement*)m_TextElement)->GetSubStringLength(0, middle);
		if (text.GetX() + text_size > back.GetX() + back.GetWidth())
			return AjustSize(start, middle, back, text);
		else
			return AjustSize(middle, end, back, text);
	}
}

void SVGUITextCtrl::SetText(const wxString& value)
{
	if (!m_initialised)
		Initialize();
	if (m_TextElement)
	{
		((wxSVGTextElement*)m_TextElement)->SetCanvasItem(NULL);
    	wxSVGElement* n = (wxSVGElement*)m_TextElement->GetChildren();
    	wxSVGElement* text = NULL;
    	if (n && n->GetType() == wxSVGXML_ELEMENT_NODE)
    	{
			    text = (wxSVGElement*)n->GetChildren();
        	if (text && text->GetType() == wxSVGXML_TEXT_NODE)
           		text->SetContent(LocaleToUTF8(value));
    	}
    	if (m_cursor_position > value.Length())
    		m_cursor_position = value.Length();
    	if (m_style && wxALIGN_RIGHT)
    	{
    		if (m_BackgroundElement && m_TextElement)
    		{
            wxSVGRect background_bbox = wxSVGLocatable::GetElementBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
            wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
            m_offset.SetX(15);
	      		double Xposition = text_bbox.GetX()+background_bbox.GetWidth()-text_bbox.GetWidth()- m_offset.GetX() + m_offset_ctm_x;
	      		double Yposition = text_bbox.GetY() + m_offset_ctm_y+background_bbox.GetHeight()/2-text_bbox.GetHeight()/2;
            MoveElement(m_TextElement, Xposition, Yposition);
    		}
    	}
    	if (!m_fixed_size && m_BackgroundElement)
    	{
            wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
            wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
            m_last_bbox = background_bbox;
            double new_width = (text_bbox.GetX() - background_bbox.GetX()) * 2 + text_bbox.GetWidth();
            double scale = new_width / background_bbox.GetWidth();
            ScaleElement(m_BackgroundElement, scale, 1);
   		}
	}
}

void SVGUITextCtrl::SetValue(const wxString& value)
{
	SetText(value);
	if (m_fixed_size && m_BackgroundElement)
	{
		wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
		wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
		if (text_bbox.GetX() + text_bbox.GetWidth() > background_bbox.GetX() + background_bbox.GetWidth())
		{
			unsigned int size = AjustSize(0, value.Length(), background_bbox, text_bbox);
			SetText(LocaleToUTF8(value.Left(size)));
		}
	}
	Refresh();
}

void SVGUITextCtrl::AddCharInText(const wxString& character)
{
	wxString label = GetValue();
	if (label != wxT(""))
	{
		wxString before_cursor = label.Left(m_cursor_position);
		wxString after_cursor = label.Right(label.Length() - m_cursor_position);
		wxString new_label = before_cursor + character + after_cursor;
		SetText(new_label);
		if (m_fixed_size && m_BackgroundElement)
		{
			wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
	        wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
	
		    if (text_bbox.GetX() < background_bbox.GetX()+ m_offset_ctm_x && m_style && wxALIGN_RIGHT)
		      SetText(label);
	    	else if (text_bbox.GetX()+text_bbox.GetWidth() > background_bbox.GetX()+background_bbox.GetWidth())
	    		SetText(label);
	    	else
	    		m_cursor_position += 1;
		}
		else
			m_cursor_position += 1;
	}
	else
	{
		SetText(character);
		//Initialize();
		SetText(character);
		m_cursor_position += 1;
	}
	Update_Cursor();
	Refresh();
}

void SVGUITextCtrl::DelCharInText(const bool backward)
{
	wxString label = GetValue();
	if (label != wxT(""))
	{
		if (backward)
		{
			if (m_cursor_position > 0)
			{
				wxString before_cursor = label.Left(m_cursor_position - 1);
				wxString after_cursor = label.Right(label.Length() - m_cursor_position);
				label = before_cursor + after_cursor;
				if (m_cursor_position > 0)
					m_cursor_position -= 1;
			}
		}
		else
		{
			if (m_cursor_position < label.Length())
			{
				wxString before_cursor = label.Left(m_cursor_position);
				wxString after_cursor = label.Right(label.Length() - m_cursor_position - 1);
				label = before_cursor + after_cursor;
			}
		}
		SetText(label);
	}
	Update_Cursor();
	Refresh();
}

void SVGUITextCtrl::MoveCursor(const int direction)
{
	wxString label = GetValue();
	if (m_cursor_position + direction >= 0 && m_cursor_position + direction <= label.Length())
	{
		m_cursor_position += direction;
		Update_Cursor();
		Refresh();
	}
}

unsigned int SVGUITextCtrl::RecursiveCursorSearch(wxSVGPoint& point, unsigned int start, unsigned int end)
{
	if (start == end)
	{
		wxSVGRect char_bbox = ((wxSVGTextElement*)m_TextElement)->GetExtentOfChar(start);
		double middle_position = char_bbox.GetX() + char_bbox.GetWidth() / 2;
		if (point.GetX() <= middle_position)
			return start;
		else
			return start + 1;
	}
	else
	{
		int middle = (end + start) / 2;
		wxSVGPoint end_middle = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(middle);
		if (point.GetX() <= end_middle.GetX())
			return RecursiveCursorSearch(point, start, middle);
		else
			return RecursiveCursorSearch(point, middle + 1, end); 
	}
}

void SVGUITextCtrl::FindCursorIndex(wxSVGPoint& point)
{
	wxString label = GetValue();
	
	if (m_TextElement)
		if (label.Length() > 0)
		{
			if (m_style && wxALIGN_RIGHT)
			{
				wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
				wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
				point.SetX(point.GetX()-background_bbox.GetWidth()+text_bbox.GetWidth()+ m_offset.GetX());
			} 
			m_cursor_position = RecursiveCursorSearch(point, 0, label.Length()-1);
		}
		else
			m_cursor_position = 0;
}

void SVGUITextCtrl::Update_Cursor()
{
	wxString label = GetValue();
	if (m_selected && m_CursorElement && m_TextElement)
	{
        wxSVGRect cursor_bbox = wxSVGLocatable::GetElementResultBBox(m_CursorElement, wxSVG_COORDINATES_VIEWPORT);
		double Xposition = 0;
		double Yposition = cursor_bbox.GetY()-0.5;
		unsigned int nb_chars = label.Length();
		if (m_style && wxALIGN_RIGHT)
		{
			
			
			if (m_BackgroundElement)
			{
					wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
					wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
		  		double Xposition = background_bbox.GetWidth() - m_offset.GetX() - text_bbox.GetWidth();
		      double Yposition = background_bbox.GetHeight();
		      //MoveElement(m_TextElement, Xposition, Yposition);
			}
			
			
			
			
			if (m_cursor_position > 0 && m_cursor_position < nb_chars)
			{
				wxSVGPoint end_char_before = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(m_cursor_position-1);
				wxSVGPoint start_char_after = ((wxSVGTextElement*)m_TextElement)->GetStartPositionOfChar(m_cursor_position);
				wxSVGRect background_bbox =wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
				wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
				Xposition = (end_char_before.GetX() + start_char_after.GetX() - cursor_bbox.GetWidth())/2+background_bbox.GetWidth()-text_bbox.GetWidth()- m_offset.GetX(); 
				/*double XEndPosition = bg_bbox.GetX()+bg_bbox.GetWidth();
				double XBeginPosition = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT).GetX();
				Xposition = XBeginPosition + m_cursor_position *20;*/
			}
			else if (m_cursor_position == 0)
			{
				if (nb_chars == 0)
				{
					wxSVGRect background_bbox = wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT);
					Xposition = background_bbox.GetWidth()+background_bbox.GetX()- m_offset.GetX()/2;
				}
				else
				{
					double XBeginPosition = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT).GetX();
					Xposition = XBeginPosition;
				}
			}
			else if (m_cursor_position == nb_chars)
			{ 
				wxSVGRect text_bbox = wxSVGLocatable::GetElementResultBBox(m_TextElement, wxSVG_COORDINATES_VIEWPORT);
				wxSVGRect bg_bbox =wxSVGLocatable::GetElementResultBBox(m_BackgroundElement, wxSVG_COORDINATES_VIEWPORT); 
				Xposition = text_bbox.GetX() + text_bbox.GetWidth();
			}
			
			
			
		}
		else
		{
			if (m_cursor_position > 0 && m_cursor_position < nb_chars)
			{
				wxSVGPoint end_char_before = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(m_cursor_position-1);
				wxSVGPoint start_char_after = ((wxSVGTextElement*)m_TextElement)->GetStartPositionOfChar(m_cursor_position);
				Xposition = (end_char_before.GetX() + start_char_after.GetX() - cursor_bbox.GetWidth())/2;
			}
			else if (m_cursor_position == 0)
			{
				if (nb_chars == 0)
				{
					SetText(wxT("x"));
				}
				wxSVGPoint first = ((wxSVGTextElement*)m_TextElement)->GetStartPositionOfChar(0);
				Xposition = first.GetX() - cursor_bbox.GetWidth();
				if (nb_chars == 0)
				{
					SetText(wxT(""));
				}
			}
			else if (m_cursor_position == nb_chars)
			{ 
				wxSVGPoint last = ((wxSVGTextElement*)m_TextElement)->GetEndPositionOfChar(nb_chars-1);
				Xposition = last.GetX();
			}
		}
		MoveElement(m_CursorElement,Xposition,Yposition);
	}
}

void SVGUITextCtrl::OnLeftDown(wxMouseEvent &event)
{
	if (!m_selected)
	{
		m_selected = true;
		Update_Elements();
	}
	SetValue(GetValue());
	wxSVGPoint point(event.GetX(), event.GetY());
	FindCursorIndex(point);
	Update_Cursor();
	Refresh();
	event.Skip();
}

void SVGUITextCtrl::OnMotion(wxMouseEvent &event)
{
	event.Skip();
}

void SVGUITextCtrl::OnLeftUp(wxMouseEvent &event)
{
	event.Skip();
}

void SVGUITextCtrl::OnChar(wxKeyEvent &event)
{
	int keycode = event.GetKeyCode();
	if (keycode == WXK_LEFT)
		MoveCursor(-1);
	else if (keycode == WXK_RIGHT)
		MoveCursor(1);
	else if (keycode == WXK_DELETE)
		DelCharInText(false);
	else if (keycode == WXK_BACK)
		DelCharInText(true);
	else if (keycode < 0 || keycode >= 32 && keycode <= 255)
		AddCharInText(wxChar(keycode));
	event.Skip();
}
