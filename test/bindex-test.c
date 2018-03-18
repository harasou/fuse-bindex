#include "bindex.h"

#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>

Test(bindex, bindex)
{
	char *binddir = "/vagrant/test/olddir";
	cr_assert(bindex_init(binddir));
}
