#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <civetweb.h>
#include "logging.h"

// Global in-memory log buffer and offset.
char log_buffer[LOG_BUFFER_SIZE] = "";
int log_buffer_offset = 0;


void append_to_log_buffer(const char* message) {
    if (message == NULL)
        return;
    int len = (int)strlen(message);
    if (log_buffer_offset + len < LOG_BUFFER_SIZE - 1) {
        strcpy(&log_buffer[log_buffer_offset], message);
        log_buffer_offset += len;
    }
}

// Custom access log callback for CivetWeb.
int my_log_access(const struct mg_connection* conn, const char* message) {
    append_to_log_buffer(message);
    append_to_log_buffer("\n");
    return 0;
}

// Show the in-memory log buffer continuously.
void show_logs(void) {
    while (1) {
        system("cls"); 
        printf("\n<<<========== BEGIN LOGS ==========>>>\n");
        printf("%s", log_buffer);
        printf("\n>>>---------- END LOGS ----------<<<\n");
        printf("Press 'q' to return to main menu...\n");

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }
        Sleep(1000);  
    }
}
