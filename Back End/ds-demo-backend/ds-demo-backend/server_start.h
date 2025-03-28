#ifndef SERVER_START_H
#define SERVER_START_H

#include <civetweb.h>

typedef struct ServerResources {
    struct mg_context* ctx;
    struct mg_callbacks* callbacks;
} ServerResources;

ServerResources start_server(const char** options);

#endif  // SERVER_START_H
