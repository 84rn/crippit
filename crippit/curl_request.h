#ifndef CURL_REQUEST_H_
#define CURL_REQUEST_H_

#include <curl\curl.h>

typedef struct s_response_buffer * p_response_buffer;

int init_curl();
void cleanup_curl();
CURLcode start_request();
int download_files();
int fast_forward_page();
long long int get_file_bytes();

#endif