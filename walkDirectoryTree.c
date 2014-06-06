
// Apated from tree traversal code- rosettacode.org

#include "dedup.h"

enum {
	WALK_OK = 0,
	WALK_BADPATTERN,
	WALK_NAMETOOLONG,
	WALK_BADIO,
};

#define WS_NONE		0
#define WS_RECURSIVE	(1 << 0)
#define WS_DEFAULT	WS_RECURSIVE
#define WS_FOLLOWLINK	(1 << 1)	/* follow symlinks */
#define WS_DOTFILES	(1 << 2)	/* per unix convention, .file is hidden */
#define WS_MATCHDIRS	(1 << 3)	/* if pattern is used on dir names too */

int walk_recur(char *dname, regex_t *reg, int spec, void (*hashFile)(char *file))
{
	struct dirent *dent;
	DIR *dir;
	struct stat st;
	char fn[FILENAME_MAX];
	int res = WALK_OK;
	int len = (int)strlen(dname);
	if (len >= FILENAME_MAX - 1)
		return WALK_NAMETOOLONG;

	Str_Strcpy(fn, dname,MAX_FILE_NAME_LEN);
	fn[len++] = '/';

	if (!(dir = opendir(dname))) {
		printf("can't open %s\n", dname);
		return WALK_BADIO;
	}

	errno = 0;
	while ((dent = readdir(dir))) {
		if (!(spec & WS_DOTFILES) && dent->d_name[0] == '.')
			continue;
		if (!strcmp(dent->d_name, ".") || !strcmp(dent->d_name, ".."))
			continue;

		Str_Strcpy(fn + len, dent->d_name, FILENAME_MAX - len);

		if (lstat(fn, &st) == -1) {
			printf("Can't stat %s\n", fn);
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
				walk_recur(fn, reg, spec,hashFile);

			if (!(spec & WS_MATCHDIRS)) continue;
		}

		if(S_ISREG(st.st_mode)){
			printf("Considering file %s\n",fn);
			hashFile(fn);
		}
	}
	if (dir) closedir(dir);
	return res ? res : errno ? WALK_BADIO : WALK_OK;
}

int walk_dir(char *dname, char *pattern, int spec, void (*hashFile)(char *file))
{
	regex_t r;
	int res;
	if (regcomp(&r, pattern, REG_EXTENDED | REG_NOSUB))
		return WALK_BADPATTERN;
	res = walk_recur(dname, &r, spec,hashFile);
	regfree(&r);

	return res;
}

int walk(char *src, void (*hashFile)(char *file))
{
	int r = walk_dir(src, ".\\.c$", WS_DEFAULT|WS_MATCHDIRS,hashFile);
	switch(r) {
        case WALK_OK:		break;
        case WALK_BADIO:	printf("IO error\n");
        case WALK_BADPATTERN:	printf("Bad pattern\n");
        case WALK_NAMETOOLONG:	printf("Filename too long\n");
        default:
            printf("Unknown error?\n");
	}
	return 0;
}
