#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "page_url.h"
#include "options.h"

#define MAX_URL_LEN 1024
#define BASE_REDDIT_URL "http://www.reddit.com/r/"
#define BASE_REDDIT_URL_LEN strlen(BASE_REDDIT_URL)
#define JSON_POSTFIX ".json"
#define SLASH "/"
#define QMARK "?"
#define AMPERSAND "&"
#define SORT_STR "sort="
#define TIME_STR "t="
#define AFTER_STR "after="
#define TAG_WIDTH 9

static struct
{
	char string[MAX_URL_LEN];
	int base_len;	
	int next_set;
} url;

static char last_tag[TAG_WIDTH + 1];

char * get_url()
{
	return url.string;
}

void set_base_url()
{
	strcpy(url.string, "http://www.reddit.com/r/");
	strcat(url.string, get_opt_subreddit());
	strcat(url.string, SLASH);
	char *sort = get_opt_sort_string();
	strcat(url.string, sort);
	strcat(url.string, SLASH);
	strcat(url.string, JSON_POSTFIX);
	strcat(url.string, QMARK);
	strcat(url.string, SORT_STR);
	strcat(url.string, get_opt_sort_string());
	strcat(url.string, AMPERSAND);
	strcat(url.string, TIME_STR);
	strcat(url.string, get_opt_window_string());

	url.base_len = strlen(url.string);
}

int get_base_url_len()
{
	return url.base_len;
}

int next_page()
{
	if (!strlen(last_tag))
		return 1;

	if (!url.next_set)
	{
		strcat(url.string, AMPERSAND);
		strcat(url.string, AFTER_STR);
		url.next_set = 1;
	}

	char *paste = strrchr(url.string, '=');

	if (!paste)
	{
		fprintf(stderr, "Error: could not create next page url!\n");
		return 1;
	}
	
	strcpy(++paste, last_tag);
	return 0;
}

void set_last_tag(char *tag)
{
	strcpy(last_tag, tag);
}