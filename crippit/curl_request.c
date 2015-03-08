#include <stdio.h> 
#include <stdlib.h>
#include "curl_request.h"
#include "json.h"
#include "options.h"
#include "page_url.h"
#include "log_info.h"

#define DEFAULT_EXT ".jpg"

static struct s_response_buffer
{
	char *s;
	long len;
} response_buffer;

static p_response_buffer presp_buf = &response_buffer;

static CURL *req_handle;


void free_response_struct(p_response_buffer rt)
{
	if (rt->s)
	{
		free(rt->s);
		rt->s = NULL;
	}
	rt->len = 0;
}

void init_response_struct(p_response_buffer rt)
{
	free_response_struct(rt);

	if ((rt->s = malloc(rt->len + 1)) == NULL)
	{
		fprintf(stderr, "Error: malloc failed in init_response_struct()\n");
		exit(EXIT_FAILURE);
	}

	rt->s[rt->len] = '\0';
}


size_t response_to_string(char *ptr, size_t size, size_t nmemb, p_response_buffer rb)
{
	int new_len = rb->len + size * nmemb;

	rb->s = realloc(rb->s, new_len + 1);
	if (!rb->s)
	{
		fprintf(stderr, "Error: realloc() in response_to_string() failed\n");
		exit(EXIT_FAILURE);
	}

	memcpy(rb->s + rb->len, ptr, size * nmemb);
	rb->s[new_len] = '\0';
	rb->len = new_len;

	return size * nmemb;
}

void init_curl()
{
	CURLcode ret;

	if (ret = curl_global_init(CURL_GLOBAL_ALL))
	{
		fprintf(stderr, "Error: cURL global init failed\n");
		exit(EXIT_FAILURE);
	}

	req_handle = curl_easy_init();
	if (!req_handle)
	{
		fprintf(stderr, "Error: cURL handle init failed\n");
		exit(EXIT_FAILURE);
	}

	curl_easy_setopt(req_handle, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(req_handle, CURLOPT_WRITEFUNCTION, response_to_string);
	curl_easy_setopt(req_handle, CURLOPT_WRITEDATA, presp_buf);

}

void update_req_url()
{
	curl_easy_setopt(req_handle, CURLOPT_URL, get_url());
}

void cleanup_curl()
{
	curl_easy_cleanup(req_handle);
	curl_global_cleanup();
	free_response_struct(presp_buf);
}

CURLcode start_request()
{
	CURLcode ret = 0;

	init_response_struct(presp_buf);

	update_req_url();

	if (req_handle)
		ret = curl_easy_perform(req_handle);
	else
	{
		fprintf(stderr, "Error: could not start the request, handle = NULL\n");
		exit(EXIT_FAILURE);
	}

	return ret;
}

size_t save_to_file(char *ptr, size_t size, size_t nmemb, FILE *file)
{
	return fwrite(ptr, size, nmemb, file);
}


int download_stuff(json_data *json)
{
	char *name = strrchr(json->url, '/');
	name++;

	int album = (strstr(json->url, "/a/") == NULL ? 0 : 1);
	int https = (strstr(json->url, "https://") == NULL ? 0 : 1);
	int gifv = (strstr(json->url, ".gifv") == NULL ? 0 : 1);
	char *ext = strchr(name, '.');

	if (!album && !https && !gifv && ext)
	{
		while (isalpha(*++ext));
		*ext = '\0';
		CURL *f_handle = curl_easy_init();
		char path[1000] = { 0 };

		strcat(path, get_opt_save_path());
		strcat(path, "\\");
		strcat(path, name);
		FILE *file = fopen(path, "ab");

		if (!file)
		{
			fprintf(stderr, "FILE NULL\n");
		}

		curl_easy_setopt(f_handle, CURLOPT_URL, json->url);
		curl_easy_setopt(f_handle, CURLOPT_WRITEFUNCTION, save_to_file);
		curl_easy_setopt(f_handle, CURLOPT_WRITEDATA, file);

		log_verbose("Downloading: %s\n", json->tag);
		log_verbose("\tURL: %s\n", json->url);
		log_verbose("\tTitle: %.60s\n\n", json->title);

		curl_easy_perform(f_handle);
		curl_easy_cleanup(f_handle);

		fclose(file);
	}
	else
	{
		log_verbose("Skipping:  %s\n\n", json->url);
		return 1;
	}

	return 0;
}

int parse_response(p_response_buffer rb, int(*parse_fun)(json_data *))
{
	char *div;
	int tag_num = 0, downloaded = 0;
	json_data json;
	memset(&json, 0, sizeof(json_data));

	for (div = get_json_string_value(rb->s, "subreddit", &json.subreddit); div != NULL; div = get_json_string_value(div, "subreddit", &json.subreddit), ++tag_num)
	{
		div = get_json_string_value(div, "id", &json.id);
		div = get_json_string_value(div, "permalink", &json.permalink);
		div = get_json_string_value(div, "name", &json.tag);
		div = get_json_string_value(div, "url", &json.url);
		div = get_json_string_value(div, "title", &json.title);

		if (!parse_fun(&json))
			downloaded++;

		if (strlen(json.tag))
			set_last_tag(json.tag);
		else
			fprintf(stderr, "Error: could not copy last tag\n");

		free_json_data(&json);
	}

	return downloaded;
}

int download_files()
{
	start_request();
	return parse_response(presp_buf, download_stuff);
}

int find_last_tag(p_response_buffer rb)
{
	json_data json;
	char *div;
	int ret = 0;

	memset(&json, 0, sizeof(json_data));

	for (div = get_json_string_value(rb->s, "subreddit", &json.subreddit); div != NULL; div = get_json_string_value(div, "subreddit", &json.subreddit))
		get_json_string_value(div, "name", &json.tag);

	if (json.tag && strlen(json.tag))
	{
		set_last_tag(json.tag);
	}
	else
	{		
		ret = 1;
	}

	free_json_data(&json);

	return ret;
}

int fast_forward_page()
{
	start_request();

	if (find_last_tag(presp_buf) == 0)
	{
		log_verbose("Skipping page...\n");
		next_page();
		return 0;
	}
	else
	{
		return 1;
	}
}