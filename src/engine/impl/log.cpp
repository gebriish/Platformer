#include "../log.hpp"

static const char *log_level_names[] = {
  "INFO", 
  "WARN", 
  "ERROR", 
  "FATAL"
};

static const char *log_colors[] = {
    "\x1b[32m", // INFO  - Green
    "\x1b[33m", // WARN  - Yellow
    "\x1b[31m", // ERROR - Red
    "\x1b[35m"  // FATAL - Magenta
};

#define LOG_USE_COLORS    1 
#define LOG_TO_FILE       0

static FILE *log_file = nullptr;

void log_message(LogLevel level, const char *format, ...) {
  char buffer[512];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);

  if (LOG_USE_COLORS)
    printf("%s[%s] %s\x1b[0m\n", log_colors[level], log_level_names[level], buffer);
  else
    printf("[%s] %s\n", log_level_names[level], buffer);

  if (LOG_TO_FILE && log_file) {
    fprintf(log_file, "[%s] %s\n", log_level_names[level], buffer);
    fflush(log_file);
  }
}

void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_INFO, format, args);
    va_end(args);
}

void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_WARN, format, args);
    va_end(args);
}

void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_ERROR, format, args);
    va_end(args);
}

void log_fatal(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_message(LOG_FATAL, format, args);
    va_end(args);
}

void log_init(const char *file_name) {
    if (LOG_TO_FILE) {
        log_file = fopen(file_name, "a");
        if (!log_file) {
            printf("Failed to open log file: %s\n", file_name);
        }
    }
}

void log_close() {
    if (LOG_TO_FILE && log_file) {
        fclose(log_file);
        log_file = nullptr;
    }
}
