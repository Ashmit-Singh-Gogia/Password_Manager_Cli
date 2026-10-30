#include "auth.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASTER_MAX_LEN 128

// builds the path for saving the master password
static Status get_master_path(char *buf, size_t size) {
  const char *home = getenv("HOME");
  if (!home || home[0] == '\0') {
    return STATUS_IO_ERROR;
  }
  int w = snprintf(buf, size, "%s/.pm_master", home);
  if (w < 0 || (size_t)w >= size) {
    return STATUS_TOO_LONG;
  }
  return STATUS_OK;
}

Status auth_setup(void) {
  char path[PATH_MAX];
  if (get_master_path(path, sizeof(path)) != STATUS_OK) {
    fprintf(stderr, "Error: could not resolve master password path.\n");
    return STATUS_IO_ERROR;
  }

  // If file already exists, setup is already done
  FILE *f = fopen(path, "r");
  if (f) {
    fclose(f);
    return STATUS_OK;
  }

  char pass1[MASTER_MAX_LEN];
  char pass2[MASTER_MAX_LEN];

  printf("Set your master password.\n");
  printf("Master password: ");
  fgets(pass1, sizeof(pass1), stdin);

  printf("Confirm password: ");
  fgets(pass2, sizeof(pass2), stdin);

  // Strip newlines
  pass1[strcspn(pass1, "\n")] = '\0';
  pass2[strcspn(pass2, "\n")] = '\0';

  if (strcmp(pass1, pass2) != 0) {
    fprintf(stderr, "Error: passwords do not match.\n");
    return STATUS_INVALID_INPUT;
  }

  // Save to file
  FILE *out = fopen(path, "w");
  if (!out) {
    fprintf(stderr, "Error: could not save master password.\n");
    return STATUS_IO_ERROR;
  }
  fprintf(out, "%s\n", pass1);
  fclose(out);

  printf("Master password set!\n");
  return STATUS_OK;
}

Status auth_check(void) {
  char path[PATH_MAX];
  if (get_master_path(path, sizeof(path)) != STATUS_OK) {
    return STATUS_IO_ERROR;
  }

  // Read stored password from file
  FILE *f = fopen(path, "r");
  if (!f) {
    fprintf(stderr,
            "Error: master password not set. Run any command to set it.\n");
    return STATUS_IO_ERROR;
  }

  char stored[MASTER_MAX_LEN];
  if (!fgets(stored, sizeof(stored), f)) {
    fclose(f);
    return STATUS_IO_ERROR;
  }
  fclose(f);
  stored[strcspn(stored, "\n")] = '\0';

  // Ask user to enter it
  char entered[MASTER_MAX_LEN];
  printf("Master password: ");
  fgets(entered, sizeof(entered), stdin);
  entered[strcspn(entered, "\n")] = '\0';

  if (strcmp(entered, stored) != 0) {
    fprintf(stderr, "Error: wrong password.\n");
    return STATUS_INVALID_INPUT;
  }

  return STATUS_OK;
}