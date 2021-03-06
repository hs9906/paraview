/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkProp3DCollection.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkProp3DCollection.h"
#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkProp3DCollection, "$Revision: 1.7 $");
vtkStandardNewMacro(vtkProp3DCollection);

//----------------------------------------------------------------------------
void vtkProp3DCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
