##############################################################################
## Name:        interface.py
## Purpose:     
## Author:      Alex Thuering
## Created:     2005/01/19
## RCS-ID:      $Id: interfaces.py,v 1.3 2005-05-16 11:00:17 ntalex Exp $
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
	  wxXmlElement(wxXML_ELEMENT_NODE, tagName) { m_doc = doc; }
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
inter.include_methods.append('\tvirtual wxSVGRect GetBBox() { return wxSVGRect(); }\n')
inter.include_methods.append('\tstatic wxSVGRect GetBBox(wxSVGElement* parent);\n')
inter.exclude_methods = ["GetBBox"]

# SVGTransformable
inter = interface()
interfaces["SVGTransformable"]=inter
inter.include_methods.append('\tvoid UpdateMatrix(wxSVGMatrix& matrix);\n')

# SVGStylable
inter = interface()
interfaces["SVGStylable"]=inter
inter.include_methods.append('\tinline void UpdateStyle(wxCSSStyleDeclaration& style) { style.Add(GetStyle()); }\n')

# SVGElementInstance
inter = interface()
interfaces["SVGElementInstance"]=inter
inter.include_methods.append('\twxSVGElementInstance() {}\n')
inter.include_methods.append('\tvirtual ~wxSVGElementInstance() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGLength
inter = interface()
interfaces["SVGLength"]=inter
inter.include_methods.append('\twxSVGLength() : m_unitType(wxSVG_LENGTHTYPE_UNKNOWN), m_value(0) {}\n')
inter.include_methods.append('\twxSVGLength(float v) : m_unitType(wxSVG_LENGTHTYPE_NUMBER), m_value(v) {}\n')
inter.include_methods.append('\tvirtual ~wxSVGLength() {}\n')
inter.include_methods.append('\tinline operator float() { return GetValue(); }\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGAngle
inter = interface()
interfaces["SVGAngle"]=inter
inter.include_methods.append('\twxSVGAngle() : m_unitType(wxSVG_ANGLETYPE_UNKNOWN), m_value(0) {}\n')
inter.include_methods.append('\twxSVGAngle(float v) : m_unitType(wxSVG_ANGLETYPE_DEG), m_value(v) {}\n')
inter.include_methods.append('\tvirtual ~wxSVGAngle() {}\n')
inter.include_methods.append('\tinline operator float() { return GetValue(); }\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGPoint
inter = interface()
interfaces["SVGPoint"]=inter
inter.include_methods.append('\twxSVGPoint(): m_x(0), m_y(0) {}\n')
inter.include_methods.append('\twxSVGPoint(float x, float y): m_x(x), m_y(y) {}\n')
inter.include_methods.append('\tvirtual ~wxSVGPoint() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGRect
inter = interface()
interfaces["SVGRect"]=inter
inter.include_methods.append('\twxSVGRect(): m_x(0), m_y(0), m_width(0), m_height(0) {}\n')
inter.include_methods.append('\twxSVGRect(float x, float y, float width, float height):\n\t  m_x(x), m_y(y), m_width(width), m_height(height) {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGMatrix
inter = interface()
interfaces["SVGMatrix"]=inter
inter.include_methods.append('\twxSVGMatrix(): m_a(1), m_b(0), m_c(0), m_d(1), m_e(0), m_f(0) {}\n')
inter.include_methods.append('\twxSVGMatrix(float a, float b, float c, float d, float e, float f):\n\t  m_a(a), m_b(b), m_c(c), m_d(d), m_e(e), m_f(f) {}\n')
inter.include_methods.append('\tvirtual ~wxSVGMatrix() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

# SVGTransform
inter = interface()
interfaces["SVGTransform"]=inter
inter.include_methods.append('\twxSVGTransform(const wxSVGMatrix& matrix): m_type(wxSVG_TRANSFORM_MATRIX), m_matrix(matrix), m_angle(0) {}\n')
inter.include_methods.append('\tvirtual ~wxSVGTransform() {}\n')
inter.include_methods.append('\tinline void SetMatrix(const wxSVGMatrix& n) { m_type = wxSVG_TRANSFORM_MATRIX; m_matrix = n; }\n')
inter.exclude_methods = ["SetMatrix"]
inter.user_defined_destructor=1

# SVGPathSeg
inter = interface()
interfaces["SVGPathSeg"]=inter
inter.include_methods.append('\twxSVGPathSeg(wxPATHSEG type = wxPATHSEG_UNKNOWN) { m_pathSegType = type; }\n')
inter.include_methods.append('\tvirtual ~wxSVGPathSeg() {}\n')
inter.user_defined_constructor=1
inter.user_defined_destructor=1

## container elements
for name in ["SVGSVGElement", "SVGGElement", "SVGDefsElement", "SVGUseElement",
"SVGAElement", "SVGSwitchElement", "SVGForeignObjectElement"]:
  inter = interface()
  interfaces[name]=inter
  inter.include_methods.append('\tvirtual wxSVGRect GetBBox() { return wxSVGLocatable::GetBBox(this); }\n')

## visible elements
for name in ["SVGLineElement", "SVGPolylineElement", "SVGPolygonElement",
"SVGRectElement", "SVGCircleElement", "SVGEllipseElement", "SVGPathElement",
"SVGTextElement"]: ##"SVGImageElement", "SVGClipPathElement"]:
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
  inter.include_methods.append('\twxSVGRect GetBBox();\n')
  inter.include_fwd_decls = ["SVGCanvasItem"]


# SVGDocument
inter = interface()
interfaces["SVGDocument"]=inter

inter.exclude_attributes = ["rootElement"]
inter.include_attributes.append('''
  protected:
	wxSVGCanvas* m_canvas;\n
''')
inter.include_methods.append('''\twxSVGDocument() { Init(); }
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
	
	wxImage Render(int width = -1, int height = -1);
''')
inter.include_fwd_decls = ["SVGSVGElement","SVGElement","SVGCanvas"]
inter.include_includes = ["<wx/image.h>"]
inter.user_defined_contructor=1
inter.user_defined_destructor=1

