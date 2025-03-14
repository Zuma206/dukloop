#include <duk_config.h>
#include <duktape.h>
#include <stdbool.h>
#include <stdio.h>

#define debug_log(message) printf("[DUKLOOP log] %s\n", message)

void assert(bool assertion, char *failure, char *reason) {
  if (!assertion) {
    fprintf(stderr, "[DUKLOOP failure] Failed to %s: cannot %s\n", failure,
            reason);
    exit(EXIT_FAILURE);
  }
}

char *get_script_path(int nargs, char *args[]) {
  assert(nargs == 2, "start the program", "find the script argument");
  return args[1];
}

size_t get_file_size(FILE *file) {
  size_t current_pos = ftell(file);
  char *failure = "read file";
  assert(current_pos != -1, failure, "get the current file position");
  assert(fseek(file, 0, SEEK_END) == 0, failure,
         "move the current file position");
  size_t file_size = ftell(file);
  assert(file_size != -1, failure, "get the file size value");
  assert(fseek(file, current_pos, SEEK_SET) == 0, failure,
         "cannot restore the file position");
  return file_size;
}

void get_file_content(FILE *file, char *buf, size_t size) {
  size_t n_written = fread(buf, size, 1, file);
  char *reason = "read the file content";
  assert(n_written == 1, reason, reason);
}

duk_ret_t js_println(duk_context *ctx) {
  const char *str = duk_safe_to_string(ctx, -1);
  printf("%s\n", str);
  return 0;
}

duk_ret_t js_file_stream_constructor(duk_context *ctx) {
  duk_push_this(ctx);
  duk_swap_top(ctx, -2);
  duk_put_prop_literal(ctx, -2, DUK_HIDDEN_SYMBOL("file_stream"));
  return 0;
}

duk_ret_t js_file_stream_print(duk_context *ctx) {
  const char *text = duk_get_string(ctx, -1);
  duk_push_this(ctx);
  duk_get_prop_literal(ctx, -1, DUK_HIDDEN_SYMBOL("file_stream"));
  FILE *file_stream = duk_get_pointer(ctx, -1);
  fprintf(file_stream, "%s", text);
  return 0;
}

int main(int nargs, char *args[]) {
  char *script_path = get_script_path(nargs, args);

  debug_log("opening script file");
  FILE *script_file = fopen(script_path, "r");
  assert(script_file != NULL, "open the script file", "find the file");
  size_t script_size = get_file_size(script_file);

  debug_log("reading script content");
  char *script_content = malloc(script_size);
  get_file_content(script_file, script_content, script_size);

  debug_log("setting globals");
  duk_context *ctx = duk_create_heap_default();
  duk_push_c_function(ctx, js_file_stream_constructor, 1);
  duk_push_object(ctx);
  duk_push_c_function(ctx, js_file_stream_print, 1);
  duk_put_prop_literal(ctx, -2, "print");
  duk_put_prop_literal(ctx, -2, "prototype");
  debug_log("message");
  duk_put_global_literal(ctx, "FileStream");
  duk_get_global_literal(ctx, "FileStream");
  duk_push_pointer(ctx, stdout);
  duk_new(ctx, 1);
  duk_put_global_literal(ctx, "stdout");

  debug_log("executing script");
  duk_eval_string(ctx, script_content);

  return EXIT_SUCCESS;
}