#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <civetweb.h>
#include "cleanup.h"
#include "task_queue.h"

// Helper: Print a message (used as a task in the cleanup task queue).
static void print_message(void* context) {
    const char* msg = (const char*)context;
    if (msg) {
        printf("%s", msg);
    }
}

// Helper to process a task queue.
static void process_task_queue(TaskQueue* queue) {
    void* taskContext = NULL;
    task_func_t task;
    while ((task = task_queue_dequeue(queue, &taskContext)) != NULL) {
        task(taskContext);
    }
}

// Cleanup function: stops the server, calls mg_exit_library, clears data structures,
// prints messages, and then checks for memory leaks.
void cleanup_and_check_leaks(struct mg_context* ctx, _CrtMemState* initialState) {
    TaskQueue cleanupQueue;
    task_queue_init(&cleanupQueue);

    // Enqueue shutdown tasks.
    task_queue_enqueue(&cleanupQueue, (task_func_t)mg_stop, ctx);                // Stop the server.
    task_queue_enqueue(&cleanupQueue, print_message, "Server stopped.\n");
    task_queue_enqueue(&cleanupQueue, (task_func_t)mg_exit_library, NULL);         // Exit CivetWeb library.
    task_queue_enqueue(&cleanupQueue, (task_func_t)clear_all_data_structures, NULL); // Clear data structures.
    task_queue_enqueue(&cleanupQueue, print_message, "Data structures cleared.\n");

    // Process all cleanup tasks.
    process_task_queue(&cleanupQueue);

    // Take a final memory snapshot and compare.
    _CrtMemState finalState, diffState;
    _CrtMemCheckpoint(&finalState);

    if (_CrtMemDifference(&diffState, initialState, &finalState)) {
        printf("Memory leaks detected:\n");
        _CrtDumpMemoryLeaks();
    }
    else {
        printf("No memory leaks detected.\n");
    }
}
