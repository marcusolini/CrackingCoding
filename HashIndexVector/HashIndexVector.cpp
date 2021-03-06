// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// IndexVector.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <functional>

#include "HashIndexVector.h"

int TestHashIndexVector()
{
     int nStatus = EXIT_SUCCESS;
     
     IndexVector<int, int>* pIndexVector = nullptr;

     int array[] = { 2, 6, 778, 45, 333, 3457, 4433, 5667, 33234, 5 };

     try
     {
          pIndexVector = new IndexVector<int, int>();

          for (auto& val : array)
          {
               pIndexVector->Insert(val, val);
               int value = pIndexVector->Get(val);
          }
     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
          nStatus = EXIT_FAILURE;
     }

     if (pIndexVector)
     {
          delete pIndexVector;
          pIndexVector = nullptr;
     }

     return nStatus;
}

size_t NextPrime(size_t num)
{
     num++;
     for (size_t i = 2; i < num; i++)
     {
          if (num%i == 0)
          {
               num++;
               i = 2;
          }
     }
     return num;
}


int TestHash()
{
     int nStatus = EXIT_SUCCESS;

     int array[] = { 2, 6, 778, 45, 333, 3457, 4433, 5667, 33234, 5 };
     size_t count = 17;

     for (auto& val : array)
     {
          std::hash<int> m_hashFunc;
          size_t hash = m_hashFunc(val);
          size_t index = hash % count;

          int nLoop = 1;

          do
          {
               size_t newHash = m_hashFunc(val);
               size_t newCount = count * 2;

               newCount = NextPrime(newCount);

               size_t countDiff = newCount - count;

               size_t newIndex = newHash % newCount;
               size_t indexDiff = (newIndex>index) ? (newIndex-index) : (index-newIndex);

               std::wcout << L"cout:" << count << L" newCount:" << newCount << L" diff:" << countDiff << std::endl;
               std::wcout << L"index:" << index << L" newIndex:" << newIndex << L" diff:" << indexDiff << std::endl;

               if (hash != newHash)
               {
                   //throw std::out_of_range("hashes not equal");
               }

               if (index != newIndex)
               {
                    //throw std::out_of_range("indices not equal");
               }

               count = newCount;
               index = newIndex;

               nLoop += 1;

          } while (nLoop <= 10);
     }

     return nStatus;
}



int wmain()
{
     int nStatus = EXIT_SUCCESS;

     try
     {
          //nStatus += TestHash();
          nStatus += TestHashIndexVector();
     }
     catch (std::exception& e)
     {
          std::wcerr << L"exception: " << e.what() << std::endl;
     }

     return nStatus;
}

