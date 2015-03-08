#include <stdlib.h>
#include "options.h"
#include "page_url.h"
#include "curl_request.h"
#include "log_info.h"

int init(int argc, char *argv[]);
void cleanup();

int main(int argc, char *argv[])
{	
	int ret;
	int downloaded = 0, page_id = 1, pages_done = 0, sum = 0, starting_page, pages_left;
	long long int bytes = 0;

	if (ret = init(argc, argv))
		return ret;

	starting_page = get_opt_page_start();

	/* If pages num wasn't set, it runs forever (pages_left always negative) */
	pages_left = get_opt_page_num();

	while (--starting_page > 0)
	{
		if (fast_forward_page())
			break;
		page_id++;
	}
		
	do
	{
		log_verbose("\n-> Downloading page %d\n\n", page_id++);
		downloaded = download_files();
		next_page();
		sum += downloaded;
		
		if(downloaded)
			pages_done++;
	} while (--pages_left && downloaded > 0);

	bytes = get_file_bytes();
	log_verbose("-> Finished downloading %d pages [%d pictures - %llu.%llu MBytes]\n", pages_done, sum,
		bytes / 1000000L, (bytes % 1000000L) / 10000L);

	cleanup();

	return 0;
}

int init(int argc, char *argv[])
{
	int ret;
	if (ret = parse_options(argc, argv))
		return ret;
	
	set_base_url();
	if (init_curl())
	{
		cleanup();
		exit(EXIT_FAILURE);
	}

	return 0;
}

void cleanup()
{
	cleanup_curl();
}