//////////////////////////////////////////////////////////////////////////////
// Name:        SVGStylable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/04/29
// RCS-ID:      $Id: SVGStylable.cpp,v 1.3 2005-10-17 14:04:07 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGStylable.h"
#include "svg.h"

const wxCSSValue& wxSVGStylable::GetPresentationAttribute(const wxString& name)
{
  return m_style.GetPropertyCSSValue(name);
}

bool wxSVGStylable::HasCustomAttribute(const wxString& name)
{
  return wxCSSStyleDeclaration::GetPropertyId(name) != wxCSS_PROPERTY_UNKNOWN;
}

wxString wxSVGStylable::GetCustomAttribute(const wxString& name)
{
  return m_style.GetPropertyValue(name);
}

bool wxSVGStylable::SetCustomAttribute(const wxString& name, const wxString& value)
{
  wxCSS_PROPERTY id = wxCSSStyleDeclaration::GetPropertyId(name);
  
  if (id == wxCSS_PROPERTY_UNKNOWN)
    return false;
	
  m_style.SetProperty(name, value);
  return true;
}

#define GET_ELEMENT_STYLE(the_dtd, the_class)\
case the_dtd:\
  return ((the_class&) element).GetStyle();

const wxCSSStyleDeclaration& wxSVGStylable::GetElementStyle(const wxSVGElement& element)
{
  static wxCSSStyleDeclaration emptyStyle;
  if (element.GetType() != wxXML_ELEMENT_NODE)
    return emptyStyle;
  switch (element.GetDtd())
  {
    GET_ELEMENT_STYLE(wxSVG_A_ELEMENT, wxSVGAElement)
    GET_ELEMENT_STYLE(wxSVG_ALTGLYPH_ELEMENT, wxSVGAltGlyphElement)
    GET_ELEMENT_STYLE(wxSVG_CIRCLE_ELEMENT, wxSVGCircleElement)
    GET_ELEMENT_STYLE(wxSVG_CLIPPATH_ELEMENT, wxSVGClipPathElement)
    GET_ELEMENT_STYLE(wxSVG_DEFS_ELEMENT, wxSVGDefsElement)
    GET_ELEMENT_STYLE(wxSVG_DESC_ELEMENT, wxSVGDescElement)
    GET_ELEMENT_STYLE(wxSVG_ELLIPSE_ELEMENT, wxSVGEllipseElement)
    GET_ELEMENT_STYLE(wxSVG_FILTER_ELEMENT, wxSVGFilterElement)
    GET_ELEMENT_STYLE(wxSVG_FONT_ELEMENT, wxSVGFontElement)
    GET_ELEMENT_STYLE(wxSVG_FOREIGNOBJECT_ELEMENT, wxSVGForeignObjectElement)
    GET_ELEMENT_STYLE(wxSVG_G_ELEMENT, wxSVGGElement)
    GET_ELEMENT_STYLE(wxSVG_GLYPH_ELEMENT, wxSVGGlyphElement)
    GET_ELEMENT_STYLE(wxSVG_GLYPHREF_ELEMENT, wxSVGGlyphRefElement)
    GET_ELEMENT_STYLE(wxSVG_LINEARGRADIENT_ELEMENT, wxSVGLinearGradientElement)
    GET_ELEMENT_STYLE(wxSVG_RADIALGRADIENT_ELEMENT, wxSVGRadialGradientElement)
    GET_ELEMENT_STYLE(wxSVG_IMAGE_ELEMENT, wxSVGImageElement)
    GET_ELEMENT_STYLE(wxSVG_LINE_ELEMENT, wxSVGLineElement)
    GET_ELEMENT_STYLE(wxSVG_MARKER_ELEMENT, wxSVGMarkerElement)
    GET_ELEMENT_STYLE(wxSVG_MASK_ELEMENT, wxSVGMaskElement)
    GET_ELEMENT_STYLE(wxSVG_MISSING_GLYPH_ELEMENT, wxSVGMissingGlyphElement)
    GET_ELEMENT_STYLE(wxSVG_PATH_ELEMENT, wxSVGPathElement)
    GET_ELEMENT_STYLE(wxSVG_PATTERN_ELEMENT, wxSVGPatternElement)
    GET_ELEMENT_STYLE(wxSVG_POLYGON_ELEMENT, wxSVGPolygonElement)
    GET_ELEMENT_STYLE(wxSVG_POLYLINE_ELEMENT, wxSVGPolylineElement)
    GET_ELEMENT_STYLE(wxSVG_RECT_ELEMENT, wxSVGRectElement)
    GET_ELEMENT_STYLE(wxSVG_SVG_ELEMENT, wxSVGSVGElement)
    GET_ELEMENT_STYLE(wxSVG_STOP_ELEMENT, wxSVGStopElement)
    GET_ELEMENT_STYLE(wxSVG_SWITCH_ELEMENT, wxSVGSwitchElement)
    GET_ELEMENT_STYLE(wxSVG_SYMBOL_ELEMENT, wxSVGSymbolElement)
    GET_ELEMENT_STYLE(wxSVG_TITLE_ELEMENT, wxSVGTitleElement)
    GET_ELEMENT_STYLE(wxSVG_TEXT_ELEMENT, wxSVGTextElement)
    GET_ELEMENT_STYLE(wxSVG_TEXTPATH_ELEMENT, wxSVGTextPathElement)
    GET_ELEMENT_STYLE(wxSVG_TREF_ELEMENT, wxSVGTRefElement)
    GET_ELEMENT_STYLE(wxSVG_TSPAN_ELEMENT, wxSVGTSpanElement)
    GET_ELEMENT_STYLE(wxSVG_USE_ELEMENT, wxSVGUseElement)
    GET_ELEMENT_STYLE(wxSVG_FEBLEND_ELEMENT, wxSVGFEBlendElement)
    GET_ELEMENT_STYLE(wxSVG_FECOLORMATRIX_ELEMENT, wxSVGFEColorMatrixElement)
    GET_ELEMENT_STYLE(wxSVG_FECOMPONENTTRANSFER_ELEMENT, wxSVGFEComponentTransferElement)
    GET_ELEMENT_STYLE(wxSVG_FECOMPOSITE_ELEMENT, wxSVGFECompositeElement)
    GET_ELEMENT_STYLE(wxSVG_FECONVOLVEMATRIX_ELEMENT, wxSVGFEConvolveMatrixElement)
    GET_ELEMENT_STYLE(wxSVG_FEDIFFUSELIGHTING_ELEMENT, wxSVGFEDiffuseLightingElement)
    GET_ELEMENT_STYLE(wxSVG_FEDISPLACEMENTMAP_ELEMENT, wxSVGFEDisplacementMapElement)
    GET_ELEMENT_STYLE(wxSVG_FEFLOOD_ELEMENT, wxSVGFEFloodElement)
    GET_ELEMENT_STYLE(wxSVG_FEGAUSSIANBLUR_ELEMENT, wxSVGFEGaussianBlurElement)
    GET_ELEMENT_STYLE(wxSVG_FEMERGE_ELEMENT, wxSVGFEMergeElement)
    GET_ELEMENT_STYLE(wxSVG_FEMORPHOLOGY_ELEMENT, wxSVGFEMorphologyElement)
    GET_ELEMENT_STYLE(wxSVG_FEOFFSET_ELEMENT, wxSVGFEOffsetElement)
    GET_ELEMENT_STYLE(wxSVG_FESPECULARLIGHTING_ELEMENT, wxSVGFESpecularLightingElement)
    GET_ELEMENT_STYLE(wxSVG_FETILE_ELEMENT, wxSVGFETileElement)
    GET_ELEMENT_STYLE(wxSVG_FETURBULENCE_ELEMENT, wxSVGFETurbulenceElement)
    default:
      break;
  }
  return emptyStyle;
}
