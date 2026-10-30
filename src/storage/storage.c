#include "storage.h"
#include <stdio.h>
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