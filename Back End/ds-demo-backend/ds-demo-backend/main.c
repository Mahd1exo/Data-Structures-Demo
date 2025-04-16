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
    CLEAN_CONSOLE= 0,
    EXIT,
    SHOW_LOGS,
    SHOW_HISTORY,
    CLEAN_MEMORY = 1234
};

// Display a small menu
static void show_menu(void) {
    printf("\n<<<======== ADMIN MENU ========>>>\n");
    printf("0. Clean Console\n");
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
    stack_ops_init(&commandStack);

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
        case CLEAN_CONSOLE:
            system("cls");
            break;
        case EXIT:
            exitFlag = true;
            break;

        case SHOW_LOGS:
            // Add a command to the stack
            stack_ops_push(&commandStack, "Show Logs");
            // Enqueue a task to show logs
            // For SHOW_LOGS, run directly on the main thread since _kbhit() and _getch() are best used there.
            task_queue_enqueue(&mainQueue, (task_func_t)show_logs, NULL);  // Call synchronously
            break;

        case SHOW_HISTORY:
            stack_ops_push(&commandStack, "Show Operation History");
            // Print the stack of commands
            task_queue_enqueue(&mainQueue, (task_func_t)print_stack_ops, &commandStack);
            break;

        case CLEAN_MEMORY:
            stack_ops_push(&commandStack, "Starting Cleaning Memory...");
            task_queue_enqueue(&mainQueue, (task_func_t)clear_all_data_structures, NULL);
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Memory cleaned.\n");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "NOTE: After cleaning memory, you must reinitialize data structures.\n");
            stack_ops_push(&commandStack, "Cleaned Memory");
            break;

        default:
            stack_ops_push(&commandStack, "Invalid Option");
            task_queue_enqueue(&mainQueue, (task_func_t)printf, "Invalid option. Please try again.\n");
            break;
        }

        // Process any enqueued tasks
        process_task_queue(&mainQueue);
    }

    // Clean up the main queue and stack
    task_queue_clear(&mainQueue);
    stack_ops_free(&commandStack);

    // Test memory dig
    //int* leak = malloc(100);
    
    // Final cleanup: stop the server, free callbacks, check for leaks
    cleanup_and_check_leaks(resources, &initialState);

    return 0;
}
/*
* why?
*  1.Resource Management.a
*  2.Potential for Asynchronous or Multi-threaded Execution.
*  3.Centralized Error Handling and Monitoring.
*  4.Dynamic Task Management.
*  5.Flexibility and Extensibility.
*  6.Decoupling Task Scheduling from Execution
*/
//When to Use Which Approach 
//    ??Use the Task Queue if 
//    ??You need to manage tasks more flexibly(e.g. reordering, cancellation, or grouping).
//    ??You want to decouple task creation from execution.
//    ??There is a need to handle failure cases(e.g. if the thread pool cannot be initialized you can process tasks synchronously).
//    ??Use Direct Thread Task if 
//    ??The task flow is straightforward and does not require intermediate management.
//    ??You want to minimize overhead and complexity.