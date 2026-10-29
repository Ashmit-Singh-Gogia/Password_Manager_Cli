#ifndef VAULT_H
#define VAULT_H
#include <stddef.h>
#include <common/status.h>

#define SERVICE_MAX_LEN 64
#define USERNAME_MAX_LEN 64
#define PASSWORD_MAX_LEN 128

typedef struct entry {
  char service[SERVICE_MAX_LEN];
  char username[USERNAME_MAX_LEN];
  char password[PASSWORD_MAX_LEN];
} entry;

typedef struct vault {
  entry *items;
  size_t count;
  size_t capacity;
} vault;

void vault_init(vault *v);
void vault_free(vault *v);
Status vault_add(vault *v , const char *service, const char *username, const char *password);

#endif