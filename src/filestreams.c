#include <duk_config.h>
#include <duktape.h>
#include <stdio.h>

/* A hidden symbol for accessing the FILE pointer for a FileStream class */
static const char *FILE_STREAM_PROP = DUK_HIDDEN_SYMBOL("file_stream");

/* The FileStream class constructor function */
static duk_ret_t file_stream(duk_context *ctx) {
  duk_push_this(ctx);
  duk_swap_top(ctx, -2);
  duk_put_prop_literal(ctx, -2, FILE_STREAM_PROP);
  return 0;
}

/* A common C function for the FileStream print and println methods */
static void file_stream_print_c(duk_context *ctx, char *end) {
  const char *text = duk_get_string(ctx, -1);
  duk_push_this(ctx);
  duk_get_prop_literal(ctx, -1, FILE_STREAM_PROP);
  FILE *file_stream = duk_get_pointer(ctx, -1);
  fprintf(file_stream, "%s%s", text, end);
}

/* The FileStream print method */
static duk_ret_t file_stream_print(duk_context *ctx) {
  file_stream_print_c(ctx, "");
  return 0;
}

/* The FileStream println method */
static duk_ret_t file_stream_println(duk_context *ctx) {
  file_stream_print_c(ctx, "\n");
  return 0;
}

/* Creates the global FileStream class */
static void file_stream_init(duk_context *ctx) {
  duk_push_c_function(ctx, file_stream, 1);
  duk_push_object(ctx);
  duk_push_c_function(ctx, file_stream_print, 1);
  duk_put_prop_literal(ctx, -2, "print");
  duk_push_c_function(ctx, file_stream_println, 1);
  duk_put_prop_literal(ctx, -2, "println");
  duk_put_prop_literal(ctx, -2, "prototype");
  duk_put_global_literal(ctx, "FileStream");
}

/* Creates a global instance of the FileStream class bound to a FILE pointer */
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