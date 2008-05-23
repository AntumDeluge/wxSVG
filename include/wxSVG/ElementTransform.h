//////////////////////////////////////////////////////////////////////////////
// Name:        ElementTransform.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: ElementTransform.h,v 1.2 2008-05-23 13:47:22 etisserant Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_TRANSFORM_H
#define ELEMENT_TRANSFORM_H

#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "math.h"
#include "SVGUIID.h"

void ClearList(void);
void ResetElementMatrix(wxSVGElement* elem);
void MoveElement(wxSVGElement* elem, double Xposition, double Yposition);
void MoveElementByCenter(wxSVGElement* elem, double Xposition, double Yposition);
void ScaleElement(wxSVGElement* elem, double Xscale, double Yscale);
void RotateElement(wxSVGElement* elem, double angle, wxSVGPoint* center=NULL);

#endif //ELEMENT_TRANSFORM_H
