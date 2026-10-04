#ifndef PM_VAULT_H
#define PM_VAULT_H

#include "common.h"

// Maximum text lengths
#define PM_MAX_SERVICE 63
#define PM_MAX_USERNAME 127
#define PM_MAX_PASSWORD 127
#define PM_MAX_URL 255

typedef struct {
  char *service;
  char *username;
  char *url;
  char *password;
} credential;

pm_status credential_init(credential *c, const char *service,
                          const char *username, const char *url,
                          const char *password);

void credential_free(credential *c);

#endif