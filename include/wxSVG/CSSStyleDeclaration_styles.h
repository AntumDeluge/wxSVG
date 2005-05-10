//////////////////////////////////////////////////////////////////////////////
// Name:        CSSStyleDeclaration_styles.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/07
// RCS-ID:      $Id: CSSStyleDeclaration_styles.h,v 1.1.1.1 2005-05-10 17:51:11 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

// type, get_function, method_name, dtd_name, def_value
ATTRDEF(wxRGBColor, GetRGBColorValue, Color, "color", wxRGBColor())
ATTRDEF(wxRGBColor, GetRGBColorValue, Fill, "fill", wxRGBColor(0,0,0))
ATTRDEF(wxRGBColor, GetRGBColorValue, Stroke, "stroke", wxRGBColor())
ATTRDEF(wxRGBColor, GetRGBColorValue, StopColor, "stop-color", wxRGBColor())
ATTRDEF(float, GetFloatValue, StrokeWidth, "stroke-width", 1)
ATTRDEF(float, GetFloatValue, Opacity, "opacity", 1)
ATTRDEF(float, GetFloatValue, FillOpacity, "fill-opacity", 1)
ATTRDEF(float, GetFloatValue, StrokeOpacity, "stroke-opacity", 1)
ATTRDEF(float, GetFloatValue, StopOpacity, "stop-opacity",  1)
ATTRDEF(wxString, GetStringValue, FillRule, "fill-rule", wxT("nonzero"))
ATTRDEF(wxString, GetStringValue, ClipPath, "clip-path", wxT(""))
ATTRDEF(wxString, GetStringValue, FontFamily, "font-family", wxT(""))
ATTRDEF(float, GetFloatValue, FontSize, "font-size", 20)
ATTRDEF(wxString, GetStringValue, FontStyle, "font-style", wxT(""))
ATTRDEF(wxString, GetStringValue, FontWeight, "font-weight", wxT(""))
ATTRDEF(wxString, GetStringValue, Filter, "filter", wxT(""))
ATTRDEF(wxString, GetStringValue, Marker, "marker", wxT(""))
ATTRDEF(wxString, GetStringValue, MarkerStart, "marker-start", wxT("none"))
ATTRDEF(wxString, GetStringValue, MarkerEnd, "marker-end", wxT("none"))
ATTRDEF(wxString, GetStringValue, MarkerMid, "marker-mid", wxT("none"))
ATTRDEF(bool, GetBooleanValue, Visibility, "visibility", true)
ATTRDEF(bool, GetBooleanValue, Display, "display", true)
ATTRDEF(wxString, GetStringValue, TextAnchor, "text-anchor", wxT(""))
ATTRDEF(wxString, GetStringValue, StrokeLinecap, "stroke-linecap", wxT("butt"))
ATTRDEF(wxString, GetStringValue, StrokeLinejoin, "stroke-linejoin", wxT("miter"))
ATTRDEF(float, GetFloatValue, StrokeMiterlimit, "stroke-miterlimit", 4)
ATTRDEF(float, GetFloatValue, StrokeDashoffset, "stroke-dashoffset", 0)
//ATTRDEF(wxNumberList, GetNumberListValue, StrokeDasharray, "stroke-dasharray")

