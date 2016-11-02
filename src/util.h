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
#define MEMBER_SIZE(type, member) (sizeof(((type *)0)->member))

/** Remove trailing whitespace.
 *
 *  @return New length of s.
 */
size_t trim_right(char* s);
#endif
