/* secure.c - wipe sensitive memory. */
#include "common.h"

void pm_secure_wipe(void *p, size_t n) {
  volatile unsigned char *v = p;
  while (n--)
    *v++ = 0;
}