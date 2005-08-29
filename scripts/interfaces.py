##############################################################################
## Name:        interface.py
## Purpose:     
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: interfaces.py,v 1.11.2.2 2005-08-29 15:00:52 etisserant Exp $
## Copyright:   (c) 2005 Alex Thuering
## Notes:		some modules adapted from svgl project
##############################################################################

class interface:
    def __init__(self):
        self.exclude_methods = []
        self.exclude_attributes = []
        self.include_methods = []
        self.include_attributes = []
        self.include_attributes_init = []
        self.include_get_set_attributes = []
        self.include_includes = []
        self.include_fwd_decls = []
        self.user_defined_constructor=0
        self.user_defined_destructor=0

interfaces={}

#SVGElement
inter = interface()
interfaces["SVGElement"]=inter
inter.include_attributes.append('''
  protected:
    wxSVGDocument* m_doc;
	
  public:
	wxSVGElement(wxSVGDocument* doc, wxString tagName = wxT("")):
	  wxXmlElement(wxXML_ELEMENT_NODE, tagName),
      m_ownerSVGElement(NULL), m_viewportElement(NULL) { m_doc = doc; }
	virtual ~wxSVGElement() {}
	
	virtual const wxSVGDTD GetDtd() const = 0;
	virtual void AddProperty(const wxString& name, const wxString& value)
	  { SetAttribute(name, value); }
''')
inter.include_fwd_decls=["SVGSVGElement", "SVGDocument"]
inter.include_includes=["SVGDTD"]
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGLocatable
inter = interface()
interfaces["SVGLocatable"]=inter
inter.include_get_set_attributes = [["wxSVGMatrix", "screenCTM", False, False]]
inter.include_methods.append('    virtual wxSVGRect GetBBox() { return wxSVGRect(); }\n')
inter.include_methods.append('    static wxSVGRect GetBBox(wxSVGElement* parent);\n')
inter.include_methods.append('    virtual wxSVGMatrix GetCTM(){ return wxSVGMatrix(); }\n')
inter.include_methods.append('    static wxSVGMatrix GetCTM(wxSVGElement* obj);\n')
inter.exclude_methods = ["GetBBox","GetCTM"]

# SVGTransformable
inter = interface()
interfaces["SVGTransformable"]=inter
inter.include_methods.append('    void Transform(const wxSVGMatrix& matrix);\n')
inter.include_methods.append('    void Translate(double tx, double ty);\n')
inter.include_methods.append('    void Scale(double s);\n')
inter.include_methods.append('    void Scale(double sx, double sy);\n')
inter.include_methods.append('    void Rotate(double angle, double cx = 0, double cy = 0);\n')
inter.include_methods.append('    void SkewX(double angle);\n')
inter.include_methods.append('    void SkewY(double angle);\n')
inter.include_methods.append('    void UpdateMatrix(wxSVGMatrix& matrix);\n')

# SVGFitToViewBox
#inter = interface()
#interfaces["SVGFitToViewBox"]=inter
#inter.include_methods.append('    void UpdateMatrix(wxSVGMatrix& matrix);\n')

# SVGStylable
inter = interface()
interfaces["SVGStylable"]=inter
inter.include_methods.append('    inline void UpdateStyle(wxCSSStyleDeclaration& style) { style.Add(GetStyle()); }\n')

