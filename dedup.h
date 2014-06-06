//
//  dedup.h
//  Dedup_Project
//
//  Created by Aditya Kotwal on 6/5/14.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>
#include <err.h>

#include <str.h>
#include "md5.h"
#define SRC "/testInDir"
#define DST "/testOutDir/dump"

#ifndef __dedup_h__
	#define __dedup_h__

	#define verbose 0
	#define fileNameRequired 1
	#define MAX_COMBINED_HASH_LEN 1024*1024
	#define MAX_FILE_NAME_LEN 1024*1024
	#define MAX_BUF_LEN 4*1024
	#define RDLEN MAX_BUF_LEN

	int walk_recur(char *dname, regex_t *reg, int spec, void (*hashFile)(char *file));
	int walk_dir(char *dname, char *pattern, int spec, void (*hashFile)(char *file));
	int walk(char* source,void (*hashFile)(char *file));
	void getMD5(const char *string, char *md5buf, long len);

#endif
