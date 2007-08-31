//////////////////////////////////////////////////////////////////////////////
// Name:        ElementTransform.h
// Purpose:     
// Author:      Laurent Bessard
// Created:     2005/07/28
// RCS-ID:      $Id: ElementTransform.h,v 1.1 2007-08-31 08:56:16 gusstdie Exp $
// Copyright:   (c) Laurent Bessard
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#ifndef ELEMENT_TRANSFORM_H
#define ELEMENT_TRANSFORM_H

#include <wxSVG/svg.h>
#include <wxSVG/SVGElement.h>
#include "math.h"

void InitElementMatrix(wxSVGElement* elem);
void ResetElementMatrix(wxSVGElement* elem);
void MoveElement(wxSVGElement* elem, double Xposition, double Yposition);
void MoveElementByCenter(wxSVGElement* elem, double Xposition, double Yposition);
void ScaleElement(wxSVGElement* elem, double Xscale, double Yscale);
void RotateElement(wxSVGElement* elem, double angle, wxSVGPoint* center=NULL);

#endif //ELEMENT_TRANSFORM_H
