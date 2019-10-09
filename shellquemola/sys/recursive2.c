#include "recursive2.h"

int walk_recur(void (*func)(const char*), const char *dname, regex_t *reg, int spec)
{
	struct dirent *dent;
	DIR *dir;
	struct stat st;
	char fn[FILENAME_MAX];
	int res = WALK_OK;
	int len = strlen(dname);
	if (len >= FILENAME_MAX - 1)
		return WALK_NAMETOOLONG;
 
	strcpy(fn, dname);
	fn[len++] = '/';
 
	if (!(dir = opendir(dname))) {
		warn("can't open %s", dname);
		return WALK_BADIO;
	}
 
	errno = 0;
	while ((dent = readdir(dir))) {
		if (!(spec & WS_DOTFILES) && dent->d_name[0] == '.')
			continue;
		if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
			continue;
 
		strncpy(fn + len, dent->d_name, FILENAME_MAX - len);
		if (lstat(fn, &st) == -1) {
			warn("Can't stat %s", fn);
			res = WALK_BADIO;
			continue;
		}
 
		/* don't follow symlink unless told so */
		if (S_ISLNK(st.st_mode) && !(spec & WS_FOLLOWLINK))
			continue;
 
		/* will be false for symlinked dirs */
		if (S_ISDIR(st.st_mode)) {
			/* recursively follow dirs */
			if ((spec & WS_RECURSIVE))
				walk_recur(func, fn, reg, spec);
 
			if (!(spec & WS_MATCHDIRS)) continue;
		}
 
		/* pattern match */
		if (!regexec(reg, fn, 0, 0, 0)) {puts(fn);(*func)(fn);}
	}
 
	if (dir) closedir(dir);
	return res ? res : errno ? WALK_BADIO : WALK_OK;
}
 
int walk_dir(void (*func)(const char*), const char *dname, char *pattern, int spec)
{
	regex_t r;
	int res;
	if (regcomp(&r, pattern, REG_EXTENDED | REG_NOSUB))
		return WALK_BADPATTERN;
	res = walk_recur(func, dname, &r, spec);
	regfree(&r);
 
	return res;
}
 
void recursive(void (*func)(const char*), const char* basepath, int all)
{	
	int r;
	if (all) {
		r = walk_dir(func, basepath, ".\\.*$", WS_DEFAULT|WS_MATCHDIRS);
	} else {
		r = walk_dir(func, basepath, ".\\.so$", WS_DEFAULT|WS_MATCHDIRS);
	}
	switch(r) {
	case WALK_OK:		break;
	case WALK_BADIO:	err(1, "IO error");
	case WALK_BADPATTERN:	err(1, "Bad pattern");
	case WALK_NAMETOOLONG:	err(1, "Filename too long");
	default:
		err(1, "Unknown error?");
	}
}
