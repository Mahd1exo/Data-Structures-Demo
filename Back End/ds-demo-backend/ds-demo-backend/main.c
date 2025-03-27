#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <civetweb.h>
#include "route_handlers.h"

int main(int argc, char* argv[]) {
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
