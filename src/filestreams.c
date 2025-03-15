#include <duktape.h>
#include <stdio.h>

static const char *FILE_STREAM_PROP = DUK_HIDDEN_SYMBOL("file_stream");

static duk_ret_t file_stream(duk_context *ctx) {
  duk_push_this(ctx);
  duk_swap_top(ctx, -2);
  duk_put_prop_literal(ctx, -2, FILE_STREAM_PROP);
  return 0;
}

static duk_ret_t file_stream_print(duk_context *ctx) {
  const char *text = duk_get_string(ctx, -1);
  duk_push_this(ctx);
  duk_get_prop_literal(ctx, -1, FILE_STREAM_PROP);
  FILE *file_stream = duk_get_pointer(ctx, -1);
  fprintf(file_stream, "%s", text);
  return 0;
}

static void file_stream_init(duk_context *ctx) {
  duk_push_c_function(ctx, file_stream, 1);
  duk_push_object(ctx);
  duk_push_c_function(ctx, file_stream_print, 1);
  duk_put_prop_literal(ctx, -2, "print");
  duk_put_prop_literal(ctx, -2, "prototype");
  duk_put_global_literal(ctx, "FileStream");
}

static void file_stream_new_global(duk_context *ctx, char *name, FILE *file) {
  duk_get_global_literal(ctx, "FileStream");
  duk_push_pointer(ctx, file);
  duk_new(ctx, 1);
  duk_put_global_string(ctx, name);
}

void dloop_file_stream_api_init(duk_context *ctx) {
  file_stream_init(ctx);
  file_stream_new_global(ctx, "stdout", stdout);
  file_stream_new_global(ctx, "stderr", stderr);
}