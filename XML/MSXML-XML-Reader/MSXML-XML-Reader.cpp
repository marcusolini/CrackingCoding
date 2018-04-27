// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//

#include "stdafx.h"

#include <iostream>
#include <sstream>

#include <objbase.h>
#include <msxml6.h>
#include <Propvarutil.h>

#include <atlbase.h> // CComBSTR

#pragma comment(lib, "msxml6.lib")

#include "ERROR_CHECKS.H"

HRESULT PromptForXML(std::wstring& sXmlPath);
HRESULT WalkNode(IXMLDOMNode* pNode);

int wmain()
{
     HRESULT hr = ERROR_SUCCESS;
     bool bComInitalized = false;

     try
     {
          CComPtr<IXMLDOMDocument2> pDoc;

          CComBSTR bstrXML;
          CComVariant varFileName;
          VARIANT_BOOL bVarStatus = VARIANT_FALSE;

          CHECK_SUCCEEDED_LOG_THROW(CoInitialize(nullptr));
          bComInitalized = true;

          CHECK_SUCCEEDED_LOG_THROW(hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument2, (void**)&pDoc));

          pDoc->put_async(VARIANT_FALSE);
          pDoc->put_validateOnParse(VARIANT_FALSE);
          pDoc->put_resolveExternals(VARIANT_FALSE);

          std::wstring xmlFilePathname;

          hr = PromptForXML(xmlFilePathname);

          CHECK_SUCCEEDED_LOG_THROW(InitVariantFromString(xmlFilePathname.c_str(), &varFileName));

          CHECK_SUCCEEDED_LOG_THROW(pDoc->load(varFileName, &bVarStatus));

          if (bVarStatus == VARIANT_TRUE)
          {
               try
               {
                    CHECK_SUCCEEDED_LOG_THROW(pDoc->get_xml(&bstrXML));
                    std::wcout << std::endl << TEXT("XML:") << bstrXML.m_str << std::endl << std::endl;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    std::wcout << TEXT("XML output failure: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
                    throw;
               }

               CHECK_SUCCEEDED_LOG_THROW(WalkNode(pDoc));
          }
          else
          {
               CComPtr<IXMLDOMParseError> pXMLErr;
               long errorCode = 0;

               try
               {
                    CHECK_SUCCEEDED_LOG_THROW(pDoc->get_parseError(&pXMLErr));
                    CHECK_SUCCEEDED_LOG_THROW(pXMLErr->get_errorCode(&errorCode));
                    std::wcout << TEXT("XML load failure: ") << std::hex << std::showbase << errorCode << std::endl;
               }
               catch (HRESULT& check_catch_hresult)
               {
                    std::wcout << TEXT("XML load failure: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
                    throw;
               }
          }

     }
     catch (HRESULT& check_catch_hresult)
     {
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     if (bComInitalized) CoUninitialize();

     return 0;
}

HRESULT PromptForXML(std::wstring& sXmlPath)
{
     HRESULT hr = ERROR_SUCCESS;

     sXmlPath.clear();

     std::wcout << TEXT("Enter XML File Pathname: ");
     std::wcin >> sXmlPath;

     return hr;
}


HRESULT WalkNode(IXMLDOMNode* pNode)
{
     HRESULT hr = ERROR_SUCCESS;

     try
     {
          CComPtr<IXMLDOMNamedNodeMap>  pDocAttribMap;
          long numDocAttribs = 0;

          CComPtr<IXMLDOMNodeList> pChildNodes;
          long numChildNodes = 0;

          std::wostringstream outputStream;

          CComBSTR bstrTemp;
          CComVariant varNameTemp;

          std::wcout << std::endl << std::endl;

          std::wcout << TEXT("NODE >>>>>>>> ") << std::endl;

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeName(&bstrTemp));
               outputStream << TEXT("NAME: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_text(&bstrTemp));
               outputStream << TEXT("TEXT: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeTypeString(&bstrTemp));
               outputStream << TEXT("TYPE: '") << bstrTemp.m_str << TEXT("'  ");
               bstrTemp.Empty();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_nodeValue(&varNameTemp));
               //outputStream << TEXT("VALUE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_dataType(&varNameTemp));
               //outputStream << TEXT("DATATYPE: '") << varNameTemp << TEXT("'  ");
               varNameTemp.Clear();
          }
          catch (HRESULT) { /*Continue*/ }

          outputStream << std::endl;
          std::wcout << outputStream.str();
          outputStream.str(TEXT(""));

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_attributes(&pDocAttribMap));
          }
          catch (HRESULT) { /*Continue*/ }

          if (pDocAttribMap)
          {
               try {
                    CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_length(&numDocAttribs));
               }
               catch (HRESULT) { numDocAttribs = 0; /*Continue*/ }

               if (numDocAttribs)
               {
                    outputStream << TEXT("ATTRIBUTES - ");

                    for (long a = 0; a < numDocAttribs; a++)
                    {
                         CComPtr<IXMLDOMNode> pDocAttrib;

                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttribMap->get_item(a, &pDocAttrib));
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_nodeName(&bstrTemp));
                              outputStream << TEXT("NAME: '") << bstrTemp.m_str << TEXT("'  ");
                              bstrTemp.Empty();
                         }
                         catch (HRESULT) { /*Continue*/ }

                         try {
                              CHECK_SUCCEEDED_LOG_THROW(pDocAttrib->get_text(&bstrTemp));
                              outputStream << TEXT("TEXT: '") << bstrTemp.m_str << TEXT("'  ");
                              bstrTemp.Empty();
                         }
                         catch (HRESULT) { /*Continue*/ }
                    }

                    std::wcout << outputStream.str();
                    outputStream.str(TEXT(""));
               }
          }

          try {
               CHECK_SUCCEEDED_LOG_THROW(pNode->get_childNodes(&pChildNodes));
          }
          catch (HRESULT) { /*Continue*/ }

          if (pChildNodes)
          {
               try {
                    CHECK_SUCCEEDED_LOG_THROW(pChildNodes->get_length(&numChildNodes));
               }
               catch (HRESULT) { numChildNodes = 0; /*Continue*/ }

               for (long n = 0; n < numChildNodes; n++)
               {
                    CComPtr<IXMLDOMNode> pChildNode;

                    try {
                         CHECK_SUCCEEDED_LOG_THROW(pChildNodes->get_item(n, &pChildNode));
                         CHECK_SUCCEEDED_LOG_THROW(WalkNode(pChildNode));
                    }
                    catch (HRESULT) { /*Continue*/ }

               }
          }

     }
     catch (HRESULT& check_catch_hresult)
     {
          std::wcout << TEXT("Fatal error: ") << std::hex << std::showbase << check_catch_hresult << std::endl;
     }

     return hr;
}