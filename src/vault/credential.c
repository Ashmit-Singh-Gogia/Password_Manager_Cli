#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vault.h>

static pm_status dup_field(char **dst, const char *src, size_t max_len) {
  if (src == NULL) {
    src = "";
  }
  size_t len = strlen(src);

  if (len > max_len) {
    return PM_ERR_TOO_LONG;
  }
  char *p = malloc(len + 1);
  if (p == NULL) {
    return PM_ERR_NO_MEMORY;
  }
  memcpy(p, src, len + 1);
  *dst = p;
  return PM_OK;
}

static void wipe_free(char **p) {
  if (*p != NULL) {
    pm_secure_wipe(*p, strlen(*p));
    free(*p);
    *p = NULL;
  }
}

void credential_free(credential *c) {
  if (c == NULL)
    return;
  wipe_free(&c->service);
  wipe_free(&c->url);
  wipe_free(&c->password);
  wipe_free(&c->username);
}

pm_status credential_init(credential *c, const char *service,
                          const char *username, const char *password,
                          const char *url) {
  if (c == NULL || username == NULL || username[0] == '\0' ||
      password == NULL || password[0] == '\0') {
    return PM_ERR_INVALID_ARG;
  }

  credential tmp = {NULL, NULL, NULL, NULL};
  pm_status s;

  if ((s = dup_field(&tmp.service, service, PM_MAX_SERVICE)) != PM_OK)
    goto fail;
  if ((s = dup_field(&tmp.username, username, PM_MAX_USERNAME)) != PM_OK)
    goto fail;
  if ((s = dup_field(&tmp.password, password, PM_MAX_PASSWORD)) != PM_OK)
    goto fail;
  if ((s = dup_field(&tmp.url, url, PM_MAX_URL)) != PM_OK)
    goto fail;

  *c = tmp; /* success: hand ownership to the caller */
  return PM_OK;
fail:
  credential_free(&tmp); /* free whatever we managed to allocate */
  return s;
}
