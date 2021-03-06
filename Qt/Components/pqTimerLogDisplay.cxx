/*=========================================================================

   Program: ParaView
   Module:    $RCSfile: pqTimerLogDisplay.cxx,v $

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

#include "pqTimerLogDisplay.h"

#include "pqApplicationCore.h"
#include "pqFileDialog.h"
#include "pqSettings.h"

#include "vtkClientServerStream.h"
#include "vtkProcessModule.h"
#include "vtkProcessModuleConnectionManager.h"
#include "vtkPVTimerInformation.h"
#include "vtkSmartPointer.h"

#include "QFile"
#include "QPair"
#include "QTextStream"

#include "ui_pqTimerLogDisplay.h"
class pqTimerLogDisplayUi : public Ui::pqTimerLogDisplay {};

//-----------------------------------------------------------------------------
typedef struct
{
  QString name;
  float value;
} pqTimerLogDisplayFloatChoices;

typedef struct
{
  QString name;
  float value;
} pqTimerLogDisplayIntChoices;

static pqTimerLogDisplayFloatChoices ThresholdChoices[] = {
  { "Show All", 0.0f },
  { "0.001", 0.001f },
  { "0.01", 0.01f },
  { "0.1", 0.1f }
};
static const int NumThresholdChoices
  = sizeof(ThresholdChoices)/sizeof(pqTimerLogDisplayFloatChoices);

static pqTimerLogDisplayIntChoices LengthChoices[] = {
  { "100", 100 },
  { "500", 500 },
  { "1000", 1000 },
  { "5000", 5000 }
};
static const int NumLengthChoices
  = sizeof(LengthChoices)/sizeof(pqTimerLogDisplayIntChoices);

//-----------------------------------------------------------------------------
pqTimerLogDisplay::pqTimerLogDisplay(QWidget *p)
  : QDialog(p)
{
  this->ui = new pqTimerLogDisplayUi;
  this->ui->setupUi(this);

  int i;
  for (i = 0; i < NumThresholdChoices; i++)
    {
    this->ui->timeThreshold->addItem(ThresholdChoices[i].name);
    }
  for (i = 0; i < NumLengthChoices; i++)
    {
    this->ui->bufferLength->addItem(LengthChoices[i].name);
    }

  connect(this->ui->refreshButton, SIGNAL(clicked(bool)),
          this, SLOT(refresh()));
  connect(this->ui->clearButton, SIGNAL(clicked(bool)),
          this, SLOT(clear()));
  connect(this->ui->timeThreshold, SIGNAL(activated(int)),
          this, SLOT(setTimeThresholdById(int)));
  connect(this->ui->bufferLength, SIGNAL(activated(int)),
          this, SLOT(setBufferLengthById(int)));
  connect(this->ui->enable, SIGNAL(toggled(bool)),
          this, SLOT(setEnable(bool)));
  connect(this->ui->saveButton, SIGNAL(clicked(bool)),
          this, SLOT(save()));

  this->setTimeThreshold(0.01f);
  this->setBufferLength(500);
  this->setEnable(true);

  this->restoreState();
}

pqTimerLogDisplay::~pqTimerLogDisplay()
{
  delete this->ui;
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::refresh()
{
  this->ui->log->clear();

  vtkProcessModule *pm = vtkProcessModule::GetProcessModule();

  vtkSmartPointer<vtkPVTimerInformation> timerInfo
    = vtkSmartPointer<vtkPVTimerInformation>::New();

  // Get information about the local process.
  timerInfo->CopyFromObject(pm);
  this->addToLog("Local Process", timerInfo);

  // Get information about servers.
  if (pm->GetClientMode())
    {
    // TODO: When ParaView supports connections to multiple servers, we should
    // report timing on all of them.
    vtkIdType connectionId =
      vtkProcessModuleConnectionManager::GetRootServerConnectionID();

    // Clear out information by creating a new info object.
    timerInfo = vtkSmartPointer<vtkPVTimerInformation>::New();
    pm->GatherInformation(connectionId, vtkProcessModule::RENDER_SERVER,
                          timerInfo, pm->GetProcessModuleID());
    if (!pm->GetRenderClientMode(connectionId))
      {
      this->addToLog("Server", timerInfo);
      }
    else
      {
      this->addToLog("Render Server", timerInfo);

      // We just reported on the render server.  Now report on the data server.
      timerInfo = vtkSmartPointer<vtkPVTimerInformation>::New();
      pm->GatherInformation(connectionId, vtkProcessModule::DATA_SERVER,
                            timerInfo, pm->GetProcessModuleID());
      this->addToLog("Data Server", timerInfo);
      }
    }
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::addToLog(const QString &source,
                                 vtkPVTimerInformation *timerInfo)
{
  this->ui->log->insertHtml("<p><hr><p>");

  int numLogs = timerInfo->GetNumberOfLogs();
  for (int id = 0; id < numLogs; id++)
    {
    if (numLogs > 1)
      {
      this->ui->log->insertHtml(QString("<br><p><h1>%1, Process %2</h1><p>")
                                .arg(source).arg(id));
      }
    else
      {
      this->ui->log->insertHtml(QString("<br><p><h1>%1</h1><p>").arg(source));
      }
    this->ui->log->insertHtml(QString("<pre>%1</pre>").arg(timerInfo->GetLog(id)));
    }
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::clear()
{
  vtkProcessModule *pm = vtkProcessModule::GetProcessModule();
  vtkClientServerStream stream;
  stream << vtkClientServerStream::Invoke
         << pm->GetProcessModuleID() << "ResetLog"
         << vtkClientServerStream::End;
  // TODO: Reset timer on all servers.
  pm->SendStream(vtkProcessModuleConnectionManager::GetRootServerConnectionID(),
                 vtkProcessModule::CLIENT_AND_SERVERS, stream);
  this->refresh();
}

//-----------------------------------------------------------------------------
float pqTimerLogDisplay::timeThreshold() const
{
  return ThresholdChoices[this->ui->timeThreshold->currentIndex()].value;
}

void pqTimerLogDisplay::setTimeThreshold(float value)
{
  for (int i = 0; i < NumThresholdChoices; i++)
    {
    if (ThresholdChoices[i].value == value)
      {
      this->setTimeThresholdById(i);
      return;
      }
    }
  qWarning("Invalid time threshold: %f", value);
}

void pqTimerLogDisplay::setTimeThresholdById(int id)
{

  this->ui->timeThreshold->setCurrentIndex(id);

  vtkProcessModule *pm = vtkProcessModule::GetProcessModule();
  vtkClientServerStream stream;
  stream << vtkClientServerStream::Invoke
         << pm->GetProcessModuleID()
         << "SetLogThreshold" << ThresholdChoices[id].value
         << vtkClientServerStream::End;
  // TODO: Set value on all servers
  pm->SendStream(vtkProcessModuleConnectionManager::GetRootServerConnectionID(),
                 vtkProcessModule::CLIENT_AND_SERVERS, stream);

  this->refresh();
}

//-----------------------------------------------------------------------------
int pqTimerLogDisplay::bufferLength() const
{
  return qRound(LengthChoices[this->ui->bufferLength->currentIndex()].value);
}

void pqTimerLogDisplay::setBufferLength(int value)
{
  for (int i = 0; i < NumLengthChoices; i++)
    {
    if (LengthChoices[i].value == value)
      {
      this->setBufferLengthById(i);
      return;
      }
    }
  qWarning("Invalid buffer length: %d", value);
}

void pqTimerLogDisplay::setBufferLengthById(int id)
{
  this->ui->bufferLength->setCurrentIndex(id);

  vtkProcessModule *pm = vtkProcessModule::GetProcessModule();
  vtkClientServerStream stream;
  stream << vtkClientServerStream::Invoke
         << pm->GetProcessModuleID()
         << "SetLogBufferLength" << 2*LengthChoices[id].value
         << vtkClientServerStream::End;
  // TODO: Set value on all servers
  pm->SendStream(vtkProcessModuleConnectionManager::GetRootServerConnectionID(),
                 vtkProcessModule::CLIENT_AND_SERVERS, stream);
}

//-----------------------------------------------------------------------------
bool pqTimerLogDisplay::isEnabled() const
{
  return this->ui->enable->isChecked();
}

void pqTimerLogDisplay::setEnable(bool state)
{
  this->ui->enable->setChecked(state);

  vtkProcessModule *pm = vtkProcessModule::GetProcessModule();
  vtkClientServerStream stream;
  stream << vtkClientServerStream::Invoke
         << pm->GetProcessModuleID() << "SetEnableLog" << (int)state
         << vtkClientServerStream::End;
  // TODO: Set value on all servers
  pm->SendStream(vtkProcessModuleConnectionManager::GetRootServerConnectionID(),
                 vtkProcessModule::CLIENT_AND_SERVERS, stream);
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::save()
{
  QString filters;
  filters += "Text Files (*.txt)";
  filters += ";;All files (*)";

 pqFileDialog *const fileDialog = new pqFileDialog(NULL,
                                                   this,
                                                   tr("Save Timer Log"),
                                                   QString(),
                                                   filters);
  fileDialog->setAttribute(Qt::WA_DeleteOnClose);
  fileDialog->setObjectName("TimerLogSaveDialog");
  fileDialog->setFileMode(pqFileDialog::AnyFile);
  connect(fileDialog, SIGNAL(filesSelected(const QStringList &)),
          this, SLOT(save(const QStringList &)));
  fileDialog->setModal(true);
  fileDialog->show();
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::save(const QStringList &files)
{
  for (int i = 0; i < files.size(); i++)
    {
    this->save(files[i]);
    }
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::save(const QString &filename)
{
  QFile file(filename);
  file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
  if (file.error() != QFile::NoError)
    {
    qWarning("Could not open %s for reading.", filename.toAscii().data());
    return;
    }
  QTextStream(&file) << this->ui->log->toPlainText();
  if (file.error() != QFile::NoError)
    {
    qWarning("Error writing to %s.", filename.toAscii().data());
    }
  file.close();
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::saveState()
{
  pqApplicationCore *core = pqApplicationCore::instance();
  if (core)
    {
    pqSettings *settings = core->settings();
    settings->saveState(*this, "TimerLog");
    settings->beginGroup("TimerLog");
    settings->setValue("TimeThreshold", this->timeThreshold());
    settings->setValue("BufferLength", this->bufferLength());
    settings->setValue("Enable", this->isEnabled());
    settings->endGroup();
    }
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::restoreState()
{
  pqApplicationCore *core = pqApplicationCore::instance();
  if (core)
    {
    pqSettings *settings = core->settings();

    settings->restoreState("TimerLog", *this);

    settings->beginGroup("TimerLog");
    this->setTimeThreshold(settings->value("TimeThreshold",
                                           this->timeThreshold()).toDouble());
    this->setBufferLength(settings->value("BufferLength",
                                          this->bufferLength()).toInt());
    this->setEnable(settings->value("Enable",
                                    this->isEnabled()).toBool());
    settings->endGroup();
    }
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::showEvent(QShowEvent *e)
{
  this->restoreState();
  this->Superclass::showEvent(e);
}

//-----------------------------------------------------------------------------
void pqTimerLogDisplay::hideEvent(QHideEvent *e)
{
  this->saveState();
  this->Superclass::hideEvent(e);
}
