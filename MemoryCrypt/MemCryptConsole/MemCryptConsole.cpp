// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

#include "stdafx.h"

#include "../inc/MemoryCrypt.h"

int main()
{
     HRESULT hrResult = ERROR_SUCCESS;

     try
     {
          std::wstring sTest(TEXT("This is a test string"));

          std::wcout << TEXT("Test String: '") << sTest.c_str() << TEXT("'") << std::endl;

          CHECK_SUCCEEDED_LOG_THROW( MemoryCrypt::Encrypt(sTest) );

          wprintf_s(TEXT("Encrypted Test String: '%s'"), sTest.c_str());

          CHECK_SUCCEEDED_LOG_THROW( MemoryCrypt::Decrypt(sTest) );

          std::wcout << TEXT("Decrypted Test String: '") << sTest.c_str() << TEXT("'") << std::endl;
     }
     catch (HRESULT& check_catch_hresult)
     {
          hrResult = check_catch_hresult;
     }

     return hrResult;
}

