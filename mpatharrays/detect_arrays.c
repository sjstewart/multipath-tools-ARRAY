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

struct mpa_lun {
	char * wwid;
	char * name;
	char * dmname;
	int lun;
	vector paths;
	
	// Pointer back to the array
	struct mparray * mpa;
};

struct mpa_path {
	char * devname;
	int host;
	int controller;
	int target;
	int lun;

	// Pointer back to the LUN
	struct mpa_lun * mpal;
};

struct mparray * alloc_mparray (void);
struct mpa_lun * alloc_mpa_lun (void);
int add_lun_to_array (struct mparray * mpa, struct mpa_lun * mpal);
int add_path_to_lun (struct mpa_lun * mpal, struct mpa_path * mpap);
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

struct mpa_lun *
alloc_mpa_lun ()
{
	struct mpa_lun * mpal;

	mpal = malloc(sizeof(struct mpa_lun));
	mpal->paths = vector_alloc();

	return mpal;
}

int
add_lun_to_array (struct mparray * mpa, struct mpa_lun * mpal)
{
	int i = 0;
	struct mpa_lun * mpal_tmp;

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
add_path_to_lun (struct mpa_lun * mpal, struct mpa_path * mpap)
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
/*
void
detect_arrays (vector pathvec)
{
	int i, j;
	struct mparray * mps;

	arrays = vector_alloc();
	for (i = 0; i < 5; i++) {
		mps = alloc_mparray();
		mps->testint = i;
		printf("before switch\n");
		switch(i){
		case 4:
		mps->id = "AAAAAAAA";
		break;
		case 2:
		mps->id = "BBBBBBBB";
		break;
		case 1:
		mps->id = "CCCCCCCC";
		break;
		case 3:
		mps->id = "DDDDDDDD";
		break;
		case 0:
		mps->id = "CCCCCCCC";
		break;
		
		}

		// Let's add something to this vector!
		add_array( mps );
		vector_foreach_slot(arrays, mps, j){
			printf("This is a test slot %i %i %s\n", j, mps->testint, mps->id);
		}
	}

	return;
}
*/
