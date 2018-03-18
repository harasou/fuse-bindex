/* vim: set ft=c ts=8 sw=8 noet : */
#include "bindex.h"

#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int ret;
	char *binddir;

	if (argc < 3) {
		fprintf(
		    stderr,
		    "Usage: bindex [fuse options] <bind root> <mountpoint>\n");
		return -1;
	}

	binddir = realpath(argv[argc - 2], NULL);
	ret     = bindex_init(binddir);
	if (ret != 0) {
		fprintf(stderr, "`%s' is not a directory\n", binddir);
		return -1;
	}

	/* Truncate arguments for fuse_main */
	argv[argc - 2] = argv[argc - 1];
	argv[argc - 1] = NULL;
	argc--;

	return fuse_main(argc, argv, &bindex_oper, NULL);
}
