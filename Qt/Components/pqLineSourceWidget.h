/*=========================================================================

   Program:   ParaQ
   Module:    $RCSfile: pqLineSourceWidget.h,v $

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaQ is a free software; you can redistribute it and/or modify it
   under the terms of the ParaQ license version 1.1. 

   See License_v1.1.txt for the full ParaQ license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef _pqLineSourceWidget_h
#define _pqLineSourceWidget_h

#include "pqLineWidget.h"

class pqServer;

/// Provides a complete Qt UI for working with a 3D line widget
class pqLineSourceWidget :
  public pqLineWidget
{
  typedef pqLineWidget Superclass;
  
  Q_OBJECT
  
public:
  pqLineSourceWidget(vtkSMProxy* o, vtkSMProxy* pxy, QWidget* p = 0);
  ~pqLineSourceWidget();

  void setControlledProperties(vtkSMProperty* point1, vtkSMProperty* point2, vtkSMProperty* resolution);

  /// Subclasses can override this method to map properties to
  /// GUI. Default implementation updates the internal datastructures
  /// so that default implementations can be provided for 
  /// accept/reset.
  virtual void setControlledProperty(const char* function,
    vtkSMProperty * controlled_property);
private:
  class pqImplementation;
  pqImplementation* const Implementation;
};

#endif