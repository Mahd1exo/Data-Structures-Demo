#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>
#include <civetweb.h>
#include "route_handlers.h"

// Forward declaration of cleanup function
void clear_all_data_structures(void);

void cleanup_and_check_leaks(struct mg_context* ctx, _CrtMemState* initialState) {
    // Stop the server and cleanup the endpoint resources
    mg_stop(ctx);
    printf("Server stopped.\n");

    // Clear all our data structures
    clear_all_data_structures();
    printf("Data structures cleared.\n");

    // Take a final memory snapshot after cleanup
    _CrtMemState finalState, diffState;
    _CrtMemCheckpoint(&finalState);

    // Compare the initial snapshot with the final snapshot.
    if (_CrtMemDifference(&diffState, initialState, &finalState)) {
        printf("Memory leaks detected:\n");
        _CrtDumpMemoryLeaks();
    }
    else {
        printf("No memory leaks detected.\n");
    }
}

int main(int argc, char* argv[]) {
    // Take a memory snapshot before starting the server
    _CrtMemState s1;
    _CrtMemCheckpoint(&s1);

    const char* options[] = {
        "listening_ports", "8080",
        "num_threads", "2",
        NULL
    };

    struct mg_callbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));

    struct mg_context* ctx = mg_start(&callbacks, NULL, options);
    if (!ctx) {
        printf("Error: Could not start CivetWeb on port 8080.\n");
        return 1;
    }

    /* Register all endpoints */
    register_endpoints(ctx);

    printf("Server started on port 8080.\n");

    // Infinite loop until user chooses to exit
    while (1) {
        printf("\nMenu:\n");
        printf("1. Exit server and check for memory leaks\n");
        printf("2. Clean Memory\n");

        printf("Enter option: ");

        int option;
        if (scanf("%d", &option) != 1) {
            // Clear invalid input
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }

        if (option == 1) {
            break;
		}
		else if (option == 2) {
            clear_all_data_structures();
		}
        else {
            printf("Invalid option. Please try again.\n");
        }
    }

    // Cleanup server and check for leaks
    cleanup_and_check_leaks(ctx, &s1);

    return 0;
}
