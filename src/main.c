#include <duktape.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, "2 * 8");
  double result = duk_get_number(ctx, -1);
  printf("Result: %f\n", result);
  return EXIT_SUCCESS;
}