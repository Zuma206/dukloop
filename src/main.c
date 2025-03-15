#include "filestreams.h"
#include "utils.h"

#include <duktape.h>
#include <stdbool.h>
#include <stdio.h>

static char *get_script_path(int nargs, char *args[]) {
  dloop_assert(nargs == 2, "get the script path",
               "incorrect number of args (expected 1)");
  return args[1];
}

static size_t get_file_size(FILE *file) {
  size_t current_pos = ftell(file);
  char *failure = "read the script file";
  dloop_assert(current_pos != -1, failure,
               "cannot get the current file position");
  dloop_assert(fseek(file, 0, SEEK_END) == 0, failure,
               "cannot move the current file position");
  size_t file_size = ftell(file);
  dloop_assert(file_size != -1, failure, "cannot get the file size value");
  dloop_assert(fseek(file, current_pos, SEEK_SET) == 0, failure,
               "cannot restore the file position");
  return file_size;
}

static void get_file_content(FILE *file, char *buf, size_t size) {
  size_t n_written = fread(buf, size, 1, file);
  dloop_assert(n_written == 1, "read the script file contents",
               "cannot read the file");
}

int main(int nargs, char *args[]) {
  char *script_path = get_script_path(nargs, args);
  FILE *script_file = fopen(script_path, "r");
  dloop_assert(script_file != NULL, "open the script file",
               "cannot find the file");
  size_t script_size = get_file_size(script_file);
  char *script_content = malloc(script_size);
  get_file_content(script_file, script_content, script_size);

  duk_context *ctx = duk_create_heap_default();
  dloop_file_stream_api_init(ctx);
  duk_eval_string(ctx, script_content);

  return EXIT_SUCCESS;
}