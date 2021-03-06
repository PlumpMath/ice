// https://github.com/r-lyeh/giant
// Commit 645e777f09128544df48a08d5d69cd9776cdbd18
//
// Copyright (c) 2011-2014 Mario 'rlyeh' Rodriguez
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#pragma once
#include <cassert>
#include <algorithm>
#include <type_traits>

#if defined(__GLIBC__)
#include <endian.h>
#endif

namespace ice {
namespace giant {

#if defined(_LITTLE_ENDIAN) || (defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && BYTE_ORDER == LITTLE_ENDIAN) || \
  (defined(_BYTE_ORDER) && defined(_LITTLE_ENDIAN) && _BYTE_ORDER == _LITTLE_ENDIAN) ||                          \
  (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN) || defined(__i386__) || \
  defined(__alpha__) || defined(__ia64) || defined(__ia64__) || defined(_M_IX86) || defined(_M_IA64) ||          \
  defined(_M_ALPHA) || defined(__amd64) || defined(__amd64__) || defined(_M_AMD64) || defined(__x86_64) ||       \
  defined(__x86_64__) || defined(_M_X64)
enum { xinu_type = 0, unix_type = 1, nuxi_type = 2, type = xinu_type, is_little = 1, is_big = 0 };
#elif defined(_BIG_ENDIAN) || (defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN) || \
  (defined(_BYTE_ORDER) && defined(_BIG_ENDIAN) && _BYTE_ORDER == _BIG_ENDIAN) ||                         \
  (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && __BYTE_ORDER == __BIG_ENDIAN) || defined(__sparc) || \
  defined(__sparc__) || defined(_POWER) || defined(__powerpc__) || defined(__ppc__) || defined(__hpux) || \
  defined(_MIPSEB) || defined(_POWER) || defined(__s390__)
enum { xinu_type = 0, unix_type = 1, nuxi_type = 2, type = unix_type, is_little = 0, is_big = 1 };
#else
#error <giant/giant.hpp> says: Middle endian/NUXI order is not supported
enum { xinu_type = 0, unix_type = 1, nuxi_type = 2, type = nuxi_type, is_little = 0, is_big = 0 };
#endif

template <typename T>
T swap(T out) {
  if (!std::is_pod<T>::value) {
    return out;
  }

  char* ptr;

  switch (sizeof(T)) {
  case 0:
  case 1: break;
  case 2:
    ptr = reinterpret_cast<char*>(&out);
    std::swap(ptr[0], ptr[1]);
    break;
  case 4:
    ptr = reinterpret_cast<char*>(&out);
    std::swap(ptr[0], ptr[3]);
    std::swap(ptr[1], ptr[2]);
    break;
  case 8:
    ptr = reinterpret_cast<char*>(&out);
    std::swap(ptr[0], ptr[7]);
    std::swap(ptr[1], ptr[6]);
    std::swap(ptr[2], ptr[5]);
    std::swap(ptr[3], ptr[4]);
    break;
  case 16:
    ptr = reinterpret_cast<char*>(&out);
    std::swap(ptr[0], ptr[15]);
    std::swap(ptr[1], ptr[14]);
    std::swap(ptr[2], ptr[13]);
    std::swap(ptr[3], ptr[12]);
    std::swap(ptr[4], ptr[11]);
    std::swap(ptr[5], ptr[10]);
    std::swap(ptr[6], ptr[9]);
    std::swap(ptr[7], ptr[8]);
    break;
  default: assert(!"<giant/giant.hpp> says: POD type bigger than 256 bits (?)"); break;
  }

  return out;
}

template <typename T>
T letobe(const T& in) {
  return swap(in);
}
template <typename T>
T betole(const T& in) {
  return swap(in);
}

template <typename T>
T letoh(const T& in) {
  return type == xinu_type ? in : swap(in);
}
template <typename T>
T htole(const T& in) {
  return type == xinu_type ? in : swap(in);
}

template <typename T>
T betoh(const T& in) {
  return type == unix_type ? in : swap(in);
}
template <typename T>
T htobe(const T& in) {
  return type == unix_type ? in : swap(in);
}

}  // namespace giant
}  // namespace ice