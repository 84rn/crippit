#include <stdio.h>
#include "log_info.h"
#include "options.h"

void log_verbose(const char *s, ...)
{
	if (get_opt_verbose())
	{
		va_list vl;
		va_start(vl, s);
		vprintf(s, vl);
		va_end(vl);
	}
}

void log_debug(const char *s, ...)
{
#ifdef CRIPPIT_DEBUG
	va_list vl;
	va_start(vl, s);
	vprintf(s, vl);
	va_end(vl);
#endif
}

void log_curl_error(CURLcode code)
{
	printf("cURL error [%d] : %s\n", code, curl_easy_strerror(code));
}