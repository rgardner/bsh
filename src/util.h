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

/** Get size of struct member.
 *
 *  http://stackoverflow.com/a/3553321/4228400
 */
#define MEMBER_SIZE(type, member) (sizeof(((type*)0)->member))

/** Gets the number of elements in the stack buffer. */
#define BSH_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define BSH_ARRAY_SSIZE(arr) ((ptrdiff_t)BSH_ARRAY_SIZE(arr))

/** Remove trailing whitespace.
 *
 *  @return New length of s.
 */
size_t trim_right(char* s);

size_t min(size_t a, size_t b);
size_t safe_sub(size_t lhs, size_t rhs, size_t default_value);
#endif
