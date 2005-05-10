//////////////////////////////////////////////////////////////////////////////
// Name:        SVGTransformable.cpp
// Purpose:     
// Author:      Alex Thuering
// Created:     2005/05/05
// RCS-ID:      $Id: SVGTransformable.cpp,v 1.1.1.1 2005-05-10 17:51:39 ntalex Exp $
// Copyright:   (c) 2005 Alex Thuering
// Licence:     wxWindows licence
//////////////////////////////////////////////////////////////////////////////

#include "SVGTransformable.h"
#include <wx/log.h>

void wxSVGTransformable::UpdateMatrix(wxSVGMatrix& matrix)
{
  wxSVGTransformList& transforms = GetTransform();
  for (int i=0; i<(int)transforms.Count(); i++)
	matrix = matrix.Multiply(transforms[i].GetMatrix());
}
