//////////////////////////////////////////////////////////////////////////////
// Name:        SVGRect.h
// Author:      Alex Thuering
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
// Notes:       generated by generate.py
//////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_RECT_H
#define WX_SVG_RECT_H

class wxSVGRect
{
  protected:
	float m_x;
	float m_y;
	float m_width;
	float m_height;

  public:
	inline float GetX() const { return m_x; }
	inline void SetX(const float n) { m_x = n; }

	inline float GetY() const { return m_y; }
	inline void SetY(const float n) { m_y = n; }

	inline float GetWidth() const { return m_width; }
	inline void SetWidth(const float n) { m_width = n; }

	inline float GetHeight() const { return m_height; }
	inline void SetHeight(const float n) { m_height = n; }

  public:
	wxSVGRect(): m_x(0), m_y(0), m_width(0), m_height(0) {}
	wxSVGRect(float x, float y, float width, float height):
	  m_x(x), m_y(y), m_width(width), m_height(height) {}
};

#endif // WX_SVG_RECT_H
