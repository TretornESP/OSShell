#ifndef RECURSIVE_2_INCLUDED
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <err.h>

#define RECURSIVE_2_INCLUDED
#endif

#define WS_NONE		0
#define WS_RECURSIVE	(1 << 0)
#define WS_DEFAULT	WS_RECURSIVE
#define WS_FOLLOWLINK	(1 << 1)	/* follow symlinks */
#define WS_DOTFILES	(1 << 2)	/* per unix convention, .file is hidden */
#define WS_MATCHDIRS	(1 << 3)	/* if pattern is used on dir names too */

enum {
	WALK_OK = 0,
	WALK_BADPATTERN,
	WALK_NAMETOOLONG,
	WALK_BADIO,
};
 
int walk_recur(void (*func)(const char*), const char *dname, regex_t *reg, int spec);
int walk_dir(void (*func)(const char*), const char *dname, char *pattern, int spec);
void recursive(void (*func)(const char*), const char* basepath, int all);
