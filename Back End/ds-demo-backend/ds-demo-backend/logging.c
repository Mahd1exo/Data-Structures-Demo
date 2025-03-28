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

// Append a message to the in-memory log buffer.
void append_to_log_buffer(const char* message) {
    if (message == NULL)
        return;
    int len = (int)strlen(message);
    if (log_buffer_offset + len < LOG_BUFFER_SIZE - 1) {
        // Append message to buffer.
        strcpy(&log_buffer[log_buffer_offset], message);
        log_buffer_offset += len;
    }
    // Otherwise, you may choose to rotate or truncate the buffer.
}

// Custom access log callback for CivetWeb. This is called for every request.
int my_log_access(const struct mg_connection* conn, const char* message) {
    append_to_log_buffer(message);
    append_to_log_buffer("\n");
    // Return 0 so CivetWeb does not output its default log message.
    return 0;
}

// Show the in-memory log buffer continuously until the user presses 'q'.
void show_logs(void) {
    while (1) {
        system("cls");  // Clear screen (Windows-specific)
        printf("\n========== BEGIN LOGS ==========\n");
        printf("%s", log_buffer);
        printf("\n=========== END LOGS ===========\n");
        printf("Press 'q' to return to main menu...\n");

        // Check if user pressed 'q' or 'Q'
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                break;
            }
        }
        Sleep(1000);  // Refresh every second
    }
}
