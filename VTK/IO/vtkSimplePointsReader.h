/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkSimplePointsReader.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkSimplePointsReader - Read a list of points from a file.
// .SECTION Description
// vtkSimplePointsReader is a source object that reads a list of
// points from a file.  Each point is specified by three
// floating-point values in ASCII format.  There is one point per line
// of the file.  A vertex cell is created for each point in the
// output.  This reader is meant as an example of how to write a
// reader in VTK.

#ifndef __vtkSimplePointsReader_h
#define __vtkSimplePointsReader_h

#include "vtkPolyDataAlgorithm.h"

class VTK_IO_EXPORT vtkSimplePointsReader : public vtkPolyDataAlgorithm
{
public:
  static vtkSimplePointsReader* New();
  vtkTypeRevisionMacro(vtkSimplePointsReader,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get the name of the file from which to read points.
  vtkSetStringMacro(FileName);
  vtkGetStringMacro(FileName);

protected:
  vtkSimplePointsReader();
  ~vtkSimplePointsReader();

  char* FileName;

  int RequestData(vtkInformation*,
                  vtkInformationVector**,
                  vtkInformationVector*);
private:
  vtkSimplePointsReader(const vtkSimplePointsReader&);  // Not implemented.
  void operator=(const vtkSimplePointsReader&);  // Not implemented.
};

#endif
