#ifndef SERVER_START_H
#define SERVER_START_H

#include <civetweb.h>

// Function to start the server.
// Takes the server options (an array of strings) and returns the server context.
struct mg_context* start_server(const char** options);

#endif // SERVER_START_H
