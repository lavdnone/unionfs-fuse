/*
* Description: find a file in a branch
*
* License: BSD-style license
*
* original implementation by Radek Podgorny
*
* License: BSD-style license
* Copyright: Radek Podgorny <radek@podgorny.cz>,
*            Bernd Schubert <bernd-schubert@gmx.de>
*
*
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/statvfs.h>
#include <stdbool.h>

#include "unionfs.h"
#include "opts.h"
#include "debug.h"
#include "hashtable.h"
#include "general.h"
#include "string.h"

/**
 * unionfs-fuse readdir function
 */
int unionfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
	DBG("%s\n", path);

	(void)offset;
	(void)fi;
	int rc = 0;

	// we will store already added files here to handle same file names across different branches
	struct hashtable *files = create_hashtable(16, string_hash, string_equal);

	char p[PATHLEN_MAX];
	if (BUILD_PATH(p, uopt.branches[1].path, path)) {
		rc = -ENAMETOOLONG;
		goto out;
	}

	DIR *dp = opendir(p);

	struct dirent *de;
	while ((de = readdir(dp)) != NULL) {

		// fill with something dummy, we're interested in key existence only
		char *key = strdup(de->d_name);
		hashtable_insert(files, key, key);

		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		if (filler(buf, de->d_name, &st, 0)) break;
	}

	closedir(dp);

out:
	hashtable_destroy(files, 0);

	RETURN(rc);
}

/**
 * check if a directory on all paths is empty
 * return 0 if empty, 1 if not and negative value on error
 *
 * TODO: This shares lots of code with unionfs_readdir(), can we merge
 *       both functions?
 */
int dir_not_empty(const char *path) {

	DBG("%s\n", path);

	int rc = 0;
	int not_empty = 0;

	char p[PATHLEN_MAX];
	if (BUILD_PATH(p, uopt.branches[1].path, path)) {
		rc = -ENAMETOOLONG;
		goto out;
	}

	DIR *dp = opendir(p);
	struct dirent *de;
	while ((de = readdir(dp)) != NULL) {
		if ((strcmp(de->d_name, ".") == 0) ||  (strcmp(de->d_name, "..") == 0)) {
			continue;
		}

		// When we arrive here, a valid entry was found
		not_empty = 1;
		closedir(dp);
		goto out;
	}

	closedir(dp);
out:
	if (rc) RETURN(rc);

	RETURN(not_empty);
}