# SVGElementInstance
inter = interface()
interfaces["SVGElementInstance"]=inter
inter.include_methods.append('    wxSVGElementInstance() {}\n')
inter.include_methods.append('    virtual ~wxSVGElementInstance() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# wxSVGColor
inter = interface()
interfaces["SVGColor"]=inter
inter.include_methods.append('    wxSVGColor(): m_colorType(wxSVG_COLORTYPE_UNKNOWN) {}\n')
inter.include_methods.append('    wxSVGColor(unsigned char r, unsigned char g, unsigned char b):\n')
inter.include_methods.append('      m_colorType(wxSVG_COLORTYPE_RGBCOLOR), m_rgbColor(r, g, b) {}\n')
inter.include_methods.append('    virtual ~wxSVGColor() {}\n')
inter.include_methods.append('    wxCSSValue* Clone() const { return new wxSVGColor(*this); }\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    wxString GetCSSText() const;\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    inline const wxRGBColor& GetRGBColor() const { return m_rgbColor; }\n')
inter.include_methods.append('    virtual void SetRGBColor(const wxRGBColor& rgbColor);\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    inline const wxSVGICCColor& GetICCColor() const { return m_iccColor; }\n')
inter.include_methods.append('    virtual void SetICCColor(const wxSVGICCColor& iccColor);\n')
inter.include_methods.append('    \n')
inter.exclude_methods = ["GetRgbColor", "SetRgbColor", "GetIccColor", "SetIccColor"]
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# wxSVGPaint
inter = interface()
interfaces["SVGPaint"]=inter
inter.include_methods.append('    wxSVGPaint(): m_paintType(wxSVG_PAINTTYPE_NONE) {}\n')
inter.include_methods.append('    wxSVGPaint(unsigned char r, unsigned char g, unsigned char b):\n')
inter.include_methods.append('      wxSVGColor(r, g, b), m_paintType(wxSVG_PAINTTYPE_RGBCOLOR) {}\n')
inter.include_methods.append('    virtual ~wxSVGPaint() {}\n')
inter.include_methods.append('    wxCSSValue* Clone() const { return new wxSVGPaint(*this); }\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    wxString GetCSSText() const;\n')
inter.include_methods.append('    inline const wxString& GetUri() const { return m_uri; }\n')
inter.include_methods.append('    virtual void SetUri(const wxString& uri);\n')
inter.include_methods.append('    virtual void SetRGBColor(const wxRGBColor& rgbColor);\n')
inter.include_methods.append('    virtual void SetICCColor(const wxSVGICCColor& iccColor);\n')
inter.include_methods.append('    \n')
inter.include_methods.append('''    inline bool Ok() const
    {
	  return m_paintType != wxSVG_PAINTTYPE_UNKNOWN &&
	         m_paintType != wxSVG_PAINTTYPE_NONE;
	}\n''')
inter.include_methods.append('    \n')
inter.exclude_methods = ["GetUri", "SetUri"]
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGLength
inter = interface()
interfaces["SVGLength"]=inter
inter.include_methods.append('    wxSVGLength() : m_unitType(wxSVG_LENGTHTYPE_UNKNOWN), m_value(0), m_valueInSpecifiedUnits(0) {}\n')
inter.include_methods.append('    wxSVGLength(double v) : m_unitType(wxSVG_LENGTHTYPE_NUMBER), m_value(v), m_valueInSpecifiedUnits(v) {}\n')
inter.include_methods.append('    wxSVGLength(wxSVG_LENGTHTYPE unitType, double v): m_unitType(unitType) { SetValueInSpecifiedUnits(v); }\n')
inter.include_methods.append('    wxSVGLength(const wxSVGLength& l): m_unitType(l.m_unitType), m_value(l.m_value), m_valueInSpecifiedUnits(l.m_valueInSpecifiedUnits) {}\n')
inter.include_methods.append('    virtual ~wxSVGLength() {}\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    inline double GetValue() const { return m_value; }\n')
inter.include_methods.append('    inline void SetValue(double n) { m_unitType = wxSVG_LENGTHTYPE_NUMBER; m_valueInSpecifiedUnits = n; m_value = n; }\n')
inter.include_methods.append('    inline operator double() const { return GetValue(); }\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    double GetValueInSpecifiedUnits() const { return m_valueInSpecifiedUnits; }\n')
inter.include_methods.append('    void SetValueInSpecifiedUnits(double n);\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    wxString GetValueAsString() const;\n')
inter.include_methods.append('    void SetValueAsString(const wxString& n);\n')
inter.include_methods.append('''    
    inline void ToViewportWidth(float viewportWidth) { m_value = m_valueInSpecifiedUnits*viewportWidth/100; }
	inline void ToViewportHeight(float viewportHeight) { m_value = m_valueInSpecifiedUnits*viewportHeight/100; }
	inline void ToViewportSize(float viewportWidth, float viewportHeight)
	{
	  m_value = m_valueInSpecifiedUnits*
		sqrt(viewportWidth*viewportWidth + viewportHeight*viewportHeight)/sqrt(2)/100;
	}\n
''')
inter.exclude_methods = ["GetValue", "SetValue", "GetValueInSpecifiedUnits", "GetValueInSpecifiedUnits"]
inter.exclude_attributes = ["valueAsString"]
inter.include_includes = ["String", "SVGLengthCalculate", "math"]
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGAngle
inter = interface()
interfaces["SVGAngle"]=inter
inter.include_methods.append('    wxSVGAngle() : m_unitType(wxSVG_ANGLETYPE_UNKNOWN), m_value(0) {}\n')
inter.include_methods.append('    wxSVGAngle(double v) : m_unitType(wxSVG_ANGLETYPE_UNSPECIFIED), m_value(v) {}\n')
inter.include_methods.append('    virtual ~wxSVGAngle() {}\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    inline double GetValue() const { return m_value; }\n')
inter.include_methods.append('    inline void SetValue(double n) { m_unitType = wxSVG_ANGLETYPE_UNSPECIFIED; m_valueInSpecifiedUnits = n; m_value = n; }\n')
inter.include_methods.append('    inline operator double() const { return GetValue(); }\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    inline double GetValueInSpecifiedUnits() const { return m_valueInSpecifiedUnits; }\n')
inter.include_methods.append('    void SetValueInSpecifiedUnits(double n);\n')
inter.include_methods.append('    \n')
inter.include_methods.append('    wxString GetValueAsString() const;\n')
inter.include_methods.append('    void SetValueAsString(const wxString& n);\n')
inter.include_methods.append('    \n')
inter.exclude_methods = ["GetValue", "SetValue", "GetValueInSpecifiedUnits", "GetValueInSpecifiedUnits"]
inter.exclude_attributes = ["valueAsString"]
inter.include_includes = ["String"]
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGPoint
inter = interface()
interfaces["SVGPoint"]=inter
inter.include_methods.append('    wxSVGPoint(): m_x(0), m_y(0) {}\n')
inter.include_methods.append('    wxSVGPoint(double x, double y): m_x(x), m_y(y) {}\n')
inter.include_methods.append('    virtual ~wxSVGPoint() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGRect
inter = interface()
interfaces["SVGRect"]=inter
inter.include_methods.append('    wxSVGRect(): m_x(0), m_y(0), m_width(0), m_height(0) {}\n')
inter.include_methods.append('    wxSVGRect(double x, double y, double width, double height):\n      m_x(x), m_y(y), m_width(width), m_height(height) {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGMatrix
inter = interface()
interfaces["SVGMatrix"]=inter
inter.include_methods.append('    wxSVGMatrix(): m_a(1), m_b(0), m_c(0), m_d(1), m_e(0), m_f(0) {}\n')
inter.include_methods.append('    wxSVGMatrix(double a, double b, double c, double d, double e, double f):\n      m_a(a), m_b(b), m_c(c), m_d(d), m_e(e), m_f(f) {}\n')
inter.include_methods.append('    virtual ~wxSVGMatrix() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGTransform
inter = interface()
interfaces["SVGTransform"]=inter
inter.include_methods.append('    wxSVGTransform(const wxSVGMatrix& matrix): m_type(wxSVG_TRANSFORM_MATRIX), m_matrix(matrix), m_angle(0) {}\n')
inter.include_methods.append('    virtual ~wxSVGTransform() {}\n')
inter.include_methods.append('    inline void SetMatrix(const wxSVGMatrix& n) { m_type = wxSVG_TRANSFORM_MATRIX; m_matrix = n; }\n')
inter.exclude_methods = ["SetMatrix"]
inter.user_defined_destructor=1

# SVGPathSeg
inter = interface()
interfaces["SVGPathSeg"]=inter
inter.include_methods.append('    wxSVGPathSeg(wxPATHSEG type = wxPATHSEG_UNKNOWN) { m_pathSegType = type; }\n')
inter.include_methods.append('    virtual ~wxSVGPathSeg() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

## container elements
for name in ["SVGSVGElement", "SVGGElement", "SVGDefsElement", "SVGUseElement",
"SVGAElement", "SVGSwitchElement", "SVGForeignObjectElement"]:
  inter = interface()
  interfaces[name]=inter
  inter.include_methods.append('    virtual wxSVGRect GetBBox() { return wxSVGLocatable::GetBBox(this); }\n')

## visible elements
for name in ["SVGLineElement", "SVGPolylineElement", "SVGPolygonElement",
"SVGRectElement", "SVGCircleElement", "SVGEllipseElement", "SVGPathElement",
"SVGImageElement"]: ##, "SVGClipPathElement"]:
  inter = interface()
  interfaces[name]=inter
  inter.include_attributes.append('''
  protected:
	wxSVGCanvasItem* m_canvasItem;
  public:
	inline wxSVGCanvasItem* GetCanvasItem() { return m_canvasItem; }
	inline void SetCanvasItem(wxSVGCanvasItem* canvasItem) { m_canvasItem = canvasItem; }\n
''')
  inter.include_attributes_init = [["canvasItem", True]]
  inter.include_methods.append('    wxSVGRect GetBBox();\n')
  inter.include_methods.append('    wxSVGMatrix GetCTM(){ return wxSVGLocatable::GetCTM(this); }\n')
  inter.include_fwd_decls = ["SVGCanvasItem"]


inter = interface()
interfaces["SVGTextElement"]=inter
inter.include_attributes.append('''
  protected:
    wxSVGCanvasItem* m_canvasItem;
  public:
    inline wxSVGCanvasItem* GetCanvasItem() { return m_canvasItem; }
    inline void SetCanvasItem(wxSVGCanvasItem* canvasItem) { m_canvasItem = canvasItem; }\n
''')
inter.include_attributes_init = [["canvasItem", True]]
inter.include_methods.append('''    wxSVGRect GetBBox();
    long GetNumberOfChars();
    double GetComputedTextLength();
    double GetSubStringLength(unsigned long charnum, unsigned long nchars);
    wxSVGPoint GetStartPositionOfChar(unsigned long charnum);
    wxSVGPoint GetEndPositionOfChar(unsigned long charnum);
    wxSVGRect GetExtentOfChar(unsigned long charnum);
    double GetRotationOfChar(unsigned long charnum);
    long GetCharNumAtPosition(const wxSVGPoint& point);
''')
inter.include_fwd_decls = ["SVGCanvasItem"]

# SVGDocument
inter = interface()
interfaces["SVGDocument"]=inter

inter.exclude_attributes = ["rootElement"]
inter.include_attributes.append('''
  protected:
	wxSVGCanvas* m_canvas;\n
''')
inter.include_methods.append('''    wxSVGDocument() { Init(); }
	wxSVGDocument(const wxString& filename, const wxString& encoding = wxT("UTF-8")):
	  wxXmlDocument(filename, encoding) { Init(); }
	wxSVGDocument(wxInputStream& stream, const wxString& encoding = wxT("UTF-8")):
	  wxXmlDocument(stream, encoding) { Init(); }
	virtual ~wxSVGDocument();
	
	void Init();
	inline wxSVGCanvas* GetCanvas() { return m_canvas; }
	
	wxXmlElement* CreateElement(const wxString& tagName);
	wxXmlElement* CreateElementNS(const wxString& namespaceURI, const wxString& qualifiedName);
	
	wxSVGSVGElement* GetRootElement() { return (wxSVGSVGElement*) GetRoot(); }
	void SetRootElement(wxSVGSVGElement* n) { SetRoot((wxXmlElement*) n); }
    
    wxSVGElement* GetElementById(const wxString& id);
	
	wxImage Render(int width = -1, int height = -1);
''')
inter.include_fwd_decls = ["SVGSVGElement","SVGElement","SVGCanvas"]
inter.include_includes = ["<wx/image.h>"]
inter.user_defined_contructor=1
inter.user_defined_destructor=1

