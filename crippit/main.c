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
	int downloaded = 0, page_id = 1, page_num = 0, sum = 0;

	if (ret = init(argc, argv))
		return ret;

	int starting_page = get_opt_page_start();
	int pages_left = get_opt_page_num();

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
			page_num++;
	} while (--pages_left && downloaded > 0);

	log_verbose("-> Finished downloading %d pages [%d pictures]\n", page_num, sum);

	cleanup();

	return 0;
}

int init(int argc, char *argv[])
{
	int ret;
	if (ret = parse_options(argc, argv))
		return ret;

	set_base_url();
	init_curl();

	return 0;
}

void cleanup()
{
	cleanup_curl();
}