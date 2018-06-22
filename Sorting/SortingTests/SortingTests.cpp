// SortingTests.cpp : Defines the entry point for the console application.
//


#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN 
     //#include <SDKDDKVer.h>
     //#include <stdio.h>
     #include <windows.h>
#else

#endif // _WIN32

#include <string>
#include <chrono>
#include <errno.h>
#include <memory>

#include "../../Error_Checks/ERROR_CHECKS.H"
#include "../SortingTemplateLib/SortingLib.h"


// DECLARATIONS

template < class T, class U > struct ArraysStruct;

long TestNegatives();
long TestInts();
long TestAlpas();

template<class T, class U> long RunTests(ArraysStruct<T,U> arrays[], std::size_t arraysSize);
template < class T, class U > long PrintArray(const T* array, const U arraySize);


// TESTING

int main()
{
     auto start = std::chrono::high_resolution_clock::now();

     long nStatus = EXIT_SUCCESS;

     nStatus += TestNegatives();
     nStatus += TestInts();
     nStatus += TestAlpas();

     auto end = std::chrono::high_resolution_clock::now();
     auto nTotalMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

     std::wcout << L"All RESULTS: " << (nStatus ? L"FAILURE" : L"SUCCESSFUL") << std::endl;
     std::wcout << L"ALL EXECUTION TIME: " << nTotalMilliseconds.count() << L" milliseconds" << std::endl;

     return nStatus;
}



// DEFINITIONS

template < class T, class U >
struct ArraysStruct
{
     T* array;
     U size;
};



template < class T, class U >
long PrintArray(const T* array, const U arraySize)
{
     long nStatus = 0;

     if ((0 == array) || (0 == arraySize))
     {
          std::wcout << L"Invalid array" << std::endl;
          return EINVAL;
     }

     for (U iIndex = 0; iIndex < arraySize; iIndex++)
     {
          std::wcout << array[iIndex] << L" ";
     }
     std::wcout << std::endl;

     return nStatus;
}


long TestNegatives()
{
     long nStatus = EXIT_SUCCESS;

     bool bSortCorrect = false;
     long nNegativeStatus = 0;

     int* n1 = 0;
     int n2[] = { 1 };

     ArraysStruct<int, std::size_t> nullTestArrays[] = {
     { n1, sizeof(n1) / sizeof(int) },    // 0, value
     { n1, 0 },                           // 0, 0
     { n2, 0 }                            // value, 0
     };

     for (auto& iNullTestArrays : nullTestArrays)
     {
          for (ESortingTypes eSortType = ESortingTypes::QuickSort; eSortType <= ESortingTypes::BubbleSort; eSortType++)
          {
               std::wcout << L"SORTING TYPE: " << eSortType << std::endl;

               nNegativeStatus += PrintArray<int, std::size_t>(iNullTestArrays.array, iNullTestArrays.size);
               nNegativeStatus += CSorting<int, std::size_t>::Sort(eSortType, iNullTestArrays.array, iNullTestArrays.size);
               nNegativeStatus += CSorting<int, std::size_t>::VerifySort(0, 0, bSortCorrect);

               if (true == bSortCorrect || 0 == nNegativeStatus)
               {
                    nStatus += EINVAL;
               }

               std::wcout << L"Negative Test Results: " << (nNegativeStatus ? L"SUCCESS" : L"FAILURE") << std::endl << std::endl;
               nNegativeStatus = 0;
          }
     }

     return nStatus;
}


long TestInts()
{
     long nStatus = EXIT_SUCCESS;

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

     ArraysStruct<int, std::size_t> testArrays[] = {
     { a1, sizeof(a1) / sizeof(int) },
     { a2, sizeof(a2) / sizeof(int) },
     { a3, sizeof(a3) / sizeof(int) },
     { a4, sizeof(a4) / sizeof(int) },
     { a5, sizeof(a5) / sizeof(int) },
     { a6, sizeof(a6) / sizeof(int) },
     { a7, sizeof(a7) / sizeof(int) },
     { a8, sizeof(a8) / sizeof(int) },
     { a9, sizeof(a9) / sizeof(int) },
     { a10, sizeof(a10) / sizeof(int) },
     };

     nStatus = RunTests<int, std::size_t>(testArrays, sizeof(testArrays)/sizeof(ArraysStruct<int, std::size_t>));

     return nStatus;
}


