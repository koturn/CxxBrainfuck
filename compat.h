/*!
 * @brief Compatibility utility for compilers before C++11
 * @author  koturn
 * @file    compat.h
 */
#ifndef COMPAT_H
#define COMPAT_H


#if __cplusplus < 201103L
static const int nullptr = 0;
#endif


#endif  /* COMPAT_H */
