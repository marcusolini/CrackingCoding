// Copyright 2018 marcusolini@outlook.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.

// References
// Doctor Dobbs - A lightweight logger
// A Thread-Safe Logger

#pragma once

#include "LogPolicy.h"
#include <fstream>

class CFileLogPolicy : public ILogPolicy
{
public:
     CFileLogPolicy();
     ~CFileLogPolicy();

     void Open(const std::string& name);
     void Close();
     void Write(const std::string& msg);

private:
     std::ofstream m_outStream;

};


CFileLogPolicy::CFileLogPolicy()
{
}

CFileLogPolicy::~CFileLogPolicy()
{
}

void CFileLogPolicy::Open(const std::string& name)
{
     m_outStream.open(name.c_str(), std::ios_base::out | std::ofstream::app);
     if (!m_outStream.is_open())
     {
          throw(std::runtime_error("LOGGER: Unable to open an output stream"));
     }
}

void CFileLogPolicy::Close()
{
     if (m_outStream.is_open())
     {
          m_outStream.close();
     }
}

void CFileLogPolicy::Write(const std::string& msg)
{
     m_outStream << msg.c_str() << std::endl;
}