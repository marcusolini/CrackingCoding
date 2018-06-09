//Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include <QMessageBox>

#include <thread>
#include <sstream>

#include "progressdialog.h"
#include "ui_progressdialog.h"
#include "leakdialogqtui.h"

#include "LeakLib.h"

ProgressDialog::ProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProgressDialog)
{
    ui->setupUi(this);

    m_bThreadContinue = true;
    m_bThreadPause = false;

    // Marquee Progress Bar
    ui->RunTestsProgressBar->setMinimum(0);
    ui->RunTestsProgressBar->setMaximum(0);

    connect(ui->StopPushButton, SIGNAL (released()), this, SLOT(handleStopButton()));
    connect(ui->PausePushButton, SIGNAL (released()), this, SLOT(handlePauseButton()));

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parent);

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
        std::thread progressThread = std::thread(ProgressDialog::ProgressThreadFunction, this, &iTest);
        progressThread.detach();
        ui->RunTestsLabel->setText("Running Leak Tests...~~~~~");
    }
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::handleStopButton()
{
    bool bThreadAlreadyPaused = m_bThreadPause;
    if ( false == bThreadAlreadyPaused)
    {
        m_bThreadPause = true;
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(100);
        ui->RunTestsProgressBar->setValue(0);
    }

    QMessageBox msgBox;
    msgBox.setText("Stop");
    msgBox.setInformativeText("Stop Resource Leak Tests?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    int nRet = msgBox.exec();

    if ( QMessageBox::Yes == nRet)
    {
        m_bThreadContinue = false;
        //this->close();
    }
    else
    {
        if ( false == bThreadAlreadyPaused)
        {
            m_bThreadPause = false;
            ui->RunTestsProgressBar->setMinimum(0);
            ui->RunTestsProgressBar->setMaximum(0);
        }
    }

}

void ProgressDialog::handlePauseButton()
{
    QString sText = ui->PausePushButton->text();

    if (sText.toUpper().contains("PAUSE"))
    {
        m_bThreadPause = true;
        ui->RunTestsLabel->setText("Paused Leak Tests~~~~");
        ui->PausePushButton->setText("Resume~~~~");
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(100);
        ui->RunTestsProgressBar->setValue(0);
    }
    else
    {
        m_bThreadPause = false;
        ui->RunTestsLabel->setText("Running Leak Tests...~~~~~");
        ui->PausePushButton->setText("Pause~~~~");
        ui->RunTestsProgressBar->setMinimum(0);
        ui->RunTestsProgressBar->setMaximum(0);
    }
}


void ProgressDialog::Complete()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);

    bool bAllCompleted = true;

    LeakDialogQtUi* pLeakDialogQtUi = dynamic_cast<LeakDialogQtUi*>(parentWidget());

    for ( auto& iTest : pLeakDialogQtUi->gTests)
    {
        if ( false == iTest.IsComplete())
        {
            bAllCompleted = false;
            break;
        }
    }

    if (true == bAllCompleted)
    {
        this->close();
        //emit ui->StopPushButton->released();
    }
}

void ProgressDialog::ProgressThreadFunction(ProgressDialog* pProgressDialog, CResourceLeakTest* iTest)
{
    int nStatus = 0;
    std::wstringstream ss;
    uint64_t id = 0;

    QMessageBox msgBox;
    msgBox.setText("Failure");
    msgBox.setIcon(QMessageBox::Critical);

    iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::STARTED);

    ss << std::this_thread::get_id();
    id = std::stoull(ss.str());

    iTest->SetThreadId(id);

    std::this_thread::yield();

    for (std::size_t nIteration = 0;
         (nIteration < iTest->GetInterations()) && (pProgressDialog->m_bThreadContinue) && (0 == nStatus);
         nIteration++)
    {
        iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::RUNNING);

         switch (iTest->GetResourceAllocationType())
         {
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::NEW_OPERATOR:
              nStatus = CLeakLib::LeakNewMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  msgBox.setInformativeText("new operator failure.");
                  msgBox.exec();
              }
              break;
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::MALLOC_FUNCTION:
              nStatus = CLeakLib::LeakMallocMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  msgBox.setInformativeText("malloc function failure.");
                  msgBox.exec();
              }
              break;
         case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::CALLOC_FUNCTION:
              nStatus = CLeakLib::LeakCallocMemory(1, iTest->GetBytesPerIteration());
              if (0 != nStatus)
              {
                  msgBox.setInformativeText("calloc function failure.");
                  msgBox.exec();
              }
              break;
         //case CResourceLeakTest::ERESOURCE_ALLOCATION_TYPES::HANDLE_FUNCTION:
         //     nStatus = CLeakLib::LeakHandle(1);
         //     if (0 != nStatus)
         //     {
         //         msgBox.setInformativeText("handle function failure.");
         //         msgBox.exec();
         //     }
         //     break;
         default:
              msgBox.setInformativeText("Invalid allocation type.");
              msgBox.exec();
              break;
         }

         while (pProgressDialog->m_bThreadPause && pProgressDialog->m_bThreadContinue)
         {
              iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::PAUSED);
              std::this_thread::sleep_for(std::chrono::milliseconds{ 300 });
         }
    }

    if ( false == pProgressDialog->m_bThreadContinue)
    {
        iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::INTERRUPTED);
    }
    else
    {
        if ( 0 == nStatus)
        {
            iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::SUCCESS);
        }
        else
        {
            iTest->SetState(CResourceLeakTest::ERESOURCE_ALLOCATION_COMPLETED_STATE::FAILED);
        }
    }

    std::lock_guard<std::recursive_mutex> lock(pProgressDialog->m_mutex);

    iTest->SetIsComplete();
    pProgressDialog->Complete();
}