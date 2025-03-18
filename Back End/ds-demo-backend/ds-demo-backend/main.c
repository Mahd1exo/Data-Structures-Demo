#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <civetweb.h>
#include "route_handlers.h"

int main(int argc, char* argv[]) {
    /* CivetWeb server configuration */
    const char* options[] = {
        "listening_ports", "8080",  /* Port to listen on */
        "num_threads", "2",         /* Example: 2 worker threads */
        NULL
    };

    /* Start CivetWeb without callbacks */
    struct mg_context* ctx = mg_start(NULL, NULL, options);
    if (!ctx) {
        printf("Error: Could not start CivetWeb on port 8080.\n");
        return 1;
    }

    /* Debug print for the context pointer */
    printf("CivetWeb context pointer: %p\n", (void*)ctx);

    /* Register endpoints for Data Structure operations */
    register_endpoints(ctx);

    printf("CivetWeb server started on port 8080.\n");
    printf("Endpoints:\n");

    /* LinkedList endpoints */
    printf("  LinkedList:\n");
    printf("    GET    /api/linked-list\n");
    printf("    POST   /api/linked-list/add-front  (body: {\"value\":\"X\"})\n");
    printf("    POST   /api/linked-list/add-end    (body: {\"value\":\"X\"})\n");
    printf("    DELETE /api/linked-list/remove-front\n");
    printf("    DELETE /api/linked-list/remove-end\n\n");

    /* Stack endpoints */
    printf("  Stack:\n");
    printf("    GET    /api/stack\n");
    printf("    POST   /api/stack/push  (body: {\"value\":\"X\"})\n");
    printf("    DELETE /api/stack/pop\n\n");

    /* Queue endpoints */
    printf("  Queue:\n");
    printf("    GET    /api/queue\n");
    printf("    POST   /api/queue/enqueue (body: {\"value\":\"X\"})\n");
    printf("    DELETE /api/queue/dequeue\n\n");

    /* Tree endpoints */
    printf("  Tree:\n");
    printf("    GET    /api/tree\n");
    printf("    POST   /api/tree/insert (body: {\"value\":\"X\"})\n");
    printf("    POST   /api/tree/remove (body: {\"value\":\"X\"})\n\n");

    /* Heap endpoints */
    printf("  Heap:\n");
    printf("    GET    /api/heap\n");
    printf("    POST   /api/heap/insert (body: {\"value\":\"X\"})\n");
    printf("    POST   /api/heap/extract-top\n\n");

    /* HashTable endpoints */
    printf("  HashTable:\n");
    printf("    GET    /api/hash\n");
    printf("    POST   /api/hash/insert (body: {\"key\":\"myKey\",\"val\":\"myValue\"})\n");
    printf("    POST   /api/hash/remove (body: {\"key\":\"myKey\"})\n\n");

    printf("Use Ctrl+C to stop.\n");

    /* Main loop: keep running until interrupted */
    while (1) {
#ifdef _WIN32
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    mg_stop(ctx);
    return 0;
}
