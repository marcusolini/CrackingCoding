// Copyright(c) 2018 marcusolini@outlook.com

// LeakTest.cpp : Defines the entry point for the console application.
//

// https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/finding-a-user-mode-memory-leak

#include "stdafx.h"

#include "log.h"
#include "createlogger.h"

int main()
{
     DWORD dwStatus = ERROR_SUCCESS;
     DWORD dwReturnStatus = ERROR_SUCCESS;

     size_t leakNewCalls = 1000000000000000000;
     size_t leakNewBytes = 10000;

     size_t leakMallocCalls = 1000000000000000000;
     size_t leakMallocBytes = 20000;

     size_t leakCallocCalls = 100000000000000;
     size_t leakCallocBytes = 40000;

     size_t leakHandleCalls = 1000;

     system("pause");

     std::shared_ptr< CLogger < CConsoleLogPolicy > > consoleLogger = CCreateLogger::CreateConsoleLogger();
     std::shared_ptr< CLogger < CFileLogPolicy > > fileLogger = CCreateLogger::CreateFileLogger("C:\\Projects\\Academics\\Leaks\\LeakTest\\leaktest.log");

     try
     {
          dwStatus = CLeakLib::LeakNewMemory(leakNewCalls, leakNewBytes);

          if (SUCCEEDED(dwStatus))
          {
               LOG_ERROR("New Memory Failed.");
               dwReturnStatus += dwStatus;
          }

          dwStatus = CLeakLib::LeakMallocMemory(leakMallocCalls, leakMallocBytes);

          if (SUCCEEDED(dwStatus))
          {
               LOG_ERROR("Malloc Memory Failed.");
               dwReturnStatus += dwStatus;
          }

          dwStatus = CLeakLib::LeakCallocMemory(leakCallocCalls, leakCallocBytes);

          if (SUCCEEDED(dwStatus))
          {
               LOG_ERROR("Calloc Memory Failed.");
               dwReturnStatus += dwStatus;
          }
          
          dwStatus = CLeakLib::LeakHandle(leakHandleCalls);
          
          if (SUCCEEDED(dwStatus))
          {
               LOG_ERROR("LeakNewMemory Failed.");
               dwReturnStatus += dwStatus;
          }
     }
     catch ( std::bad_alloc )
     {
          dwStatus = E_OUTOFMEMORY;
          system("pause");
     }
     catch (...)
     {
          dwStatus = E_OUTOFMEMORY;
     }

     CCreateLogger::DeleteConsoleLogger(consoleLogger);
     CCreateLogger::DeleteFileLogger(fileLogger);

     system("pause");

     return dwReturnStatus;
}

