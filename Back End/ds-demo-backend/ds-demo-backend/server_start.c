#include "server_start.h"
#include "task_stack.h"      
#include "route_handlers.h"    
#include "logging.h"           
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <civetweb.h>

/*
 * Internal structure used by the tasks that set up the server.
 * Each task function will receive a pointer to one of these
 * structures as its "context" argument.
 */
typedef struct {
    struct mg_callbacks* callbacks;
    const char** options;
    struct mg_context** ctx_ptr;
} ServerStartContext;

/*
 * TaskThread to initialize the callbacks structure.
 * This sets all fields in the mg_callbacks struct to zero.
 */
static void task_initialize_callbacks(void* context) {
    struct mg_callbacks* callbacks = (struct mg_callbacks*)context;
    memset(callbacks, 0, sizeof(struct mg_callbacks));
}

/*
 * TaskThread to set the custom log access callback.
 * We use our own 'my_log_access' function for logging requests.
 */
static void task_set_log_access(void* context) {
    struct mg_callbacks* callbacks = (struct mg_callbacks*)context;
    callbacks->log_access = my_log_access;
}

/*
 * TaskThread to start the CivetWeb server.
 * We pass in a pointer to the mg_context* so this function can assign it.
 */
static void task_start_server(void* context) {
    ServerStartContext* sctx = (ServerStartContext*)context;
    *(sctx->ctx_ptr) = mg_start(sctx->callbacks, NULL, sctx->options);
}

/*
 * TaskThread to check if the server started successfully.
 * If ctx is NULL, it indicates the server failed to start,
 * so we terminate the program.
 */
static void task_check_server(void* context) {
    struct mg_context** ctx_ptr = (struct mg_context**)context;
    if (*ctx_ptr == NULL) {
        printf("Error: Could not start CivetWeb.\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * TaskThread to register endpoints after the server context
 * has been successfully created.
 */
static void task_register_endpoints(void* context) {
    struct mg_context** ctx_ptr = (struct mg_context**)context;
    register_endpoints(*ctx_ptr);
}

/*
 * -------------------------------------------------------------------------
 * Public function: start_server
 *
 * This function creates a stack of tasks, each of which depends on the
 * prior tasks having completed successfully. Since these tasks are
 * strictly dependent on each other in sequence (e.g., we cannot register
 * endpoints before starting the server), we do not need a thread pool here.
 *
 * The tasks are pushed in reverse order so that, when we pop them off,
 * they execute in the correct sequence:
 *  1) task_initialize_callbacks
 *  2) task_set_log_access
 *  3) task_start_server
 *  4) task_check_server
 *  5) task_register_endpoints
 *
 * Because of these linear dependencies, parallel execution would not
 * provide a benefit and might even cause errors if tasks ran out of order.
 * Hence, we use a simple stack-based approach rather than a thread pool.
 * -------------------------------------------------------------------------
 */
ServerResources start_server(const char** options) {
    // We'll store both ctx and callbacks in ServerResources
    ServerResources resources;
    resources.ctx = NULL;

    // Allocate the mg_callbacks structure
    resources.callbacks = (struct mg_callbacks*)malloc(sizeof(struct mg_callbacks));
    if (!resources.callbacks) {
        fprintf(stderr, "Failed to allocate memory for mg_callbacks\n");
        exit(EXIT_FAILURE);
    }

    // Prepare the context structure for tasks
    ServerStartContext sctx;
    sctx.callbacks = resources.callbacks;
    sctx.options = options;
    sctx.ctx_ptr = &resources.ctx;

    // Create a task stack for server initialization
    TaskStack serverStack;
    task_stack_init(&serverStack);

    /*
     * The order of tasks is critical for proper server startup.
     * Each task depends on the previous task's success, so we push them
     * in reverse. They will pop off and execute in the correct order.
     */

    task_stack_push(&serverStack, task_register_endpoints, &resources.ctx);
    task_stack_push(&serverStack, task_check_server, &resources.ctx);
    task_stack_push(&serverStack, task_start_server, &sctx);
    task_stack_push(&serverStack, task_set_log_access, resources.callbacks);
    task_stack_push(&serverStack, task_initialize_callbacks, resources.callbacks);

    // Execute the tasks by popping them off the stack
    void* tsContext = NULL;
    task_func_t tsTask = NULL;
    while ((tsTask = task_stack_pop(&serverStack, &tsContext)) != NULL) {
        tsTask(tsContext);
    }
    task_stack_clear(&serverStack);

    // Return both the context and the callbacks pointer for further usage
    return resources;
}


/*
Task Queue
Decouple scheduling from execution: You can enqueue tasks anywhere in your code and process them later or in another thread, making it easy to manage tasks without blocking.

Flexible ordering or prioritization: A queue can be extended to handle priorities, retry mechanisms, or rate limits.

Centralized management: All tasks flow through a single structure, simplifying logging, error handling, and performance monitoring.

Thread Pool
Efficient parallel execution: A pool of worker threads can run multiple tasks concurrently, improving performance if tasks are independent.

Avoid thread?creation overhead: Reusing a fixed set of threads is more efficient than frequently creating and destroying threads.

Scalable concurrency: Adjust the number of threads (pool size) according to hardware resources and workload needs.

Stack (LIFO)?based Tasks
Strict ordering / dependencies: If tasks must run in a specific, reversed order of dependency, a stack naturally enforces Last?In?First?Out execution.

Simplicity: For a linear chain of dependent tasks, a stack is easier to implement and reason about than full concurrency controls.

Minimal overhead: You don’t need synchronization primitives or multiple threads if tasks must run in a strict, sequential manner anyway.
*/