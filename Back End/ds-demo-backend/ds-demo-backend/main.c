#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <civetweb.h>
#include <stdbool.h>
#include <conio.h>
#include <windows.h>

#include "route_handlers.h"
#include "task_queue.h"
#include "stack_ops.h"  

#include "cleanup.h"
#include "logging.h"
#include "server_start.h" 

// Enum for main menu options.
enum MenuOption {
    EXIT = 1,
    SHOW_LOGS = 2,
    SHOW_HISTORY = 3,
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

static void menu() {
    printf("\n======== DEBUGGING MENU ========\n");
    printf("1. Exit server and check for memory leaks\n");
    printf("2. Show logs (view in-memory log buffer)\n");
    printf("3. Show operation history\n");
    printf("1234. Clean Memory (for testing; clears data structures)\n");
    printf("!!! CLEAN MEMORY IS FOR TESTING MEMORY LEAKS, NOT FOR REGULAR OPERATION !!!\n");
    printf("Enter option: ");
}

int main(int argc, char* argv[]) {
    _CrtMemState initialState;
    _CrtMemCheckpoint(&initialState);

    TaskQueue mainQueue;
    task_queue_init(&mainQueue);

    Stack commandStack;
    stack_init(&commandStack);

    // CivetWeb options.
    const char* options[] = {
        "listening_ports", "8080",
        "num_threads", "2",
        "access_log_file", "access.log",
        NULL
    };

    // Start the server using the dedicated module.
    struct mg_context* ctx = start_server(options);
    if (!ctx) {
        printf("Error: Could not start server.\n");
        return 1;
    }
    printf("Server started on port 8080.\n");

    bool exitFlag = false;
    while (!exitFlag) {
		menu();
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
            stack_push(&commandStack, "Show Logs");
            task_queue_enqueue(&mainQueue, (task_func_t)show_logs, NULL);
            break;
        case SHOW_HISTORY:
            stack_push(&commandStack, "Show Operation History");
            task_queue_enqueue(&mainQueue, (task_func_t)print_stack, &commandStack);
            break;
        case CLEAN_MEMORY:
            stack_push(&commandStack, "Starting Cleaning Memory...");
            task_queue_enqueue(&mainQueue, (task_func_t)clear_all_data_structures, NULL);
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Memory cleaned.\n");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "NOTE: After cleaning memory, you must reinitialize some data structures.\n");
            stack_push(&commandStack, "Cleaned Memory");
            break;
        default:
			stack_push(&commandStack, "Invalid Option");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Invalid option. Please try again.\n");
            break;
        }
        process_task_queue(&mainQueue);
    }

    // Shutdown the server and check for memory leaks using a task queue.
    cleanup_and_check_leaks(ctx, &initialState);

    // Free the operation history stack.
    stack_free(&commandStack);

    return 0;
}
