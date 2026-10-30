#include "cli.h"
#include "../storage/storage.h"
#include "../vault/vault.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

static void print_help(void) {
  printf("Usage: pm <command> [args]\n\n");
  printf("Commands:\n");
  printf("  help              Show this help message\n");
  printf("  list              List all saved services\n");
  printf("  add               Add a new entry\n");
  printf("  get <service>     Get credentials for a service\n");
  printf("  delete <service>  Delete an entry\n");
}

static int cmd_list(void) {
  char path[PATH_MAX];
  if (storage_get_path(path, sizeof(path)) != STATUS_OK) {
    fprintf(stderr, "Error: could not resolve vault path.\n");
    return 1;
  }

  vault v;
  vault_init(&v);

  if (storage_load(&v, path) != STATUS_OK) {
    fprintf(stderr, "Error: could not load vault.\n");
    vault_free(&v);
    return 1;
  }

  if (v.count == 0) {
    printf("No entries saved yet. Use 'pm add' to add one.\n");
  } else {
    for (size_t i = 0; i < v.count; i++) {
      printf("  %s\n", v.items[i].service);
    }
  }

  vault_free(&v);
  return 0;
}

int cli_run(int argc, char **argv) {
  // No command given → show help
  if (argc < 2) {
    print_help();
    return 0;
  }
  const char *cmd = argv[1];
  if (strcmp(cmd, "help") == 0) {
    print_help();
    return 0;
  }
  if (strcmp(cmd, "list") == 0) {
    return cmd_list();
  }
  // Unknown command
  fprintf(stderr, "Error: unknown command '%s'\n", cmd);
  fprintf(stderr, "Run 'pm help' for usage.\n");
  return 1;
}