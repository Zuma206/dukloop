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

int main(int nargs, char *args[]) {
  char *script_path = get_script_path(nargs, args);

  debug_log("opening script file");
  FILE *script_file = fopen(script_path, "r");
  assert(script_file != NULL, "open the script file", "find the file");
  size_t script_size = get_file_size(script_file);

  debug_log("reading script content");
  char *script_content = malloc(script_size);
  get_file_content(script_file, script_content, script_size);

  debug_log("executing script");
  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, script_content);
  double result = duk_get_number(ctx, -1);
  printf("Result: %f\n", result);
  return EXIT_SUCCESS;
}