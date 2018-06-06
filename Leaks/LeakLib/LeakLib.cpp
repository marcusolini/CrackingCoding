// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// LeakLib.cpp : Defines the exported functions for the DLL application.
//

#ifdef _WIN32
#include "stdafx.h"
#endif // _WIN32

#include <malloc.h>
#include <new>
#include <string>
#include <thread>

#include <cerrno>
#include <cstddef>
#include <cstdlib>

#include "LeakLib.h"


/*static*/ int CLeakLib::LeakNewMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{    
     int nStatus = 0;

     char* pChars = nullptr;

     try
     {

          for (auto nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               pChars = new (std::nothrow) char[bytesEachCall];
               if (nullptr == pChars) nStatus = ENOMEM;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 2 });
          }

     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }

     return nStatus;
}

/*static*/ int CLeakLib::LeakMallocMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{
     int nStatus = 0;

     void* pVoid = nullptr;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               pVoid = malloc(bytesEachCall);
               if (nullptr == pVoid) nStatus = ENOMEM;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 3 });
          }
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }

     return nStatus;
}

/*static*/ int CLeakLib::LeakCallocMemory(const size_t numberOfCalls, const size_t bytesEachCall)
{
     int nStatus = 0;

     void* pVoid = nullptr;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               pVoid = calloc(1, bytesEachCall);
               if (nullptr == pVoid) nStatus = ENOMEM;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 5 });
          }
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }


     return nStatus;
}

#ifdef _WIN32
/*static*/ int CLeakLib::LeakHandle(const size_t numberOfCalls)
{
     int nStatus = 0;

     HANDLE hMutex = INVALID_HANDLE_VALUE;

     try
     {
          for (auto nCalls = 0; (nCalls < numberOfCalls) && (0 == nStatus); nCalls++)
          {
               std::wstring sMutexName = L"Local\\LeakedMutex" + nCalls;
               hMutex = CreateMutex(nullptr, true, sMutexName.c_str());
               if (INVALID_HANDLE_VALUE == hMutex) nStatus = ENOMEM;
               std::this_thread::sleep_for(std::chrono::milliseconds{ 7 });
          }
     }
     catch (std::bad_alloc)
     {
          nStatus = ENOMEM;
     }
     catch (...)
     {
          nStatus = ENOMEM;
     }

     return nStatus;
}
#endif // _WIN32