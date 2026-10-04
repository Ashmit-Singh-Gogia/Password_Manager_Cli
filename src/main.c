/* main.c - temporary test version. */
#include "common.h"
#include "vault.h"
#include <stdio.h>

int main(void) {
  credential c;
  pm_status s = credential_init(&c, "gmail", "me@example.com", "s3cret", NULL);
  printf("good entry : %s\n", pm_status_str(s));
  if (s == PM_OK) {
    printf("service='%s' url='%s'\n", c.service, c.url);
    credential_free(&c);
  }

  s = credential_init(&c, "gmail", "", "s3cret", NULL);
  printf("empty user : %s\n", pm_status_str(s));
  return 0;
}