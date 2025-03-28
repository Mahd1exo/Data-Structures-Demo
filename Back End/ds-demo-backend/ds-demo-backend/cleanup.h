#ifndef CLEANUP_H
#define CLEANUP_H

#include <crtdbg.h>
#include <civetweb.h>
#include "route_handlers.h"



// Shuts down the server, performs cleanup tasks, and checks for memory leaks.
void cleanup_and_check_leaks(struct mg_context* ctx, _CrtMemState* initialState);

#endif  // CLEANUP_H
