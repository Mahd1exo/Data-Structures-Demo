#include "server_start.h"
#include "task_stack.h"
#include "route_handlers.h"
#include "logging.h"   // For my_log_access
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <civetweb.h>

// Define a structure to pass parameters to the server start task.
typedef struct {
    struct mg_callbacks* callbacks;
    const char** options;
    struct mg_context** ctx_ptr;  // Where the server context will be stored.
} ServerStartContext;

// Task to initialize the callback structure.
static void task_initialize_callbacks(void* context) {
    struct mg_callbacks* callbacks = (struct mg_callbacks*)context;
    memset(callbacks, 0, sizeof(struct mg_callbacks));
}

// Task to set the custom log access callback.
static void task_set_log_access(void* context) {
    struct mg_callbacks* callbacks = (struct mg_callbacks*)context;
    callbacks->log_access = my_log_access;
}

// Task to start the CivetWeb server.
static void task_start_server(void* context) {
    ServerStartContext* sctx = (ServerStartContext*)context;
    *(sctx->ctx_ptr) = mg_start(sctx->callbacks, NULL, sctx->options);
}

// Task to check if the server started successfully.
static void task_check_server(void* context) {
    struct mg_context** ctx_ptr = (struct mg_context**)context;
    if (*ctx_ptr == NULL) {
        printf("Error: Could not start CivetWeb on port 8080.\n");
        exit(EXIT_FAILURE);
    }
}

// Task to register endpoints.
static void task_register_endpoints(void* context) {
    struct mg_context** ctx_ptr = (struct mg_context**)context;
    register_endpoints(*ctx_ptr);
}

// The public function to start the server.
struct mg_context* start_server(const char** options) {
    // Allocate a callbacks structure.
    struct mg_callbacks* callbacks = malloc(sizeof(struct mg_callbacks));
    if (callbacks == NULL) {
        fprintf(stderr, "Failed to allocate memory for callbacks\n");
        exit(EXIT_FAILURE);
    }
    struct mg_context* ctx = NULL;

    // Prepare the server start context.
    ServerStartContext sctx;
    sctx.callbacks = callbacks;
    sctx.options = options;
    sctx.ctx_ptr = &ctx;

    // Create and initialize a task stack for server initialization.
    TaskStack serverStack;
    task_stack_init(&serverStack);

    // Push tasks in reverse order so that they execute in the desired sequence.
    task_stack_push(&serverStack, task_register_endpoints, &ctx);
    task_stack_push(&serverStack, task_check_server, &ctx);
    task_stack_push(&serverStack, task_start_server, &sctx);
    task_stack_push(&serverStack, task_set_log_access, callbacks);
    task_stack_push(&serverStack, task_initialize_callbacks, callbacks);

    /*
     * Note:
     * The order of tasks is critical for proper server startup.
     * Each task depends on the successful execution of the previous tasks,
     * creating a chain of nested dependencies.
     * If the tasks are not executed in the correct order, the server may not start properly.
     */

    // Process the server initialization tasks.
    void* tsContext = NULL;
    task_func_t tsTask;
    while ((tsTask = task_stack_pop(&serverStack, &tsContext)) != NULL) {
        tsTask(tsContext);
    }
    task_stack_clear(&serverStack);

    // Return the server context.
    return ctx;
}
