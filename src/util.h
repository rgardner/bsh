/** Utility functions and macros.
 *
 *  @file
 *
 */

#ifndef UTIL_H
#define UTIL_H

#include <stddef.h>  // size_t

/** Silence unused variable warnings. */
#define UNUSED(x) (void)(x)

/** Remove trailing whitespace.
 *
 *  @return New length of s.
 */
size_t trim_right(char* s);
#endif
