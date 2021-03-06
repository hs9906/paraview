/*=========================================================================

   Program: ParaView
   Module:    $RCSfile: pqComboBoxDomain.h,v $

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
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

#ifndef pq_ComboBoxDomain_h
#define pq_ComboBoxDomain_h

#include <QObject>
#include "pqComponentsExport.h"

class QComboBox;
class vtkSMProperty;

/// combo box domain 
/// observers the domain for a combo box and updates accordingly
/// the list of values in the combo box is automatically 
/// updated when the domain changes
class PQCOMPONENTS_EXPORT pqComboBoxDomain : public QObject
{
  Q_OBJECT
public:
  /// constructor requires a QComboBox, 
  /// and the property with the domain to observe
  /// optionally pass in a domainName if a specific one 
  /// needs to be watched
  pqComboBoxDomain(QComboBox* p, vtkSMProperty* prop,
                   const QString& domainName = QString());
  ~pqComboBoxDomain();

  // explicitly trigger a domain change.
  // simply calls internalDomainChanged();
  void forceDomainChanged() 
    { this->internalDomainChanged(); }

public slots:
  void domainChanged();
protected slots:
  void internalDomainChanged();
protected:
  class pqInternal;
  pqInternal* Internal;
};

#endif

