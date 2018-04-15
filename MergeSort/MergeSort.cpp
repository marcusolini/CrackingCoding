// Copyright(c) 2018 marcusolini@outlook.com
// MergeSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <memory>

// Declarations 

using INT = int;
using INTARRAY = int*;
using PPINTARRAY = int**;

template <class T, class U> struct ARRAY_STRUCT;

template <class T, class U> void MergeSort(T array, U size, T mergedArray);
template <class T, class U> void MergeSort(T array, U size);
template <class T, class U, class V> void MergeSplit(T array, U size, V lArray, U& lArraySize, V rArray, U&rArraySize);
template <class T, class U> void PrintArray(T array, U size);
template <class T, class U> int VerifyArray(T array, U size);


// Definitions

template <class T, class U> 
void MergeSort(T array, U size, T mergedArray)  
{
     if (nullptr == array || 0 == size)
     {
          std::wcout << L"Invalid array" << std::endl << std::endl;
     }
     else
     {
          if (size > 1)
          {
               T lArray = 0, rArray = 0;
               U lArraySize = 0, rArraySize = 0;

               MergeSplit<T, U, PPINTARRAY>(array, size, &lArray, lArraySize, &rArray, rArraySize);
               MergeSort<T, U>(lArray, lArraySize, mergedArray);
               MergeSort<T, U>(rArray, rArraySize, mergedArray);

               // Perform merge sort
               int lIndex = 0, rIndex = 0, mIndex = 0;

               while (lIndex < lArraySize && rIndex < rArraySize)
               {
                    if (lArray[lIndex] < rArray[rIndex])
                    {
                         mergedArray[mIndex] = lArray[lIndex];
                         lIndex++;
                    }
                    else
                    {
                         mergedArray[mIndex] = rArray[rIndex];
                         rIndex++;
                    }
                    mIndex++;
               }

               // Copy the remaining
               for (; lIndex < lArraySize; lIndex++, mIndex++)
                    mergedArray[mIndex] = lArray[lIndex];
               for (; rIndex < rArraySize; rIndex++, mIndex++)
                    mergedArray[mIndex] = rArray[rIndex];

               // Copy sorted and merged array back into original array
               for (int nIndex = 0; nIndex < size; nIndex++)
                    array[nIndex] = mergedArray[nIndex];
          }
     }
}


template <class T, class U, class V>
void MergeSplit(T array, U size, V lArray, U& lArraySize, V rArray, U& rArraySize)
{
     if (nullptr == array || 0 == size)
     {
          std::wcout << L"Invalid array" << std::endl << std::endl;
     }
     else
     {
          lArraySize = size / 2;
          *lArray = array;

          rArraySize = size - lArraySize;
          *rArray = &array[lArraySize];
     }
}
 

template <class T, class U>
void MergeSort(T array, U size)
{
     if (nullptr == array || 0 == size)
     {
          std::wcout << L"Invalid array" << std::endl << std::endl;
     }
     else
     {
          T pMergedArray = new U[size];

          for (auto nIndex = 0; nIndex < size; nIndex++)  // initialize merged array with original array
               pMergedArray[nIndex] = array[nIndex];

          MergeSort<T, U>(array, size, pMergedArray);

          for (auto nIndex = 0; nIndex < size; nIndex++) // copy merged array into original array
               array[nIndex] = pMergedArray[nIndex];

          if (pMergedArray) delete[] pMergedArray;
     }
}


template <class T, class U>
struct ARRAY_STRUCT
{
     T array;
     U size;
};

template <class T, class U>
void PrintArray(T array, U size)
{
     if (nullptr == array || 0 == size)
     {
          std::wcout << L"Invalid array" << std::endl << std::endl;
     }
     else
     {
          for (int nIndex = 0; nIndex < size; nIndex++)
          {
               std::wcout << array[nIndex] << " ";
          }
          std::cout << std::endl;
     }
}


template <class T, class U>
int VerifyArray(T array, U size)
{    // TODO: Enhance to also verify all elements are present and amount is correct.

     int nStatus = EXIT_SUCCESS;

     if (nullptr == array || 0 == size)
     {
          std::wcout << L"Invalid array" << std::endl;
          //nStatus = EXIT_FAILURE;
     }
     else
     {
          for (int nIndex = 0; (nIndex < size - 1) && (EXIT_SUCCESS == nStatus); nIndex++)
          {
               if (array[nIndex] > array[nIndex + 1])
               {
                    nStatus = EXIT_FAILURE;
               }
          }
     }

     std::wcout << L"Result: " << (nStatus ? L"Falure" : L"Success") << std::endl << std::endl;

     return nStatus;
}


int main()
{
     int nStatus = EXIT_SUCCESS;

     int a0[] = { 10, 5, 2, 7, 4, 9, 12, 1, 8, 6, 11, 3 };
     int a1[] = { 15, 3, 9, 8, 2, 7, 1, 6 };
     int a2[] = { 5, 3, 10, 1, 2 };
     int a3[] = { 5, 3, 10, 11, 2, 4 };
     int a4[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
     int a5[] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
     int a6[] = { 10, 10, 9, 9, 8, 8 };
     int a7[] = { 1, 0 };
     int a8[] = { 0, 1, 0 };
     int a9[] = { 0, 0, 1, 0, 0 };
     int a10[] = { 1 };

     ARRAY_STRUCT<INTARRAY, INT> arrays[] = {
       { a0, sizeof(a0) / sizeof(int) }
      ,{ a1, sizeof(a1) / sizeof(int) }
      ,{ a2, sizeof(a2) / sizeof(int) }
      ,{ a3, sizeof(a3) / sizeof(int) }
      ,{ a4, sizeof(a4) / sizeof(int) }
      ,{ a5, sizeof(a5) / sizeof(int) }
      ,{ a6, sizeof(a6) / sizeof(int) }
      ,{ a7, sizeof(a7) / sizeof(int) }
      ,{ a8, sizeof(a8) / sizeof(int) }
      ,{ a9, sizeof(a9) / sizeof(int) }
      ,{ a10, sizeof(a10) / sizeof(int) }
     };

     // Null Tests
     PrintArray<INTARRAY, INT>(0, 0);
     MergeSort<INTARRAY, INT>(0, 0);
     PrintArray<INTARRAY, INT>(0, 0);
     nStatus += VerifyArray<INTARRAY, INT>(0, 0);

     // Sort Tests
     for (auto& array : arrays)
     {
          PrintArray<INTARRAY, INT>(array.array, array.size);
          MergeSort<INTARRAY, INT>(array.array, array.size);
          PrintArray<INTARRAY, INT>(array.array, array.size);
          nStatus += VerifyArray<INTARRAY, INT>(array.array, array.size);
     }
     
     std::wcout << L"ALL RESULTS: " << (nStatus ? L"FAILURE" : L"SUCCESS") << std::endl << std::endl;


     return nStatus;
}