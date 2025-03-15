#ifndef DLOOP_FILESTREAMS_H
#define DLOOP_FILESTREAMS_H

#include <duktape.h>

/*
  Initialise the full FileStream API on the global scope.
  Includes FileStream, stdout, and stderr
*/
void dloop_file_stream_api_init(duk_context *ctx);

#endif