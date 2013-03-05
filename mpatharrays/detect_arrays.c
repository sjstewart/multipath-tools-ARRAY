/**
 * 
 *
 */
#include "libmpatharrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <string.h>

vector mpath_storage_arrays;

/*
struct mparray {
	int testint;
	char * storage_array_name;
	vector luns;
};

struct mparray_lun {
	char * wwid;
	char * name;
	char * dmname;
	int lun;
	vector paths;
	
	// Pointer back to the array
	struct mparray * mpa;
};

struct mparray_path {
	char * devname;
	int host;
	int controller;
	int target;
	int lun;

	// Pointer back to the LUN
	struct mparray_lun * mpal;
};

struct mparray * alloc_mparray (void);
struct mparray_lun * alloc_mparray_lun (void);
int add_lun_to_array (struct mparray * mpa, struct mparray_lun * mpal);
int add_path_to_lun (struct mparray_lun * mpal, struct mparray_path * mpap);
*/
void detect_arrays (vector pathvec);

/*

struct mparray *
alloc_mparray ()
{
	struct mparray * mpsa;

	mpsa = malloc(sizeof(struct mparray));
	mpsa->luns = vector_alloc();

	return mpsa;
}

struct mparray_lun *
alloc_mparray_lun ()
{
	struct mparray_lun * mpal;

	mpal = malloc(sizeof(struct mparray_lun));
	mpal->paths = vector_alloc();

	return mpal;
}

int
add_lun_to_array (struct mparray * mpa, struct mparray_lun * mpal)
{
	int i = 0;
	struct mparray_lun * mpal_tmp;

	// Traverse the list until we've gone one entry too far
	// Then decrement the pointer and insert the new entry at that place
	vector_foreach_slot (mpa->luns, mpal_tmp, i) {
		if (mpal->lun > mpal_tmp->lun) {
			i--;
			break;
		// There's already a LUN on this array for this host
		} else if (mpal->lun == mpal_tmp->lun) {
			// If the WWIDs match, we're okay, otherwise, we've hit a bad condition
			if (!strcmp(mpal->wwid, mpal_tmp->wwid))
				return 0;
			else
				return -1;
		}
	}

	if (!vector_insert_slot (mpa->luns, i, mpal))
		return -1;
	
	// Point back to the parent
	mpal->mpa = mpa;

	return 0;
}

// TODO:  Some checking and stuff to make sure this will work properly... Check for duplicate entries?
int
add_path_to_lun (struct mparray_lun * mpal, struct mparray_path * mpap)
{

	if (vector_insert_slot( mpal->paths, 0, mpap ))
	{
		// Point back to the parent
		mpap->mpal = mpal;
		return 0;
	}

	return 1;
}
*/
void
detect_arrays (vector pathvec)
{
	int i;
	struct mparray * mps;

	mpath_storage_arrays = vector_alloc();
	for (i = 0; i < 5; i++) {
		mps = alloc_mparray();
		mps->testint = i;
		mps->storage_array_name = "This is my array or something";

		// Let's add something to this vector!
		if (!vector_alloc_slot(mpath_storage_arrays))
			goto it_didnt_work;

		vector_set_slot(mpath_storage_arrays, mps);
	}

	vector_foreach_slot(mpath_storage_arrays, mps, i){
		printf("This is a test %i %s\n", mps->testint, mps->storage_array_name);
	}

	return;

it_didnt_work:
	printf("It didn't work\n");
}
