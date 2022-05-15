// AsmJit - Complete JIT Assembler for C++ Language.

// Copyright (c) 2008-2010, Petr Kobalicek <kobalicek.petr@gmail.com>
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// We are using sprintf() here.
#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

// [Dependencies]
#include "Logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

// [Api-Begin]
#include "ApiBegin.h"

namespace AsmJit {

// ============================================================================
// [AsmJit::Logger]
// ============================================================================

Logger::Logger() ASMJIT_NOTHROW :
  _enabled(true),
  _used(true),
  _logBinary(false)
{
}

Logger::~Logger() ASMJIT_NOTHROW
{
}

void Logger::logFormat(const char* fmt, ...) ASMJIT_NOTHROW
{
  char buf[1024];
  sysuint_t len;

  va_list ap;
  va_start(ap, fmt);
  len = vsnprintf(buf, 1023, fmt, ap);
  va_end(ap);

  logString(buf, len);
}

void Logger::setEnabled(bool enabled) ASMJIT_NOTHROW
{
  _enabled = enabled;
  _used = enabled;
}

// ============================================================================
// [AsmJit::FileLogger]
// ============================================================================

FileLogger::FileLogger(FILE* stream) ASMJIT_NOTHROW
  : _stream(NULL)
{
  setStream(stream);
}

void FileLogger::logString(const char* buf, sysuint_t len) ASMJIT_NOTHROW
{
  if (!_used) return;

  if (len == (sysuint_t)-1) len = strlen(buf);
  fwrite(buf, 1, len, _stream);
}

void FileLogger::setEnabled(bool enabled) ASMJIT_NOTHROW
{
  _enabled = enabled;
  _used = (_enabled == true) & (_stream != NULL);
}

//! @brief Set file stream.
void FileLogger::setStream(FILE* stream) ASMJIT_NOTHROW
{
  _stream = stream;
  _used = (_enabled == true) & (_stream != NULL);
}

} // AsmJit namespace

// [Api-End]
#include "ApiEnd.h"
