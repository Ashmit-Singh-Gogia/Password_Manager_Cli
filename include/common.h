#include <stddef.h>

#ifndef PM_COMMON_H // This prevents errors from duplicate definitions
#define PM_COMMON_H
// Failure responses
typedef enum {
  PM_OK = 0,
  PM_ERR_INVALID_ARG,
  PM_ERR_NOT_FOUND,
  PM_ERR_DUPLICATE,
  PM_ERR_TOO_LONG,
  PM_ERR_NO_MEMORY,
  PM_ERR_IO,
  PM_ERR_CRYPTO,
  PM_ERR_WRONG_PASSWORD
} pm_status;

// Prints the string form of the error codes
const char *pm_status_str(pm_status s);
void pm_secure_wipe(void *p, size_t n);

#endif