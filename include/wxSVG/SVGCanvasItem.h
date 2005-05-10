/////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasItem.h
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/09
// RCS-ID:      $Id: SVGCanvasItem.h,v 1.1.1.1 2005-05-10 17:51:20 ntalex Exp $
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
	wxSVGCanvasPath(): wxSVGCanvasItem(wxSVG_CANVAS_ITEM_PATH) { m_fill = true; }
	virtual ~wxSVGCanvasPath() {}
	
	void Init(wxSVGLineElement& element);
	void Init(wxSVGPolylineElement& element);
	void Init(wxSVGPolygonElement& element);
	void Init(wxSVGRectElement& element);
	void Init(wxSVGCircleElement& element);
	void Init(wxSVGEllipseElement& element);
	void Init(wxSVGPathElement& element);
	
	virtual void MoveTo(double x, double y, bool relative = false) = 0;
	virtual void LineTo(double x, double y, bool relative = false) = 0;
	virtual void LineToHorizontal(double x, bool relative = false) = 0;
	virtual void LineToVertical(double y, bool relative = false) = 0;
	virtual void CurveToCubic(double x1, double y1, double x2, double y2, double x, double y, bool relative = false) = 0;
	virtual void CurveToCubicSmooth(double x2, double y2, double x, double y, bool relative = false) = 0;
	virtual void CurveToQuadratic(double x1, double y1, double x, double y, bool relative = false) = 0;
	virtual void CurveToQuadraticSmooth(double x, double y, bool relative = false) = 0;
	virtual void Arc(double x, double y, double r1, double r2, double angle,
	  bool largeArcFlag, bool sweepFlag, bool relative = false) = 0;
	virtual bool ClosePath() = 0;
	virtual void End() = 0;
	
	inline void SetFill(bool fill = true) { m_fill = fill; }
	inline bool GetFill() { return m_fill; }
  
  protected:
	bool m_fill;
	void ArcToCubic(double x, double y, double r1, double r2, double angle,
	  bool largeArcFlag, bool sweepFlag, bool relative, double curx, double cury);
};

#endif // WX_SVG_CANVAS_ITEM_H
