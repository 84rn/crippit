#include <stdlib.h>
#include <string.h>
#include "json.h"

char * get_json_string_value(char *json, char *name, char **buf)
{
	char *p;

	if (p = strstr(json, name))
	{
		p += strlen(name) + 4;
		size_t width = strcspn(p, "\"");
		*buf = malloc(width + 1);
		strncpy(*buf, p, width);
		(*buf)[width] = '\0';
		p += width + 2;		
	}
	else
		return NULL;

	return p;
}

void free_json_data(json_data *dt)
{
	if (dt->id)
	{
		free(dt->id);
		dt->id = NULL;
	}
	if (dt->permalink)
	{
		free(dt->permalink);
		dt->permalink = NULL;
	}
	if (dt->subreddit)
	{
		free(dt->subreddit);
		dt->subreddit = NULL;
	}
	if (dt->tag)
	{
		free(dt->tag);
		dt->tag = NULL;
	}
	if (dt->title)
	{
		free(dt->title);
		dt->title = NULL;
	}
	if (dt->url)
	{
		free(dt->url);
		dt->url = NULL;
	}
}

