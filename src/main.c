#include "vault/vault.h"
#include <stdio.h>

int main(void) {
  vault v;
  vault_init(&v);

  Status s = vault_add(&v, "a\tb", "user", "pass");

  if (s == STATUS_INVALID_INPUT) {
    printf("Success: vault_add correctly rejected tabs with "
           "STATUS_INVALID_INPUT!\n");
  } else {
    printf("Unexpected status: %d\n", s);
  }

  printf("Entries in vault: %zu\n", v.count); // Should be 0

  vault_free(&v);
  return 0;
}