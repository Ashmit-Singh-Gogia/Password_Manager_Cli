#include "storage/storage.h"
#include "vault/vault.h"
#include <limits.h>
#include <stdio.h>

int main(void) {
  // 1. Resolve the home-directory vault path
  char path[PATH_MAX];
  if (storage_get_path(path, sizeof(path)) != STATUS_OK) {
    fprintf(stderr, "Error resolving vault path\n");
    return 1;
  }
  printf("Vault path: %s\n", path);

  // 2. Add an entry and save to the vault file
  vault v;
  vault_init(&v);
  vault_add(&v, "github", "ashmit", "secret123");
  storage_save(&v, path);
  vault_free(&v);

  // 3. Load back into a fresh vault
  vault loaded;
  vault_init(&loaded);
  storage_load(&loaded, path);

  // 4. Find by service name and print
  entry *e = vault_find(&loaded, "github");
  if (e) {
    printf("Loaded: %s | %s | %s\n", e->service, e->username, e->password);
  } else {
    printf("Not found!\n");
  }

  vault_free(&loaded);
  return 0;
}
