#ifndef JSON_H_
#define JSON_H_

typedef struct json_data_tag
{
	char *tag;
	char *subreddit;
	char *id;
	char *permalink;
	char *url;
	char *title;
} json_data;

char * get_json_string_value(char *json, char *name, char **buf);
void free_json_data(json_data *dt);

#endif