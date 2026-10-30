#include "storage.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// stores vault entries into the disc
Status storage_save(const vault *v, const char *path) {
  if (!v || !path || path[0] == '\0') {
    return STATUS_INVALID_INPUT;
  }

  FILE *f = fopen(path, "w");
  if (!f) {
    return STATUS_IO_ERROR;
  }
  for (size_t i = 0; i < v->count; i++) {
    int written = fprintf(f, "%s\t%s\t%s\n", v->items[i].service,
                          v->items[i].username, v->items[i].password);
    if (written < 0) {
      fclose(f);
      return STATUS_IO_ERROR;
    }
  }
  if (fclose(f) != 0) {
    return STATUS_IO_ERROR;
  }
  return STATUS_OK;
}

// loads extries from disc to the vault
Status storage_load(vault *v, const char *path) {
  if (!v || !path || path[0] == '\0') {
    return STATUS_INVALID_INPUT;
  }
  FILE *f = fopen(path, "r");
  if (!f) {
    if (errno == ENOENT) {
      return STATUS_OK; // Missing file on first run is not an error!
    }
    return STATUS_IO_ERROR; // Real I/O or permissions error
  }
  char line[512];
  while (fgets(line, sizeof(line), f)) {
    // 1. Strip trailing newline if present
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') {
      line[len - 1] = '\0';
    }
    // 2. Find first tab (separating service and username)
    char *tab1 = strchr(line, '\t');
    if (!tab1) {
      fclose(f);
      return STATUS_IO_ERROR; // Damaged line: missing first tab
    }
    *tab1 = '\0';
    char *service = line;
    char *rest = tab1 + 1;
    // 3. Find second tab (separating username and password)
    char *tab2 = strchr(rest, '\t');
    if (!tab2) {
      fclose(f);
      return STATUS_IO_ERROR; // Damaged line: missing second tab
    }
    *tab2 = '\0';
    char *username = rest;
    char *password = tab2 + 1;
    // 4. Ensure there is no third tab
    if (strchr(password, '\t') != NULL) {
      fclose(f);
      return STATUS_IO_ERROR; // Damaged line: extra tab
    }
    // 5. Add to the in-memory vault
    Status s = vault_add(v, service, username, password);
    if (s != STATUS_OK) {
      fclose(f);
      return s;
    }
  }
  fclose(f);
  return STATUS_OK;
}

//
Status storage_get_path(char *buf, size_t size) {
  if (!buf || size == 0) {
    return STATUS_INVALID_INPUT;
  }

  // in macos and linux this gives us the home directory path
  const char *home = getenv("HOME");
  if (!home || home[0] == '\0') {
    return STATUS_IO_ERROR;
  }

  // building the path from the above home directory
  int written = snprintf(buf, size, "%s/.pm_vault.txt", home);
  if (written < 0 || (size_t)written >= size) {
    return STATUS_TOO_LONG;
  }

  return STATUS_OK;
}