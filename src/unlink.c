/*
*  C Implementation: unlink
*
* Description: unionfs unlink() call
*              If the file to unlink exists in a lower branch, create a
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

#include "unionfs.h"
#include "opts.h"
#include "debug.h"
#include "cow.h"
#include "general.h"
#include "findbranch.h"
#include "string.h"


/**
  * unlink() call
  */
int unionfs_unlink(const char *path) {
	DBG("%s\n", path);

	// always delete file on RW branch
	char p[PATHLEN_MAX];
	if (BUILD_PATH(p, uopt.branches[0].path, path)) RETURN(ENAMETOOLONG);

	int res = unlink(p);
	if (res == -1) RETURN(errno);

	RETURN(0);
}
