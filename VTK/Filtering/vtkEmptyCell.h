/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkEmptyCell.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkEmptyCell - an empty cell used as a place-holder during processing
// .SECTION Description
// vtkEmptyCell is a concrete implementation of vtkCell. It is used
// during processing to represented a deleted element.

#ifndef __vtkEmptyCell_h
#define __vtkEmptyCell_h

#include "vtkCell.h"

class VTK_FILTERING_EXPORT vtkEmptyCell : public vtkCell
{
public:
  static vtkEmptyCell *New();
  vtkTypeRevisionMacro(vtkEmptyCell,vtkCell);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // See the vtkCell API for descriptions of these methods.
  int GetCellType() {return VTK_EMPTY_CELL;};
  int GetCellDimension() {return 0;};
  int GetNumberOfEdges() {return 0;};
  int GetNumberOfFaces() {return 0;};
  vtkCell *GetEdge(int) {return 0;};
  vtkCell *GetFace(int) {return 0;};
  int CellBoundary(int subId, double pcoords[3], vtkIdList *pts);
  void Contour(double value, vtkDataArray *cellScalars,
               vtkPointLocator *locator, vtkCellArray *verts1,
               vtkCellArray *lines, vtkCellArray *verts2,
               vtkPointData *inPd, vtkPointData *outPd,
               vtkCellData *inCd, vtkIdType cellId, vtkCellData *outCd);
  void Clip(double value, vtkDataArray *cellScalars,
            vtkPointLocator *locator, vtkCellArray *pts,
            vtkPointData *inPd, vtkPointData *outPd,
            vtkCellData *inCd, vtkIdType cellId, vtkCellData *outCd,
            int insideOut);

  int EvaluatePosition(double x[3], double* closestPoint,
                       int& subId, double pcoords[3],
                       double& dist2, double *weights);
  void EvaluateLocation(int& subId, double pcoords[3], double x[3],
                        double *weights);
  int IntersectWithLine(double p1[3], double p2[3], double tol, double& t,
                        double x[3], double pcoords[3], int& subId);
  int Triangulate(int index, vtkIdList *ptIds, vtkPoints *pts);
  void Derivatives(int subId, double pcoords[3], double *values,
                   int dim, double *derivs);

  // Description:
  // Compute the interpolation functions/derivatives
  // (aka shape functions/derivatives)
  virtual void InterpolateFunctions(double pcoords[3], double *weights);
  virtual void InterpolateDerivs(double pcoords[3], double *derivs);

protected:
  vtkEmptyCell() {};
  ~vtkEmptyCell() {};

private:
  vtkEmptyCell(const vtkEmptyCell&);  // Not implemented.
  void operator=(const vtkEmptyCell&);  // Not implemented.
};

#endif


