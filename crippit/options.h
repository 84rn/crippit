#ifndef OPTIONS_H_
#define OPTIONS_H_

/*  Arguments for crippit include:

	-p : int - start page - tells the program from which page it should start downloading
	-n : int - number of pages - how many pages should be downloaded - if the argument is 
			   omitted, the program continues until there is nothing to download (indexing starts from 0)
	-s : char[] - sort type - choose between hot (default), top, new
	-t : char[] - time window - choose between day (default)/week/month/year/all
	-d : char[] - save directory - defaults to the directory of the program
	-v : <none> - verbose, print useful info
	The last argument (mandatory) should be the subreddit's name									
*/

#define SUBREDDIT_LEN 100
#define SAVE_PATH_LEN 500
#define DEF_SAVE_PATH "."

enum sort_type
{
	SORT_HOT,
	SORT_TOP,
	SORT_NEW
};

enum time_window
{
	TIME_DAY,
	TIME_WEEK,
	TIME_MONTH,
	TIME_YEAR,
	TIME_ALL,
};

int get_opt_page_start();
int get_opt_page_num();
char * get_opt_subreddit();
enum sort_type get_opt_sort();
char * get_opt_sort_string();
enum time_window get_opt_window();
char * get_opt_window_string();
char * get_opt_save_path();
int get_opt_verbose();
int get_opt_follow_redirects();

int parse_options(int argc, char *argv[]);

#endif