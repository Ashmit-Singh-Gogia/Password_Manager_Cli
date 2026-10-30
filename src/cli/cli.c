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

// There can be some issues with limit in scanf so we are using this custom
// handler for it
static void read_line(const char *prompt, char *buf, size_t size) {
  printf("%s", prompt);
  fgets(buf, (int)size, stdin);
  size_t len = strlen(buf);
  if (len > 0 && buf[len - 1] == '\n') {
    buf[len - 1] = '\0';
  }
}

static int cmd_add(void) {
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

  char service[SERVICE_MAX_LEN];
  char username[USERNAME_MAX_LEN];
  char password[PASSWORD_MAX_LEN];

  read_line("Service:  ", service, sizeof(service));
  read_line("Username: ", username, sizeof(username));
  read_line("Password: ", password, sizeof(password));

  Status s = vault_add(&v, service, username, password);
  if (s == STATUS_OK) {
    if (storage_save(&v, path) != STATUS_OK) {
      fprintf(stderr, "Error: could not save vault.\n");
      vault_free(&v);
      return 1;
    }
    printf("Saved '%s'.\n", service);
  } else if (s == STATUS_DUPLICATE) {
    fprintf(stderr, "Error: '%s' already exists.\n", service);
  } else if (s == STATUS_TOO_LONG) {
    fprintf(stderr, "Error: one or more fields are too long.\n");
  } else if (s == STATUS_INVALID_INPUT) {
    fprintf(stderr,
            "Error: fields cannot be empty or contain tabs/newlines.\n");
  } else {
    fprintf(stderr, "Error: could not add entry (status %d).\n", s);
  }

  vault_free(&v);
  return (s == STATUS_OK) ? 0 : 1;
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

static int cmd_get(const char *service, int show_password) {
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

  entry *e = vault_find(&v, service);
  if (!e) {
    fprintf(stderr, "Error: '%s' not found.\n", service);
    vault_free(&v);
    return 1;
  }

  printf("Service:  %s\n", e->service);
  printf("Username: %s\n", e->username);
  if (show_password) {
    printf("Password: %s\n", e->password);
  } else {
    printf("Password: (hidden — use 'pm get %s --show' to reveal)\n", service);
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
  if (strcmp(cmd, "add") == 0) {
    return cmd_add();
  }
  if (strcmp(cmd, "get") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Usage: pm get <service> [--show]\n");
      return 1;
    }
    int show = (argc >= 4 && strcmp(argv[3], "--show") == 0);
    return cmd_get(argv[2], show);
  }
  // Unknown command
  fprintf(stderr, "Error: unknown command '%s'\n", cmd);
  fprintf(stderr, "Run 'pm help' for usage.\n");
  return 1;
}