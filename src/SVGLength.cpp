//////////////////////////////////////////////////////////////////////////////
// Name:        SVGLength.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGLength.cpp,v 1.2 2005-05-11 03:02:08 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGLength.h"

wxString wxSVGLength::GetValueAsString() const
{
  return wxString::Format(wxT("%f"), m_value);
}

void wxSVGLength::SetValueAsString(const wxString& n)
{
  m_value = 0;
  m_unitType = wxSVG_LENGTHTYPE_NUMBER;
  wxString value = n.Strip(wxString::both);
  wxString unit;
  if (value.length()>=2)
  {
	const wxString s_numeric = wxT("0123456789");
	const wxString s_numericFirst = wxT("+-.Ee") + s_numeric;
	if (!s_numeric.Contains(value.Right(1)))
	{
	  if (s_numericFirst.Contains(value.Mid(value.Length()-2,1)))
	  {
		unit = value.Right(1);
		value = value.Left(value.Length()-1);
	  }
	  else
	  {
		unit = value.Right(2);
		value = value.Left(value.Length()-2);
	  }
	}
  }
  
  double d;
  if (!value.ToDouble(&d))
	return;
  m_value = d;
  
  if (unit.length() == 0);
  else if (unit == wxT("px"))
	m_unitType = wxSVG_LENGTHTYPE_PX;
  else if (unit.Right(1) == wxT("%"))
	m_unitType = wxSVG_LENGTHTYPE_PERCENTAGE;
  else if (unit == wxT("em"))
	m_unitType = wxSVG_LENGTHTYPE_EMS;
  else if (unit == wxT("ex"))
	m_unitType = wxSVG_LENGTHTYPE_EXS;
  else if (unit == wxT("cm"))
	m_unitType = wxSVG_LENGTHTYPE_CM;
  else if (unit == wxT("mm"))
	m_unitType = wxSVG_LENGTHTYPE_MM;
  else if (unit == wxT("in"))
	m_unitType = wxSVG_LENGTHTYPE_IN;
  else if (unit == wxT("pt"))
	m_unitType = wxSVG_LENGTHTYPE_PT;
  else if (unit == wxT("pc"))
	m_unitType = wxSVG_LENGTHTYPE_PC;
  SetValueInSpecifiedUnits(m_value);
}

void wxSVGLength::NewValueSpecifiedUnits(wxSVG_LENGTHTYPE unitType, float valueInSpecifiedUnits)
{

}

void wxSVGLength::ConvertToSpecifiedUnits(wxSVG_LENGTHTYPE unitType)
{
  m_unitType = unitType;
}

float wxSVGLength::GetValueInSpecifiedUnits() const
{
  float value = m_value;
  switch (m_unitType)
  {
    case wxSVG_LENGTHTYPE_UNKNOWN:
	case wxSVG_LENGTHTYPE_NUMBER:
	case wxSVG_LENGTHTYPE_PX: break;
	case wxSVG_LENGTHTYPE_PERCENTAGE: break; // todo
	case wxSVG_LENGTHTYPE_EMS: break; // todo
	case wxSVG_LENGTHTYPE_EXS: break; // todo
	case wxSVG_LENGTHTYPE_CM: value /= 35.43307; break;
	case wxSVG_LENGTHTYPE_MM: value /= 3.543307; break;
	case wxSVG_LENGTHTYPE_IN: value /= 90; break;
	case wxSVG_LENGTHTYPE_PT: value /= 1.25; break;
	case wxSVG_LENGTHTYPE_PC: value /= 15; break;
  }
  return value;
}

void wxSVGLength::SetValueInSpecifiedUnits(const float n)
{
  m_value = n;
  switch (m_unitType)
  {
	case wxSVG_LENGTHTYPE_UNKNOWN: break;
	case wxSVG_LENGTHTYPE_NUMBER: break;
	case wxSVG_LENGTHTYPE_PX: break;
	case wxSVG_LENGTHTYPE_PERCENTAGE: break; // todo
	case wxSVG_LENGTHTYPE_EMS: break; // todo
	case wxSVG_LENGTHTYPE_EXS: break; // todo
	case wxSVG_LENGTHTYPE_CM: m_value *= 35.43307; break;
	case wxSVG_LENGTHTYPE_MM: m_value *= 3.543307; break;
	case wxSVG_LENGTHTYPE_IN: m_value *= 90; break;
	case wxSVG_LENGTHTYPE_PT: m_value *= 1.25; break;
	case wxSVG_LENGTHTYPE_PC: m_value *= 15; break;
  }
}
