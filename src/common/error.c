/* error.c - messages for the status codes in common.h. */
#include "common.h"

const char *pm_status_str(pm_status s) {
  switch (s) {
  case PM_OK:
    return "success";
  case PM_ERR_INVALID_ARG:
    return "invalid argument";
  case PM_ERR_NOT_FOUND:
    return "entry not found";
  case PM_ERR_DUPLICATE:
    return "entry already exists";
  case PM_ERR_TOO_LONG:
    return "input too long";
  case PM_ERR_NO_MEMORY:
    return "out of memory";
  case PM_ERR_IO:
    return "file read/write error";
  case PM_ERR_CRYPTO:
    return "encryption error";
  case PM_ERR_WRONG_PASSWORD:
    return "wrong master password";
  }
  return "unknown error";
}