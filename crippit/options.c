#include <stdlib.h>
#include <string.h> 
#include <ctype.h>
#include <stdio.h>
#include <Windows.h>
#include "options.h"
#include "log_info.h"

static struct options
{
	int page_start;
	int page_num;
	char subreddit[SUBREDDIT_LEN];
	enum sort_type sort;
	enum time_window window;
	char save_path[SAVE_PATH_LEN];
	int verbose;
} s_options;

void print_usage();

int set_opt_sort(char *s)
{
	if (!strcmp(s, "hot"))
		s_options.sort = SORT_HOT;
	else if (!strcmp(s, "new"))
		s_options.sort = SORT_NEW;
	else if (!strcmp(s, "top"))
		s_options.sort = SORT_TOP;
	else
		return 1;

	return 0;
}

int set_opt_twindow(char *s)
{
	if (!strcmp(s, "day"))
		s_options.window = TIME_DAY;
	else if (!strcmp(s, "week"))
		s_options.window = TIME_WEEK;
	else if (!strcmp(s, "month"))
		s_options.window = TIME_MONTH;
	else if (!strcmp(s, "year"))
		s_options.window = TIME_YEAR;
	else if (!strcmp(s, "all"))
		s_options.window = TIME_ALL;
	else
		return 1;

	return 0;
}


int set_opt_dir(char *s)
{
	DWORD attributes = GetFileAttributes(s);

	if (attributes == INVALID_FILE_ATTRIBUTES || !(attributes & FILE_ATTRIBUTE_DIRECTORY))
	{
			int dir;
			log_verbose("Creating directory %s - ", s);
			dir = CreateDirectory(s, NULL);

			if (dir)
				log_verbose("success.\n");
			else
			{
				log_verbose("failed.\n");
				return GetLastError();
			}			
	}

	strcpy(s_options.save_path, s);
	return 0;
}

int get_opt_page_start()
{
	return s_options.page_start;
}

int get_opt_page_num()
{
	return s_options.page_num;
}

char * get_opt_subreddit()
{
	return s_options.subreddit;
}

enum sort_type get_opt_sort()
{
	return s_options.sort;
}

enum time_window get_opt_window()
{
	return s_options.window;
}

int parse_options(int argc, char *argv[])
{
	char c;

	strcpy(s_options.save_path, DEF_SAVE_PATH);

	while (--argc && (*++argv)[0] == '-')
	{
		switch(c = *++argv[0])
		{
		case 'p':			
			if (!isdigit((*++argv)[0]))
			{
				fprintf(stderr, "Invalid switch value -p: %s\n", *argv);
				return 1;
			}
			else
			{
				--argc;
				log_debug("Setting switch -%s to %s\n", *(argv - 1), *argv);
				s_options.page_start = atoi(*argv);
			}
			break;
		case 'n':
			if (!isdigit((*++argv)[0]))
			{
				fprintf(stderr, "Invalid switch value -n: %s\n", *argv);
				return 1;
			}
			else
			{
				--argc;
				log_debug("Setting switch -%s to %s\n", *(argv - 1), *argv);
				s_options.page_num = atoi(*argv);
			}
			break;
		case 's':
			if (set_opt_sort(*++argv))
			{
				fprintf(stderr, "Invalid switch value -s: %s\n", *argv);
				return 1;
			}		
			else
			{
				--argc;
				log_debug("Setting switch -%s to %s\n", *(argv - 1), *argv);
			}		
			break;
		case 't':
			if (set_opt_twindow(*++argv))
			{
				fprintf(stderr, "Invalid switch value -t: %s\n", *argv);
				return 1;
			}
			else
			{
				--argc;
				log_debug("Setting switch -%s to %s\n", *(argv - 1), *argv);
			}
			break;
		case 'd':
			if (set_opt_dir(*++argv))
			{
				fprintf(stderr, "Invalid switch value -d: %s\n", *argv);
				return 1;
			}
			else
			{
				--argc;
				log_debug("Setting switch -%s to %s\n", *(argv - 1), *argv);
			}
			break;
		case 'h':
			print_usage();
			exit(EXIT_SUCCESS);
			break;
		case 'v':
			s_options.verbose = 1;
			break;

		
		}
	}

	if (argc != 1)
	{
		print_usage();
		return 1;
	}

	strcpy(s_options.subreddit, *argv);
	log_debug("Subreddit set to: %s\n", s_options.subreddit);
	
	return 0;
}

char * get_opt_sort_string()
{
	char * ret;
	switch (get_opt_sort())
	{
	case SORT_HOT:
		ret =  "hot";
		break;
	case SORT_NEW:
		ret = "new";
		break;
	case SORT_TOP:
		ret = "top";
		break;
	default:
		ret = NULL;
		break;
	}

	return ret;
}

int get_opt_verbose()
{
	return s_options.verbose;
}

char * get_opt_window_string()
{
	char * ret;
	switch (get_opt_window())
	{
	case TIME_ALL:
		ret = "all";
		break;
	case TIME_DAY:
		ret = "day";
		break;
	case TIME_WEEK:
		ret = "week";
		break;
	case TIME_MONTH:
		ret = "month";
		break;
	case TIME_YEAR:
		ret = "year";
		break;
	default:
		ret = NULL;
		break;
	}

	return ret;
}

char * get_opt_save_path()
{
	return s_options.save_path;
}

void print_usage()
{
	printf("Usage: crippit [-v -h] [-p start] [-n num] [-d dir] [-s sort] [-t time] subreddit\n");
	printf("\t-p start\tstart from page no. <start> (defaults to 1)\n");
	printf("\t-n num\t\tdownload <num> pages and exit\n");
	printf("\t-d dir\t\tset save directory. If it doesn't exist it will \n\t\t\tbe created (defaults to program dir)\n");
	printf("\t-s sort\t\tsort method, choose between hot/top/new \n\t\t\t(defaults to hot)\n");
	printf("\t-t time\t\tsets the result time window,\n\t\t\tone of day/week/month/year/all (defaults to day)\n");
	printf("\t-v\t\tverbose - print useful information\n");
	printf("\t-h\t\thelp - print this text\n");
	printf("\tsubreddit\tthe name of the subreddit which will be ripped from\n");
}