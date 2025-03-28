#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#include <civetweb.h>
#include "server_start.h"      // For start_server()
#include "cleanup.h"           // For cleanup_and_check_leaks()
#include "logging.h"           // For show_logs(), etc.
#include "task_queue.h"        // For TaskQueue usage
#include "stack_ops.h"         // For stack usage
#include "route_handlers.h"    // For register_endpoints

// Enum for menu options
enum MenuOption {
    EXIT = 1,
    SHOW_LOGS,
    SHOW_HISTORY,
    CLEAN_MEMORY = 1234
};

// Helper: Process tasks in a task queue
static void process_task_queue(TaskQueue* queue) {
    void* taskContext = NULL;
    task_func_t task;
    while ((task = task_queue_dequeue(queue, &taskContext)) != NULL) {
        task(taskContext);
    }
}

// Display a small menu
static void show_menu(void) {
    printf("\n======== DEBUGGING MENU ========\n");
    printf("1. Exit server and check for memory leaks\n");
    printf("2. Show logs (view in-memory log buffer)\n");
    printf("3. Show operation history\n");
    printf("1234. Clean Memory (for testing; clears data structures)\n");
    printf("!!! CLEAN MEMORY IS FOR TESTING MEMORY LEAKS, NOT FOR REGULAR OPERATION !!!\n");
    printf("Enter option: ");
}

int main(int argc, char* argv[]) {
    // Initial memory snapshot for leak detection
    _CrtMemState initialState;
    _CrtMemCheckpoint(&initialState);

    // A main task queue
    TaskQueue mainQueue;
    task_queue_init(&mainQueue);

    // A stack to keep track of commands (optional)
    Stack commandStack;
    stack_init(&commandStack);

    // CivetWeb server options
    const char* options[] = {
        "listening_ports", "8080",
        "num_threads", "2",
        "access_log_file", "access.log",
        NULL
    };

    // Start the server (returns both ctx and callbacks)
    ServerResources resources = start_server(options);
    if (!resources.ctx) {
        printf("Error: Could not start server.\n");
        return 1;
    }
    printf("Server started on port 8080.\n");

    bool exitFlag = false;
    while (!exitFlag) {
        show_menu();

        int option;
        if (scanf("%d", &option) != 1) {
            // Clear invalid input
            while (getchar() != '\n');
            continue;
        }

        switch (option) {
        case EXIT:
            exitFlag = true;
            break;

        case SHOW_LOGS:
            // Add a command to the stack
            stack_push(&commandStack, "Show Logs");
            // Enqueue a task to show logs
            task_queue_enqueue(&mainQueue, (task_func_t)show_logs, NULL);
            break;

        case SHOW_HISTORY:
            stack_push(&commandStack, "Show Operation History");
            // Print the stack of commands
            task_queue_enqueue(&mainQueue, (task_func_t)print_stack, &commandStack);
            break;

        case CLEAN_MEMORY:
            stack_push(&commandStack, "Starting Cleaning Memory...");
            task_queue_enqueue(&mainQueue, (task_func_t)clear_all_data_structures, NULL);
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Memory cleaned.\n");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "NOTE: After cleaning memory, you must reinitialize data structures.\n");
            stack_push(&commandStack, "Cleaned Memory");
            break;

        default:
            stack_push(&commandStack, "Invalid Option");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Invalid option. Please try again.\n");
            break;
        }

        // Process any enqueued tasks
        process_task_queue(&mainQueue);
    }

    // Clean up the main queue and stack
    task_queue_clear(&mainQueue);
    stack_free(&commandStack);

    // Final cleanup: stop the server, free callbacks, check for leaks
    cleanup_and_check_leaks(resources, &initialState);

    return 0;
}
