#ifndef CLEANUP_H
#define CLEANUP_H

#include <crtdbg.h>
#include "server_start.h"  // For ServerResources
#include "route_handlers.h"	


/*
 * Shuts down the server, frees resources, and checks for memory leaks.
 * - stops the server
 * - calls mg_exit_library
 * - frees callbacks
 * - calls clear_all_data_structures
 * - performs final leak check
 */
void cleanup_and_check_leaks(ServerResources resources, _CrtMemState* initialState);

#endif  // CLEANUP_H
