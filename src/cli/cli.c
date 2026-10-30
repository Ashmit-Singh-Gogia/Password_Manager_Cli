#include "cli.h"
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
  // Unknown command
  fprintf(stderr, "Error: unknown command '%s'\n", cmd);
  fprintf(stderr, "Run 'pm help' for usage.\n");
  return 1;
}