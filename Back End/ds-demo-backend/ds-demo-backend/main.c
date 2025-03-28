#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <civetweb.h>
#include <stdbool.h>

#include "route_handlers.h"

#include "task_queue.h"
#include "cleanup.h"
#include "logging.h"

// For non-blocking keyboard input and sleep on Windows.
#include <conio.h>
#include <windows.h>

// Enum for main menu options.
enum MenuOption {
    EXIT = 1,
    SHOW_LOGS = 2,
    CLEAN_MEMORY = 1234
};

// Helper: Process tasks in a task queue.
static void process_task_queue(TaskQueue* queue) {
    void* taskContext = NULL;
    task_func_t task;
    while ((task = task_queue_dequeue(queue, &taskContext)) != NULL) {
        task(taskContext);
    }
}

int main(int argc, char* argv[]) {
    _CrtMemState initialState;
    _CrtMemCheckpoint(&initialState);

    TaskQueue mainQueue;
    task_queue_init(&mainQueue);

    // CivetWeb options, including access_log_file for file logging.
    const char* options[] = {
        "listening_ports", "8080",
        "num_threads", "2",
        "access_log_file", "access.log",
        NULL
    };

    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    // Set our custom access log callback from the logging module.
    callbacks.log_access = my_log_access;

    struct mg_context* ctx = mg_start(&callbacks, NULL, options);
    if (!ctx) {
        printf("Error: Could not start CivetWeb on port 8080.\n");
        return 1;
    }

    // Register endpoints.
    register_endpoints(ctx);
    printf("Server started on port 8080.\n");

    bool exitFlag = false;
    while (!exitFlag) {
        printf("\n======== DEBUGGING MENU ========\n");
        printf("1. Exit server and check for memory leaks\n");
        printf("2. Show logs (view in-memory log buffer)\n");
        printf("1234. Clean Memory (for testing; clears data structures)\n");
        printf("!!! CLEAN MEMORY IS FOR TESTING MEMORY LEAKS, NOT FOR REGULAR OPERATION !!!\n");
        printf("Enter option: ");

        int option;
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');  // Clear invalid input.
            continue;
        }

        switch (option) {
        case EXIT:
            exitFlag = true;
            break;
        case SHOW_LOGS:
            show_logs();
            break;
        case CLEAN_MEMORY:
            task_queue_enqueue(&mainQueue, (task_func_t)clear_all_data_structures, NULL);
            printf("Memory cleaned.\n");
            printf("NOTE: After cleaning memory, you must reinitialize some data structures.\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
        process_task_queue(&mainQueue);
    }

    // Shutdown the server and check for memory leaks.
    cleanup_and_check_leaks(ctx, &initialState);

    return 0;
}
