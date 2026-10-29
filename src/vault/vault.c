#include "vault.h"
#include <stdlib.h>

void vault_init(vault *v) {
  if (!v) {
    return;
  }
  v->items = NULL;
  v->count = 0;
  v->capacity = 0;
}

void vault_free(vault *v) {
  if (!v) {
    return;
  }
  free(v->items);
  vault_init(v);
}