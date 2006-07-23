/////////////////////////////////////////////////////////////////////////////
// Name:        SVGCanvasItem.h
// Purpose:     Canvas items
// Author:      Alex Thuering
// Created:     2005/05/09
// RCS-ID:      $Id: SVGCanvasItem.h,v 1.11 2006-07-23 16:26:04 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef WX_SVG_CANVAS_ITEM_H
#define WX_SVG_CANVAS_ITEM_H

#include "svg.h"
#include <wx/dynarray.h>

/** Base class for canvas items */
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
	
    /** returns the bounding box of the item */
    virtual wxSVGRect GetBBox(const wxSVGMatrix& matrix = *(wxSVGMatrix*)NULL) { return wxSVGRect(); }
    virtual wxSVGRect GetResultBBox(const wxCSSStyleDeclaration& style,
      const wxSVGMatrix& matrix = *(wxSVGMatrix*)NULL) { return GetBBox(); }
	
  protected:
	wxSVGCanvasItemType m_type;
};

/** Canvas item, that saves a graphic path (SVGPathElement) and
  * and other elements that can be converted to a path (SVGRectElement, etc.)
  */
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
	bool m_fill; /* define, if a path can be filled (disabled for line) */
	double m_curx, m_cury, m_cubicx, m_cubicy, m_quadx, m_quady, m_begx, m_begy;
	virtual void MoveToImpl(double x, double y) = 0;
	virtual void LineToImpl(double x, double y) = 0;
	virtual void CurveToCubicImpl(double x1, double y1, double x2, double y2, double x, double y) = 0;
	virtual bool ClosePathImpl() = 0;
};

/** character */
struct wxSVGCanvasTextChar
{
  wxSVGCanvasPath* path;
  wxSVGRect bbox;
};
WX_DECLARE_OBJARRAY(wxSVGCanvasTextChar, wxSVGCanvasTextCharList);

/** text-chunk */
struct wxSVGCanvasTextChunk
{
  wxSVGCanvasTextCharList chars;
  wxCSSStyleDeclaration style;
  wxSVGMatrix matrix;
  wxSVGRect GetBBox(const wxSVGMatrix& matrix);
  wxSVGRect GetBBox() { return GetBBox(*(wxSVGMatrix*)NULL); }
};

WX_DECLARE_OBJARRAY(wxSVGCanvasTextChunk, wxSVGCanvasTextChunkList);

/** Canvas item, that saves text (SVGTextElement) as list of chunks */
class wxSVGCanvasText: public wxSVGCanvasItem
{
  public:
	wxSVGCanvasText(wxSVGCanvas* canvas);
	virtual ~wxSVGCanvasText();
	
	virtual void Init(wxSVGTextElement& element, const wxCSSStyleDeclaration& style);
    virtual wxSVGRect GetBBox(const wxSVGMatrix& matrix = *(wxSVGMatrix*)NULL);
	virtual long GetNumberOfChars();
    virtual double GetComputedTextLength();
    virtual double GetSubStringLength(unsigned long charnum, unsigned long nchars);
    virtual wxSVGPoint GetStartPositionOfChar(unsigned long charnum);
    virtual wxSVGPoint GetEndPositionOfChar(unsigned long charnum);
    virtual wxSVGRect GetExtentOfChar(unsigned long charnum);
    virtual double GetRotationOfChar(unsigned long charnum);
    virtual long GetCharNumAtPosition(const wxSVGPoint& point);
	
  public:
    wxSVGCanvasTextChunkList m_chunks; /** list of text-chunks */
	wxSVGCanvasTextChar* m_char; /** current char */
  
  protected:
    wxSVGCanvas* m_canvas;
    double m_tx, m_ty; /** current text position */
    wxCSS_VALUE m_textAnchor; /** current text anchor */
	int m_textAnchorBeginIndex; /** index of first chunk with current text anchor */
	double m_textAnchorBeginPos; /** x-coordinate of text with current text anchor */
	wxCSS_VALUE m_dominantBaseline; /** current dominant baseline */
    int m_dominantBaselineBeginIndex; /** index of first chunk with current baseline */
	virtual void Init(wxSVGTSpanElement& element, const wxCSSStyleDeclaration& style);
	virtual void InitChildren(wxSVGTextPositioningElement& element, const wxCSSStyleDeclaration& style);
	virtual void BeginChunk(const wxCSSStyleDeclaration& style);
	virtual void BeginChar();
	virtual void EndChar();
	virtual void EndTextAnchor();
	wxSVGCanvasTextChunk* getChunk(unsigned long& charnum);
    /** Converts text in path and saves in current chunk (m_chunk->path) */
    virtual void InitText(const wxString& text, const wxCSSStyleDeclaration& style) = 0;
};

/** Canvas item, that saves image (SVGImageElement) */
class wxSVGCanvasImage: public wxSVGCanvasItem
{
  public:
	wxSVGCanvasImage(): wxSVGCanvasItem(wxSVG_CANVAS_ITEM_IMAGE) {}
	virtual ~wxSVGCanvasImage() {}
	virtual void Init(wxSVGImageElement& element);
    inline int GetDefaultWidth() { return m_image.GetWidth(); }
    inline int GetDefaultHeight() { return m_image.GetHeight(); }
  
  public:
	double m_x, m_y, m_width, m_height; /** position and size of image */
    wxString m_href; /** link to the image (filename) */
	wxImage m_image; /** image data */
};

#endif // WX_SVG_CANVAS_ITEM_H
