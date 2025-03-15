#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void dloop_assert(bool assertion, char *failure, char *reason) {
  if (assertion)
    return;
  fprintf(stderr, "Failed to %s: %s\n", failure, reason);
  exit(EXIT_FAILURE);
}