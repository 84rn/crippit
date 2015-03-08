#include <stdio.h>
#include "log_info.h"
#include "options.h"

void log_verbose(char *s, ...)
{
	if (get_opt_verbose())
	{
		va_list vl;
		va_start(vl, s);
		vprintf(s, vl);
		va_end(vl);
	}
}

void log_debug(char *s, ...)
{
#ifdef CRIPPIT_DEBUG
	va_list vl;
	va_start(vl, s);
	vprintf(s, vl);
	va_end(vl);
#endif
}