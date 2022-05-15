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

// [Dependencies]
#include <stdio.h>

#include "Platform.h"

// [Api-Begin]
#include "ApiBegin.h"

// helpers
namespace AsmJit {

// ============================================================================
// [AsmJit::Assert]
// ============================================================================

void assertionFailure(const char* file, int line, const char* exp)
{
  fprintf(stderr,
    "*** ASSERTION FAILURE at %s (line %d)\n"
    "*** %s\n", file, line, exp);

  exit(1);
}

// ============================================================================
// [AsmJit::Helpers]
// ============================================================================

static bool isAligned(sysuint_t base, sysuint_t alignment)
{
  return base % alignment == 0;
}

static sysuint_t roundUp(sysuint_t base, sysuint_t pageSize)
{
  sysuint_t over = base % pageSize;
  return base + (over > 0 ? pageSize - over : 0);
}

// Implementation is from "Hacker's Delight" by Henry S. Warren, Jr.,
// figure 3-3, page 48, where the function is called clp2.
static sysuint_t roundUpToPowerOf2(sysuint_t base)
{
  base -= 1;

  base = base | (base >> 1);
  base = base | (base >> 2);
  base = base | (base >> 4);
  base = base | (base >> 8);
  base = base | (base >> 16);

  // I'm trying to make this portable and MSVC strikes me the warning C4293:
  //   "Shift count negative or too big, undefined behavior"
  // Fixing...
#if _MSC_VER
# pragma warning(disable: 4293)
#endif // _MSC_VER

  if (sizeof(sysuint_t) >= 8)
    base = base | (base >> 32);

  return base + 1;
}

} // AsmJit namespace

// ============================================================================
// [AsmJit::VirtualMemory::Windows]
// ============================================================================

#if defined(ASMJIT_WINDOWS)

#include <windows.h>

namespace AsmJit {

struct ASMJIT_HIDDEN VirtualMemoryLocal
{
  VirtualMemoryLocal() ASMJIT_NOTHROW
  {
    SYSTEM_INFO info;
    GetSystemInfo(&info);

    alignment = info.dwAllocationGranularity;
    pageSize = roundUpToPowerOf2(info.dwPageSize);
  }

  sysuint_t alignment;
  sysuint_t pageSize;
};

static VirtualMemoryLocal& vm() ASMJIT_NOTHROW
{
  static VirtualMemoryLocal vm;
  return vm;
};

void* VirtualMemory::alloc(sysuint_t length, sysuint_t* allocated, bool canExecute)
  ASMJIT_NOTHROW
{
  return allocProcessMemory(GetCurrentProcess(), length, allocated, canExecute);
}

void VirtualMemory::free(void* addr, sysuint_t length)
  ASMJIT_NOTHROW
{
  return freeProcessMemory(GetCurrentProcess(), addr, length);
}

void* VirtualMemory::allocProcessMemory(HANDLE hProcess, sysuint_t length, sysuint_t* allocated, bool canExecute) ASMJIT_NOTHROW
{
  // VirtualAlloc rounds allocated size to page size automatically.
  sysuint_t msize = roundUp(length, vm().pageSize);

  // Windows XP SP2 / Vista allow Data Excution Prevention (DEP).
  WORD protect = canExecute ? PAGE_EXECUTE_READWRITE : PAGE_READWRITE;
  LPVOID mbase = VirtualAllocEx(hProcess, NULL, msize, MEM_COMMIT | MEM_RESERVE, protect);
  if (mbase == NULL) return NULL;

  ASMJIT_ASSERT(isAligned(reinterpret_cast<sysuint_t>(mbase), vm().alignment));

  if (allocated) *allocated = msize;
  return mbase;
}

void VirtualMemory::freeProcessMemory(HANDLE hProcess, void* addr, sysuint_t /* length */) ASMJIT_NOTHROW
{
  VirtualFreeEx(hProcess, addr, 0, MEM_RELEASE);
}

sysuint_t VirtualMemory::getAlignment()
  ASMJIT_NOTHROW
{
  return vm().alignment;
}

sysuint_t VirtualMemory::getPageSize()
  ASMJIT_NOTHROW
{
  return vm().pageSize;
}

} // AsmJit

#endif // ASMJIT_WINDOWS

// ============================================================================
// [AsmJit::VirtualMemory::Posix]
// ============================================================================

#if defined(ASMJIT_POSIX)

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

// MacOS uses MAP_ANON instead of MAP_ANONYMOUS
#ifndef MAP_ANONYMOUS
# define MAP_ANONYMOUS MAP_ANON
#endif

namespace AsmJit {

struct ASMJIT_HIDDEN VirtualMemoryLocal
{
  VirtualMemoryLocal() ASMJIT_NOTHROW
  {
    alignment = pageSize = getpagesize();
  }

  sysuint_t alignment;
  sysuint_t pageSize;
};

static VirtualMemoryLocal& vm()
  ASMJIT_NOTHROW
{
  static VirtualMemoryLocal vm;
  return vm;
}

void* VirtualMemory::alloc(sysuint_t length, sysuint_t* allocated, bool canExecute)
  ASMJIT_NOTHROW
{
  sysuint_t msize = roundUp(length, vm().pageSize);
  int protection = PROT_READ | PROT_WRITE | (canExecute ? PROT_EXEC : 0);
  void* mbase = mmap(NULL, msize, protection, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (mbase == MAP_FAILED) return NULL;
  if (allocated) *allocated = msize;
  return mbase;
}

void VirtualMemory::free(void* addr, sysuint_t length)
  ASMJIT_NOTHROW
{
  munmap(addr, length);
}

sysuint_t VirtualMemory::getAlignment()
  ASMJIT_NOTHROW
{
  return vm().alignment;
}

sysuint_t VirtualMemory::getPageSize()
  ASMJIT_NOTHROW
{
  return vm().pageSize;
}

} // AsmJit

#endif // ASMJIT_POSIX

// [Api-End]
#include "ApiEnd.h"
