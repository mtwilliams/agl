//===-- gala/context.c ------------------------------------------*- C++ -*-===//
//
//  Gala
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@origamicomet.com>
//
//===----------------------------------------------------------------------===//

#include "gala/context.h"

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

gala_error_t gala_context_to_s(
  const gala_context_t *context,
  char *buf,
  const size_t buf_sz,
  const gala_error_details_t **error_details)
{
#ifndef GALA_DISABLE_ARGUMENT_CHECKS
  if (context == NULL) {
    if (error_details) {
      *error_details = gala_error_details_create_unformatted(
        GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS,
        "Expected `context' to be non-NULL.");
    }
    return GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS;
  }
  if (buf == NULL) {
    if (error_details) {
      *error_details = gala_error_details_create_unformatted(
        GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS,
        "Expected `buf' to be non-NULL.");
    }
    return GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS;
  }
  if (buf_sz == 0) {
    if (error_details) {
      *error_details = gala_error_details_create_unformatted(
        GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS,
        "Expected `buf' to be greater than `0'.");
    }
    return GALA_ERROR_ONE_OR_MORE_INVALID_ARGUMENTS;
  }
#endif // !GALA_DISABLE_ARGUMENT_CHECKS
  // TODO(mike): Defer to implementation specific implementation.
  const int written = snprintf(buf, buf_sz, "#<gala_context_t:%.16" PRIxPTR "16>",
                               context);
#ifndef GALA_DISABLE_ERROR_CHECKS
  if (written <= 0) {
    if (error_details) {
      *error_details = gala_error_details_create_unformatted(
        GALA_ERROR_OUT_OF_MEMORY,
        "The specified buffer is not large enough.");
    }
    return GALA_ERROR_OUT_OF_MEMORY;
  }
#endif // !GALA_DISABLE_ERROR_CHECKS
  if (error_details)
    *error_details = NULL;
  return GALA_ERROR_NONE;
}

//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//============================================================================//
