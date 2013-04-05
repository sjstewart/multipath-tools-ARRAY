#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <libdevmapper.h>
#include "libmpatharrays.h"
#include "inquirer.h"


/**
 * The "main" function that will run through the pathvec and inquire about the storage array
 */
extern int
inquire_arrays (struct vectors vecs)
{
	struct path *pp;
	struct mpa_pathinq mpap;
	struct inquirer *inquirer;
	int i, ret;

	INIT_MPARRAY_PATHINQ( mpap );
	
	vector_foreach_slot (vecs.pathvec, pp, i) {
		/* Go through inquirers, trying to find one that matches */
		list_for_each_entry( inquirer, &inquirers, node ) {
			ret = inquirer->inquirer_fn(&mpap, pp);
			if (ret) {
				printf("Found proper inquirer: %s  RET: %i\n", inquirer->name, ret);
				add_path(pp, &mpap, vecs);
				goto next_path;
			} else {
				printf("Inquirer %s didn't work\n", inquirer->name);
			}
		}
next_path:;
	}

	CLEAR_MPARRAY_PATHINQ( mpap );

	return 1;
}

/*
 * Allocate an inquirer structure
 */
struct inquirer *
alloc_inquirer (void)
{
	struct inquirer *i;
	
	i = MALLOC(sizeof(struct inquirer));
	if (i) {
		INIT_LIST_HEAD(&i->node);
	}
	return i;
}

int
add_inquirer (const char * inq, void * inq_function)
{
	struct inquirer *i;

	i = alloc_inquirer();
	i->name = MALLOC( INQUIRER_NAME_LEN );
	strcpy (i->name, inq);

	i->inquirer_fn = inq_function;
	printf("Adding %s inquirer\n", i->name);
	list_add_tail(&i->node, &inquirers);

	return 1;
}

/*
 * Inquirer that never matches. Just used for testing!
 */
extern int
void_inquirer (struct mpa_pathinq *mpap, struct path *pp)
{
	return 0;
}
/**
 * Populate the inquirers within the list
 */
extern void
init_inquirers (void)
{
	add_inquirer ( "VOID", &void_inquirer );
	add_inquirer ( "RDAC", &rdac_inquirer );
}
