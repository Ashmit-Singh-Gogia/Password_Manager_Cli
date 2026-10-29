#include "vault/vault.h"
#include <stdio.h>
int main(void) {
  vault v;
  vault_init(&v);

  vault_free(&v);
  vault_free(&v);

  printf("Vault init and free test passed!\n");
  return 0;
}