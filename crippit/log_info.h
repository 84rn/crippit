#ifndef LOG_INFO_H_
#define LOG_INFO_H_

#include <stdarg.h>
#include <curl\curl.h>

void log_verbose(const char *s, ...);
void log_debug(const char *s, ...);
void log_curl_error(CURLcode code);


#endif