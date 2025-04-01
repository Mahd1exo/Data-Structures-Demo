#ifndef LOGGING_H
#define LOGGING_H

// Define the maximum size for our in-memory log buffer.
#define LOG_BUFFER_SIZE 100000

// Initialize the log buffer (to be defined in logging.c).
extern char log_buffer[LOG_BUFFER_SIZE];
extern int log_buffer_offset;

// Append a message to the in-memory log buffer.
void append_to_log_buffer(const char* message);

// Custom access log callback for CivetWeb.
int my_log_access(const struct mg_connection* conn, const char* message);

// Show the in-memory log buffer (with a continuous refresh until 'q' is pressed).
void show_logs(void);

#endif  // LOGGING_H
