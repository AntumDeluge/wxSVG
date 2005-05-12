//////////////////////////////////////////////////////////////////////////////
// Name:        SVGDocument.cpp
// Purpose:     wxSVGDocument - SVG render & data holder class
// Author:      Alex Thuering
// Created:     2005/01/17
// RCS-ID:      $Id: SVGDocument.cpp,v 1.4 2005-05-12 04:01:25 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGDocument.h"
#include "libart/SVGCanvasLibart.h"

#include <wx/log.h>

wxSVGDocument::~wxSVGDocument()
{
  delete m_canvas;
}

void wxSVGDocument::Init()
{
  m_canvas = new wxSVGCanvasLibart;
}

wxXmlElement* wxSVGDocument::CreateElement(const wxString& tagName)
{
  return CreateElementNS(wxT(""), tagName);
}

#include "SVGDocument_CreateElement.cpp"

void RenderChilds(wxSVGCanvas* canvas, wxSVGElement* parent,
  wxSVGMatrix* parentMatrix, wxCSSStyleDeclaration* parentStyle)
{
  wxSVGElement* elem = (wxSVGElement*) parent->GetChildren();
  while (elem)
  {
	if (elem->GetType() == wxXML_ELEMENT_NODE)
	{
	  wxSVGMatrix matrix(*parentMatrix);
	  wxCSSStyleDeclaration style(*parentStyle);
	  
	  switch (elem->GetDtd())
	  {
		case wxSVG_SVG_ELEMENT:
		  RenderChilds(canvas, elem, &matrix, &style);
		  break;
		case wxSVG_G_ELEMENT:
		{
		  wxSVGGElement* element = (wxSVGGElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  RenderChilds(canvas, elem, &matrix, &style);
		  break;
		}
		case wxSVG_LINE_ELEMENT:
		{
		  wxSVGLineElement* element = (wxSVGLineElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawLine(element, &matrix, &style);
		  break;
		}
		case wxSVG_POLYLINE_ELEMENT:
		{
		  wxSVGPolylineElement* element = (wxSVGPolylineElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPolyline(element, &matrix, &style);
		  break;
		}
		case wxSVG_POLYGON_ELEMENT:
		{
		  wxSVGPolygonElement* element = (wxSVGPolygonElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPolygon(element, &matrix, &style);
		  break;
		}
		case wxSVG_RECT_ELEMENT:
		{
		  wxSVGRectElement* element = (wxSVGRectElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawRect(element, &matrix, &style);
		  break;
		}
		case wxSVG_CIRCLE_ELEMENT:
		{
		  wxSVGCircleElement* element = (wxSVGCircleElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawCircle(element, &matrix, &style);
		  break;
		}
		case wxSVG_ELLIPSE_ELEMENT:
		{
		  wxSVGEllipseElement* element = (wxSVGEllipseElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawEllipse(element, &matrix, &style);
		  break;
		}
		case wxSVG_PATH_ELEMENT:
		{
		  wxSVGPathElement* element = (wxSVGPathElement*) elem;
		  element->UpdateMatrix(matrix);
		  style.Add(element->GetStyle());
		  canvas->DrawPath(element, &matrix, &style);
		  break;
		}
		case wxSVG_FETURBULENCE_ELEMENT:
		case wxSVG_FEGAUSSIANBLUR_ELEMENT:
		case wxSVG_FETILE_ELEMENT:
		case wxSVG_TEXTPATH_ELEMENT:
		case wxSVG_TEXT_ELEMENT:
		case wxSVG_IMAGE_ELEMENT:
		case wxSVG_FONT_FACE_URI_ELEMENT:
		case wxSVG_MISSING_GLYPH_ELEMENT:
		case wxSVG_FONT_FACE_ELEMENT:
		case wxSVG_SET_ELEMENT:
		case wxSVG_HKERN_ELEMENT:
		case wxSVG_MARKER_ELEMENT:
		case wxSVG_ALTGLYPH_ELEMENT:
		case wxSVG_ANIMATE_ELEMENT:
		case wxSVG_FONT_ELEMENT:
		case wxSVG_COLOR_PROFILE_ELEMENT:
		case wxSVG_MPATH_ELEMENT:
		case wxSVG_CURSOR_ELEMENT:
		case wxSVG_USE_ELEMENT:
		case wxSVG_FONT_FACE_SRC_ELEMENT:
		case wxSVG_TITLE_ELEMENT:
		case wxSVG_DEFINITION_SRC_ELEMENT:
		case wxSVG_TSPAN_ELEMENT:
		case wxSVG_FEFLOOD_ELEMENT:
		case wxSVG_FEBLEND_ELEMENT:
		case wxSVG_VKERN_ELEMENT:
		case wxSVG_FESPECULARLIGHTING_ELEMENT:
		case wxSVG_FECOLORMATRIX_ELEMENT:
		case wxSVG_FEIMAGE_ELEMENT:
		case wxSVG_RADIALGRADIENT_ELEMENT:
		case wxSVG_GLYPHREF_ELEMENT:
		case wxSVG_METADATA_ELEMENT:
		case wxSVG_DEFS_ELEMENT:
		case wxSVG_FEFUNCA_ELEMENT:
		case wxSVG_FEFUNCB_ELEMENT:
		case wxSVG_FONT_FACE_NAME_ELEMENT:
		case wxSVG_FEPOINTLIGHT_ELEMENT:
		case wxSVG_FESPOTLIGHT_ELEMENT:
		case wxSVG_FEFUNCG_ELEMENT:
		case wxSVG_FECONVOLVEMATRIX_ELEMENT:
		case wxSVG_FEDISTANTLIGHT_ELEMENT:
		case wxSVG_SYMBOL_ELEMENT:
		case wxSVG_CLIPPATH_ELEMENT:
		case wxSVG_STOP_ELEMENT:
		case wxSVG_STYLE_ELEMENT:
		case wxSVG_ANIMATECOLOR_ELEMENT:
		case wxSVG_FEDISPLACEMENTMAP_ELEMENT:
		case wxSVG_FEFUNCR_ELEMENT:
		case wxSVG_GLYPH_ELEMENT:
		case wxSVG_A_ELEMENT:
		case wxSVG_FECOMPOSITE_ELEMENT:
		case wxSVG_FOREIGNOBJECT_ELEMENT:
		case wxSVG_DESC_ELEMENT:
		case wxSVG_ALTGLYPHITEM_ELEMENT:
		case wxSVG_FECOMPONENTTRANSFER_ELEMENT:
		case wxSVG_FEMORPHOLOGY_ELEMENT:
		case wxSVG_ANIMATEMOTION_ELEMENT:
		case wxSVG_FEDIFFUSELIGHTING_ELEMENT:
		case wxSVG_SCRIPT_ELEMENT:
		case wxSVG_MASK_ELEMENT:
		case wxSVG_ALTGLYPHDEF_ELEMENT:
		case wxSVG_TREF_ELEMENT:
		case wxSVG_FILTER_ELEMENT:
		case wxSVG_FONT_FACE_FORMAT_ELEMENT:
		case wxSVG_SWITCH_ELEMENT:
		case wxSVG_ANIMATETRANSFORM_ELEMENT:
		case wxSVG_LINEARGRADIENT_ELEMENT:
		case wxSVG_FEOFFSET_ELEMENT:
		case wxSVG_PATTERN_ELEMENT:
		case wxSVG_FEMERGE_ELEMENT:
		case wxSVG_VIEW_ELEMENT:
		case wxSVG_FEMERGENODE_ELEMENT:
		case wxSVG_UNKNOWN_ELEMENT:
		  break;
	  }
	}
	elem = (wxSVGElement*) elem->GetNext();
  }
}

wxImage wxSVGDocument::Render(int width, int height)
{
  if (!GetRootElement())
	return wxImage();
  
  wxSVGMatrix matrix;
  
  // view box
  wxSVGRect viewbox = GetRootElement()->GetViewBox();
  if (viewbox.GetWidth()<=0 && viewbox.GetHeight()<=0)
    viewbox = wxSVGRect(0, 0, GetRootElement()->GetWidth(), GetRootElement()->GetHeight());
  if (viewbox.GetWidth()<=0 || viewbox.GetHeight()<=0)
  {
	viewbox = GetRootElement()->GetBBox();
	matrix = matrix.Scale(0.95).Translate(width*0.025, height*0.025);
  }
  
  // scale it to fit in
  float scale = 1;
  if (width == -1 || height == -1)
  {
	width = (int) viewbox.GetWidth();
	height = (int) viewbox.GetHeight();
  }
  else if (viewbox.GetWidth()>0 && viewbox.GetHeight()>0)
  {
	scale = width/viewbox.GetWidth();
	if (scale>height/viewbox.GetHeight())
	  scale = height/viewbox.GetHeight();
  }
  matrix = matrix.Scale(scale);
  if (viewbox.GetX()!=0 || viewbox.GetY()!=0)
	matrix = matrix.Translate(-viewbox.GetX(), -viewbox.GetY());
  
  // render
  wxImage image(width, height);
  m_canvas->SetImage(&image);
  m_canvas->Clear();
  RenderChilds(m_canvas, GetRootElement(), &matrix, &GetRootElement()->GetStyle());
  
  return image;
}

