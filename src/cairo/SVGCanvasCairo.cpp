//////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasCairo.cpp
// Purpose:     Cairo render
// Author:      Alex Thuering
// Created:     2005/05/12
// RCS-ID:      $Id: SVGCanvasCairo.cpp,v 1.1 2005-08-06 19:25:51 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGCanvasCairo.h"
#include "SVGCanvasPathCairo.h"
#include "../freetype/SVGCanvasTextFreetype.h"
#include <wx/log.h>
#include <wx/file.h>

wxSVGCanvasCairo::~wxSVGCanvasCairo()
{
  if (m_cr)
	cairo_destroy(m_cr);
  if (m_surface)
	cairo_surface_destroy(m_surface);
  if (m_data)
    delete[] m_data;
}

void wxSVGCanvasCairo::Init(int width, int height)
{
  m_data = new unsigned char[width*height*4];
  m_surface = cairo_image_surface_create_for_data(m_data,
	CAIRO_FORMAT_RGB24, width, height, width*4);
  m_cr = cairo_create(m_surface);
}

wxImage wxSVGCanvasCairo::GetImage()
{
  wxImage img;
  int width = cairo_image_surface_get_width(m_surface);
  int height = cairo_image_surface_get_height(m_surface);
  img.Create(width, height);
  unsigned char* src = m_data;
  unsigned char* dst = img.GetData();
  for (int i=0; i<width*height; i++)
  {
    dst[0] = src[2];
    dst[1] = src[1];
    dst[2] = src[0];
    src+=4; dst+=3;
  }
  return img;
}

void wxSVGCanvasCairo::Clear(wxRGBColor color)
{
  if (!m_cr || !m_surface)
    return;
  cairo_set_source_rgb(m_cr, color.Red()/255.0, color.Green()/255.0, color.Blue()/255.0);
  cairo_rectangle(m_cr, 0, 0, cairo_image_surface_get_width(m_surface),
	cairo_image_surface_get_height(m_surface));
  cairo_fill(m_cr);
}

wxSVGCanvasPath* wxSVGCanvasCairo::CreateCanvasPath()
{
  return new wxSVGCanvasPathCairo();
}

wxSVGCanvasItem* wxSVGCanvasCairo::CreateItem(wxSVGTextElement* element,
  wxCSSStyleDeclaration* style)
{
  wxSVGCanvasTextFreetype* canvasText = new wxSVGCanvasTextFreetype;
  if (style == NULL)
	style = (wxCSSStyleDeclaration*) &element->GetStyle();
  canvasText->Init(*element, *style);
  return canvasText;
}

void wxSVGCanvasCairo::DrawItem(wxSVGCanvasItem& item,
  wxSVGMatrix& matrix, wxCSSStyleDeclaration& style)
{
  switch (item.GetType())
  {
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_PATH:
	  DrawCanvasPath((wxSVGCanvasPathCairo&) item, matrix, style);
	  break;
    case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_TEXT:
      DrawCanvasText((wxSVGCanvasTextFreetype&) item, matrix, style);
	  break;
	case wxSVGCanvasItem::wxSVG_CANVAS_ITEM_IMAGE:
	  break;
  }
}

void wxSVGCanvasCairo::DrawCanvasPath(wxSVGCanvasPathCairo& canvasPath,
 wxSVGMatrix& matrix, wxCSSStyleDeclaration& style)
{
  cairo_matrix_t affine;
  affine.xx = matrix.GetA();
  affine.yx = matrix.GetB();
  affine.xy = matrix.GetC();
  affine.yy = matrix.GetD();
  affine.x0 = matrix.GetE();
  affine.y0 = matrix.GetF();
  cairo_set_matrix(m_cr, &affine);
  
  // Filling
  if (canvasPath.GetFill() && style.GetFill().Ok() &&
	  style.GetFill().GetPaintType() != wxSVG_PAINTTYPE_URI &&
	  style.GetFill().GetPaintType() != wxSVG_PAINTTYPE_URI_NONE)
  {
    cairo_path_t* path = canvasPath.GetPath();
    cairo_append_path(m_cr, path);
  
    wxRGBColor color = style.GetFill().GetRGBColor();
    cairo_set_source_rgb(m_cr, color.Red()/255.0, color.Green()/255.0, color.Blue()/255.0);
    cairo_fill(m_cr);
    
    cairo_path_destroy(path);
  }
  
  // Stroking
  if (style.GetStroke().Ok() && style.GetStrokeWidth()>0 &&
	  style.GetStroke().GetPaintType() != wxSVG_PAINTTYPE_URI &&
	  style.GetStroke().GetPaintType() != wxSVG_PAINTTYPE_URI_NONE)
  {
    cairo_path_t* path = canvasPath.GetPath();
    cairo_append_path(m_cr, path);
    
    wxRGBColor color = style.GetStroke().GetRGBColor();
    cairo_set_source_rgb(m_cr, color.Red()/255.0, color.Green()/255.0, color.Blue()/255.0);
    cairo_stroke(m_cr);
    
    cairo_path_destroy(path);
  }
}

void wxSVGCanvasCairo::DrawCanvasText(wxSVGCanvasTextFreetype& canvasText,
  wxSVGMatrix& matrix, wxCSSStyleDeclaration& style)
{
  /*for (int i=0; i<(int)canvasText.m_chunks.Count(); i++)
  {
	wxCanvasTextChunk& chunk = canvasText.m_chunks[i];
	chunk.style.SetFillRule(wxCSS_VALUE_EVENODD);
	wxSVGMatrix pathMatrix = matrix.Multiply(chunk.matrix);
	DrawCanvasPath(chunk.path, pathMatrix, chunk.style);
  }*/
}
