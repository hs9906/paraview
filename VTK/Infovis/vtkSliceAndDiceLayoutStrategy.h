/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkSliceAndDiceLayoutStrategy.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/*----------------------------------------------------------------------------
 Copyright (c) Sandia Corporation
 See Copyright.txt or http://www.paraview.org/HTML/Copyright.html for details.
----------------------------------------------------------------------------*/
// .NAME vtkSliceAndDiceLayoutStrategy - a horizontal and vertical slicing tree map layout
//
// .SECTION Description
// Lays out a tree-map alternating between horizontal and vertical slices,
// taking into account the relative size of each vertex.
//
// .SECTION Thanks
// Slice and dice algorithm comes from:
// Shneiderman, B. 1992. Tree visualization with tree-maps: 2-d space-filling approach. 
// ACM Trans. Graph. 11, 1 (Jan. 1992), 92-99. 

#ifndef __vtkSliceAndDiceLayoutStrategy_h
#define __vtkSliceAndDiceLayoutStrategy_h

#include "vtkTreeMapLayoutStrategy.h"

class VTK_INFOVIS_EXPORT vtkSliceAndDiceLayoutStrategy : public vtkTreeMapLayoutStrategy 
{
public:
  static vtkSliceAndDiceLayoutStrategy *New();

  vtkTypeRevisionMacro(vtkSliceAndDiceLayoutStrategy,vtkTreeMapLayoutStrategy);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // The field name associated with the size of the vertex.
  vtkGetStringMacro(SizeFieldName);
  vtkSetStringMacro(SizeFieldName);

  // Description:
  // Perform the layout of a tree and place the results as 4-tuples in
  // coordsArray (Xmin, Xmax, Ymin, Ymax).
  void Layout(vtkTree *inputTree, vtkDataArray *coordsArray);

protected:
  vtkSliceAndDiceLayoutStrategy();
  ~vtkSliceAndDiceLayoutStrategy();

private:
  vtkSliceAndDiceLayoutStrategy(const vtkSliceAndDiceLayoutStrategy&);  // Not implemented.
  void operator=(const vtkSliceAndDiceLayoutStrategy&);  // Not implemented.

  char * SizeFieldName;
};

#endif

