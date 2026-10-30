#include "storage/storage.h"
#include "vault/vault.h"
#include <stdio.h>

int main(void) {
  const char *file = "test_vault.txt";

  // 1. Add and save
  vault v;
  vault_init(&v);
  vault_add(&v, "github", "ashmit", "secret123");
  storage_save(&v, file);
  vault_free(&v);

  // 2. Load back
  vault loaded;
  vault_init(&loaded);
  storage_load(&loaded, file);

  // 3. Find by service and print
  entry *e = vault_find(&loaded, "github");
  if (e) {
    printf("Loaded: %s | %s | %s\n", e->service, e->username, e->password);
  } else {
    printf("Not found!\n");
  }

  vault_free(&loaded);
  return 0;
}