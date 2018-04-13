// Copyright(c) 2018 marcusolini@outlook.com

// LeakLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include <malloc.h>
#include <new>
#include <string>
#include <thread>

#include "LeakLib.h"


/*static*/ DWORD CLeakLib::LeakNewMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{    
     DWORD dwStatus = ERROR_SUCCESS;

     char* pChars = nullptr;

     try
     {

          for (auto nCalls = 0; (nCalls < numberOfCalls) && (ERROR_SUCCESS == dwStatus); nCalls++)
          {
               pChars = new (std::nothrow) char[bytesEachCall];
               if (nullptr == pChars) dwStatus = ERROR_OUTOFMEMORY;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 2 });
          }

     }
     catch (std::bad_alloc)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }
     catch (...)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }

     return dwStatus;
}

/*static*/ DWORD CLeakLib::LeakMallocMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{
     DWORD dwStatus = ERROR_SUCCESS;

     void* pVoid = nullptr;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (ERROR_SUCCESS == dwStatus); nCalls++)
          {
               pVoid = malloc(bytesEachCall);
               if (nullptr == pVoid) dwStatus = ERROR_OUTOFMEMORY;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 3 });
          }
     }
     catch (std::bad_alloc)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }
     catch (...)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }

     return dwStatus;
}

/*static*/ DWORD CLeakLib::LeakCallocMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{
     DWORD dwStatus = ERROR_SUCCESS;

     void* pVoid = nullptr;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (ERROR_SUCCESS == dwStatus); nCalls++)
          {
               pVoid = calloc(1, bytesEachCall);
               if (nullptr == pVoid) dwStatus = ERROR_OUTOFMEMORY;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 5 });
          }
     }
     catch (std::bad_alloc)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }
     catch (...)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }


     return dwStatus;
}

/*static*/ DWORD CLeakLib::LeakHandle(const size_t numberOfCalls)
{
     DWORD dwStatus = ERROR_SUCCESS;

     HANDLE hMutex = INVALID_HANDLE_VALUE;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (ERROR_SUCCESS == dwStatus); nCalls++)
          {
               std::wstring sMutexName = L"Local\\LeakedMutex" + nCalls;
               hMutex = CreateMutex(nullptr, true, sMutexName.c_str());
               if (INVALID_HANDLE_VALUE == hMutex) dwStatus = ERROR_OUTOFMEMORY;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 7 });
          }
     }
     catch (std::bad_alloc)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }
     catch (...)
     {
          dwStatus = ERROR_OUTOFMEMORY;
     }

     return dwStatus;
}
