/*
*  C Implementation: rmdir
*
* Description: unionfs rmdir() call
*              If the directory to remove exists in a lower branch, create a
*              file with a tag informing other functions that the file
*              is hidden.
*
* original implementation by Radek Podgorny
*
* License: BSD-style license
* Copyright: Radek Podgorny <radek@podgorny.cz>,
*            Bernd Schubert <bernd-schubert@gmx.de>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
#include <sys/types.h>
#include <dirent.h>

#include "unionfs.h"
#include "opts.h"
#include "debug.h"
#include "cow.h"
#include "general.h"
#include "findbranch.h"
#include "string.h"
#include "readdir.h"
#include "usyslog.h"

/**
  * rmdir() call
  */
int unionfs_rmdir(const char *path) {
	DBG("%s\n", path);

	if (dir_not_empty(path)) return -ENOTEMPTY;

	//always delete on RW
	char p[PATHLEN_MAX];
	if (BUILD_PATH(p, uopt.branches[0].path, path)) return ENAMETOOLONG;
	
	int res = rmdir(p);
	if (res == -1) return (errno);

	return(0);
}