long TestAlpas()
{
     long nStatus = EXIT_SUCCESS;

     wchar_t a1[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
     wchar_t a2[] = { 'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a' };
     wchar_t a3[] = { 'x', 'y', 'z', 'c', 'b', 'a' };
     wchar_t a4[] = { 'w', 'x', 'y', 'z', 'c', 'b', 'a' };
     wchar_t a5[] = { 'a' };
     wchar_t a6[] = { 'b', 'b', 'a', 'a' };
     wchar_t a7[] = { 'b', 'b', 'c', 'c', 'a', 'a' };
     wchar_t a8[] = { 'b', 'a', 'b' };

     ArraysStruct<wchar_t, std::size_t> testArrays[] = {
     { a1, sizeof(a1) / sizeof(wchar_t) },
     { a2, sizeof(a2) / sizeof(wchar_t) },
     { a3, sizeof(a3) / sizeof(wchar_t) },
     { a4, sizeof(a4) / sizeof(wchar_t) },
     { a5, sizeof(a5) / sizeof(wchar_t) },
     { a6, sizeof(a6) / sizeof(wchar_t) },
     { a7, sizeof(a7) / sizeof(wchar_t) },
     { a8, sizeof(a8) / sizeof(wchar_t) },
     };

     nStatus = RunTests<wchar_t, std::size_t>(testArrays, sizeof(testArrays)/sizeof(ArraysStruct<wchar_t, std::size_t>));

     return nStatus;
}


template<class T, class U> long RunTests(ArraysStruct<T, U> arrays[], std::size_t arraysSize)
{
     long nStatus = EXIT_SUCCESS;

     bool bSortCorrect = false;
     std::size_t nNumberOfSorts = 0;
     std::chrono::duration<double> duration;

     for (auto arraysIndex = 0; arraysIndex < arraysSize; arraysIndex++)
     {
          for (ESortingTypes eSortType = ESortingTypes::QuickSort; eSortType <= ESortingTypes::BubbleSort; eSortType++)
          {
               try
               {
                    nNumberOfSorts = 0;

                    std::shared_ptr<T[]> tempArray(new T[arrays[arraysIndex].size]);

                    for (auto nIndex = 0; nIndex < arrays[arraysIndex].size; nIndex++)
                         tempArray[nIndex] = arrays[arraysIndex].array[nIndex];

                    std::wcout << L"SORTING TYPE: " << eSortType;

                    std::wcout << L" -- INPUT ARRAY: ";
                    //    CHECK_SUCCEEDED_LOG_THROW((PrintArray<PIntArray, std::size_t>(iTestArrays.array, iTestArrays.size)));
                    CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray.get(), arrays[arraysIndex].size)));

                    CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::Sort(eSortType, tempArray.get(), arrays[arraysIndex].size, &nNumberOfSorts, &duration)));

                    std::wcout << L"SORTED ARRAY: ";
                    CHECK_SUCCEEDED_LOG_THROW((PrintArray<T, U>(tempArray.get(), arrays[arraysIndex].size)));

                    auto nNumberOfMircosecondsToSort = std::chrono::duration_cast<std::chrono::microseconds>(duration);

                    std::wcout << L"SORTS: " << nNumberOfSorts << L" - " << L"TIME: " << nNumberOfMircosecondsToSort.count() << L" microseconds: " << std::endl;

                    CHECK_SUCCEEDED_LOG_THROW((CSorting<T, U>::VerifySort(tempArray.get(), arrays[arraysIndex].size, bSortCorrect)));
                    std::wcout << L"RESULT: " << (bSortCorrect ? L"SUCCESSFUL" : L"FAILURE") << std::endl << std::endl;

                    CHECK_BOOL_TRUE_LOG_THROW(bSortCorrect);
               }
               catch (long& check_catch_lresult)
               {
                    nStatus += check_catch_lresult;
               }
          }
     }

     return nStatus;
}



