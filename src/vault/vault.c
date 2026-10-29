#include "vault.h"
#include <stdlib.h>
#include <string.h>

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

Status vault_add(vault *v, const char *service, const char *username,
                 const char *password) {

  // check for null pointers
  if (!v || !service || !username || !password) {
    return STATUS_INVALID_INPUT;
  }
  // if empty strings are passed, return invalid input
  if (service[0] == '\0' || username[0] == '\0' || password[0] == '\0') {
    return STATUS_INVALID_INPUT;
  }
  // check for string length exceeding the maximum allowed length
  if (strlen(service) >= SERVICE_MAX_LEN ||
      strlen(username) >= USERNAME_MAX_LEN ||
      strlen(password) >= PASSWORD_MAX_LEN) {
    return STATUS_TOO_LONG;
  }

  // duplicate check
  for (size_t i = 0; i < v->count; i++) {
    if (strcmp(v->items[i].service, service) == 0) {
      return STATUS_DUPLICATE;
    }
  }

  // resize the items array if necessary
  if (v->count >= v->capacity) {
    size_t new_capacity = v->capacity == 0 ? 8 : v->capacity * 2;
    entry *new_items = realloc(v->items, new_capacity * sizeof(entry));
    if (!new_items) {
      return STATUS_NO_MEMORY;
    }
    v->items = new_items;
    v->capacity = new_capacity;
  }

  // put the new entry into the items array
  entry *new_entry = &v->items[v->count];
  strncpy(new_entry->service, service, SERVICE_MAX_LEN - 1);
  new_entry->service[SERVICE_MAX_LEN - 1] = '\0';
  strncpy(new_entry->username, username, USERNAME_MAX_LEN - 1);
  new_entry->username[USERNAME_MAX_LEN - 1] = '\0';
  strncpy(new_entry->password, password, PASSWORD_MAX_LEN - 1);
  new_entry->password[PASSWORD_MAX_LEN - 1] = '\0';

  v->count++;

  return STATUS_OK;
}