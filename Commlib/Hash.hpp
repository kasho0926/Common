#pragma once
#include <cstdio>

inline void CombineHash(std::size_t & seed, const std::size_t & hash)
{
  seed ^= hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

inline std::size_t BKDRHash(const char * str)
{
  unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
  unsigned int hash = 0;

  while(*str) {
    hash = hash * seed + (*str++);
  }

  return (hash & 0x7FFFFFFF);
}