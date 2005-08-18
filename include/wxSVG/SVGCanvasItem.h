/////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasItem.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/09
// RCS-ID:      $Id: SVGCanvasItem.h,v 1.5.2.1 2005-08-18 14:05:16 lbessard Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_ITEM_H
#define WX_SVG_CANVAS_ITEM_H

#include "svg.h"

class wxSVGCanvasItem
{
  public:
	enum wxSVGCanvasItemType
	{
	  wxSVG_CANVAS_ITEM_PATH,
	  wxSVG_CANVAS_ITEM_TEXT,
	  wxSVG_CANVAS_ITEM_IMAGE
	};
	
	wxSVGCanvasItem(wxSVGCanvasItemType type) { m_type = type; }
	virtual ~wxSVGCanvasItem() {}
	wxSVGCanvasItemType GetType() { return m_type; }
	
	virtual wxSVGRect GetBBox() = 0;
	//virtual wxSVGRect GetResultBBox() = 0;
	
  protected:
	wxSVGCanvasItemType m_type;
};

class wxSVGCanvasPath: public wxSVGCanvasItem
{
  public:
	wxSVGCanvasPath();
	virtual ~wxSVGCanvasPath() {}
	
	void Init(wxSVGLineElement& element);
	void Init(wxSVGPolylineElement& element);
	void Init(wxSVGPolygonElement& element);
	void Init(wxSVGRectElement& element);
	void Init(wxSVGCircleElement& element);
	void Init(wxSVGEllipseElement& element);
	void Init(wxSVGPathElement& element);
		
	void MoveTo(double x, double y, bool relative = false);
	void LineTo(double x, double y, bool relative = false);
	void LineToHorizontal(double x, bool relative = false);
	void LineToVertical(double y, bool relative = false);
	void CurveToCubic(double x1, double y1, double x2, double y2, double x, double y, bool relative = false);
	void CurveToCubicSmooth(double x2, double y2, double x, double y, bool relative = false);
	void CurveToQuadratic(double x1, double y1, double x, double y, bool relative = false);
	void CurveToQuadraticSmooth(double x, double y, bool relative = false);
	void Arc(double x, double y, double r1, double r2, double angle,
	  bool largeArcFlag, bool sweepFlag, bool relative = false);
	bool ClosePath();
	
	virtual void End() = 0;
	
	inline void SetFill(bool fill = true) { m_fill = fill; }
	inline bool GetFill() { return m_fill; }
  
  protected:
	bool m_fill;
	double m_curx, m_cury, m_cubicx, m_cubicy, m_quadx, m_quady, m_begx, m_begy;
	virtual void MoveToImpl(double x, double y) = 0;
	virtual void LineToImpl(double x, double y) = 0;
	virtual void CurveToCubicImpl(double x1, double y1, double x2, double y2, double x, double y) = 0;
	virtual bool ClosePathImpl() = 0;
};

class wxSVGCanvasText: public wxSVGCanvasItem
{
  public:
	wxSVGCanvasText();
	virtual ~wxSVGCanvasText() {}
	
	virtual void Init(wxSVGTextElement& element, wxCSSStyleDeclaration& style);
	virtual long GetNumberOfChars();
    virtual double GetComputedTextLength();
    virtual double GetSubStringLength(unsigned long charnum, unsigned long nchars);
    virtual wxSVGPoint GetStartPositionOfChar(unsigned long charnum);
    virtual wxSVGPoint GetEndPositionOfChar(unsigned long charnum);
    virtual wxSVGRect GetExtentOfChar(unsigned long charnum);
    virtual double GetRotationOfChar(unsigned long charnum);
    virtual long GetCharNumAtPosition(const wxSVGPoint& point);
	
  protected:
    double m_tx, m_ty;
	virtual void Init(wxSVGTSpanElement& element, wxCSSStyleDeclaration& style);
	virtual void InitChildren(wxSVGTextPositioningElement& element, wxCSSStyleDeclaration& style);
    virtual void InitText(const wxString& text) = 0;
	virtual void BeginChunk(wxCSSStyleDeclaration& style) = 0;
	virtual void EndChunk() = 0;
};

class wxSVGCanvasImage: public wxSVGCanvasItem
{
  public:
	wxSVGCanvasImage(): wxSVGCanvasItem(wxSVG_CANVAS_ITEM_IMAGE) {}
	virtual ~wxSVGCanvasImage() {}
	virtual void Init(wxSVGImageElement& element);
	wxSVGRect GetBBox() { return wxSVGRect(); }
  
  public:
	double m_x, m_y, m_width, m_height;
	wxImage m_image;
};

#endif // WX_SVG_CANVAS_ITEM_H
