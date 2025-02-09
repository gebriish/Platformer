#pragma once
#include <cstdio>
#include <cstdarg>

enum LogLevel {
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL
};

void log_message(LogLevel level, const char *format, ...);
void log_info(const char* format, ...);
void log_warn(const char* format, ...);
void log_error(const char* format, ...);
void log_fatal(const char* format, ...);

void log_init(const char* file_name);
void log_close();
