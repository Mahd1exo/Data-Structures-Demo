#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <civetweb.h>

#include "cleanup.h"
#include "task_queue.h"
#include "thread_pool.h" 


// Helper: Print a message (used as a task in the cleanup queue)
static void print_message(void* context) {
    const char* msg = (const char*)context;
    if (msg) {
        printf("%s", msg);
    }
}

// -------------------------------------------------------------------
// The main cleanup function that stops the server, calls mg_exit_library,
// frees the callbacks pointer, and checks for memory leaks.
void cleanup_and_check_leaks(ServerResources resources, _CrtMemState* initialState) {
    struct mg_context* ctx = resources.ctx;
    struct mg_callbacks* callbacks = resources.callbacks;

    // 1) Create a task queue for cleanup tasks
    TaskQueue cleanupQueue;
    task_queue_init(&cleanupQueue);

    // 2) Enqueue tasks (stop server, exit library, clear data, etc.)
    task_queue_enqueue(&cleanupQueue, (task_func_t)mg_stop, ctx);
    task_queue_enqueue(&cleanupQueue, print_message, "Server stopped.\n");
    task_queue_enqueue(&cleanupQueue, (task_func_t)mg_exit_library, NULL);
    task_queue_enqueue(&cleanupQueue, (task_func_t)clear_all_data_structures, NULL);
    task_queue_enqueue(&cleanupQueue, print_message, "Data structures cleared.\n");

    // 3) Initialize a thread pool (e.g., with 4 worker threads)
    ThreadPool cleanupPool;
    if (thread_pool_init(&cleanupPool, 4) != 0) {
        fprintf(stderr, "Failed to initialize thread pool.\n");
        // If thread pool creation fails, optionally process synchronously:
        process_task_queue(&cleanupQueue);
        task_queue_clear(&cleanupQueue);
        free(callbacks);
        return; 
    }
    else {
        // 4) Dequeue each task and add it to the thread pool
        void* taskContext = NULL;
        task_func_t task = NULL;
        while ((task = task_queue_dequeue(&cleanupQueue, &taskContext)) != NULL) {
            thread_pool_add_task(&cleanupPool, task, taskContext);
        }

        // 5) Destroy the pool, which waits for all tasks to finish
        thread_pool_destroy(&cleanupPool);
    }

    // 6) Clear the task queue (though by now it should be empty)
    task_queue_clear(&cleanupQueue);

    // 7) Free the mg_callbacks pointer (allocated in start_server)
    free(callbacks);
    //for checking memory leaks
    //int* leak = malloc(100);

    // 8) Perform the final memory snapshot and leak check
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
