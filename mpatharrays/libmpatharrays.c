/**
 * library for detecting storage array information for multipath devices
 */

#include <vector.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libdevmapper.h>
#include "libmpatharrays.h"



extern struct mparray *
alloc_mparray (struct mpa_pathinq * mpinq)
{
        struct mparray * mpa;
        mpa = malloc(sizeof(struct mparray));

	/* Copy the array id and label info */
	mpa->id = malloc( (strlen(mpinq->array_id)*sizeof(char))+1 );
	strcpy( mpa->id, mpinq->array_id );

	mpa->label = malloc( (strlen(mpinq->array_label)*sizeof(char))+1 );
	strcpy( mpa->label, mpinq->array_label );

	/* Create LUN and Controller vectors */
        mpa->luns = vector_alloc();
	mpa->ctlrs = vector_alloc();

        return mpa;
}

extern void
free_mparray(struct mparray * mpa)
{
	struct mpa_lun * mpal;
	struct mpa_ctlr * mpac;
	int i;

	/* Free child LUNs */
	vector_foreach_slot( mpa->luns, mpal, i) {
		free_mpa_lun( mpal );
	}
	vector_free( mpa->luns );

	/* Free child Controllers */
	vector_foreach_slot( mpa->ctlrs, mpac, i) {
		free_mpa_ctlr( mpac );
	}
	vector_free( mpa->ctlrs );

	/* Free char pointers, and then the object itself */
	free( mpa->id );
	free( mpa->label );
	free( mpa );
}

extern void
free_mpa_lun(struct mpa_lun * mpal)
{
	struct mpa_ctlr * mpac;
	int i;
	
	/* Free child Controllers of the LUN first */
	vector_foreach_slot(mpal->ctlrs, mpac, i) {
		free_mpa_ctlr(mpac);
	}
	vector_free( mpal->ctlrs );

	/* Now free character pointers, and the object itself */
	free( mpal->wwid );
	free( mpal->name );
	free( mpal );
}

extern void
free_mpa_ctlr(struct mpa_ctlr * mpac)
{
	struct mpa_path * mpap;
	int i;

	free( mpac->id );
	vector_foreach_slot(mpac->paths, mpap, i) {
		free_mpa_path( mpap );
	}
	vector_free( mpac->paths );
	free( mpac );
}

extern void
free_mpa_path(struct mpa_path * mpap)
{
	free( mpap->devname );
	free( mpap );
}

extern struct mpa_lun *
alloc_mpa_lun( struct path * pp, struct vectors vecs )
{
        struct mpa_lun * mpal;
	struct multipath * mpp;

	/* Find the associated multipath device */
	mpp = find_mp_by_wwid(vecs.mpvec, pp->wwid);
	if (!mpp)
		return NULL;

        mpal = malloc(sizeof(struct mpa_lun));

	/* Copy the wwid */
	mpal->wwid = malloc(strlen(pp->wwid)+1);
	strcpy(mpal->wwid, pp->wwid);

	/* Copy the alias */
	mpal->name = malloc( (strlen(mpp->alias) * sizeof(char)) +1);
	strcpy(mpal->name, mpp->alias);

	struct dm_info * dmi;
	dmi = mpp->dmi;
	/* Copy the dm- name */
	mpal->minor = (int)dmi->minor;
	mpal->lun = pp->sg_id.lun;

	mpal->ctlrs = vector_alloc();

        return mpal;
}

extern struct mpa_ctlr *
alloc_mpa_ctlr( struct mpa_pathinq * mpinq )
{
	struct mpa_ctlr * mpac;
	
	mpac = malloc( sizeof(struct mpa_ctlr) );
	mpac->id = malloc( (strlen(mpinq->controller_id) * sizeof(char))+1 );
	strcpy( mpac->id, mpinq->controller_id );
	mpac->owner = mpinq->owner;
	mpac->preferred = mpinq->preferred;
	mpac->paths = vector_alloc();
	
	return mpac;
}

extern struct mpa_path *
alloc_mpa_path( struct path * pp )
{
	struct mpa_path * mpap;

	mpap = malloc(sizeof(struct mpa_path));
	mpap->devname = malloc(strlen(pp->dev));
	strcpy(mpap->devname, pp->dev);

	mpap->host = pp->sg_id.host_no;
	mpap->controller = pp->sg_id.channel;
	mpap->target = pp->sg_id.scsi_id;
	mpap->lun = pp->sg_id.lun;

	return mpap;
}

struct mparray *
add_array (struct mpa_pathinq * mpinq)
{
	int i, j;
	struct mparray * mpa;

	vector_foreach_slot (arrays, mpa, i) {
		j = strcmp(mpa->id, mpinq->array_id);
		/* Existing array id has higher value. Insert new one in this place */
		if (j>0)
			break;
		else if (j==0) {
			return mpa;
		}
	}
	
	mpa = alloc_mparray( mpinq );
	if (!vector_insert_slot(arrays, i, mpa)) {
		free_mparray( mpa );
		return NULL;
	}
	return mpa;
}

struct mpa_lun *
add_lun_to_array (struct mparray * mpa, struct path * pp, struct vectors vecs)
{
        int i = 0;
        struct mpa_lun * mpal;
        vector_foreach_slot (mpa->luns, mpal, i) {
		/* We've found an existing LUN higher numbered than the one we're inserting. Put it here */
	        if (mpal->lun > pp->sg_id.lun) {
                        break;
                // There's already a LUN on this array for this host
              	} else if (mpal->lun == pp->sg_id.lun) {
	                // If the WWIDs match, we're okay, otherwise, we've hit a bad condition
	                if (strcmp(mpal->wwid, pp->wwid) != 0) {
                                return NULL;
                        } else {
                                return mpal;
			}
                }
        }

	mpal = alloc_mpa_lun( pp, vecs );
        if (!vector_insert_slot (mpa->luns, i, mpal)) {
		free_mpa_lun( mpal );
                return NULL;
	}

        return mpal;
}

struct mpa_ctlr * 
add_ctlr_to_lun (struct mpa_lun * mpal, struct mpa_pathinq * mpinq)
{
	struct mpa_ctlr * mpac;
	int i, cmp;

	vector_foreach_slot (mpal->ctlrs, mpac, i) {
		cmp = strcmp(mpac->id, mpinq->controller_id);
		if (cmp > 0)
			break;
		else if (cmp == 0) {
			return mpac;
		}	
	}

	mpac = alloc_mpa_ctlr( mpinq );
	if (!vector_insert_slot(mpal->ctlrs, i, mpac)) {
		free_mpa_ctlr( mpac );
		return NULL;
	}
	return mpac;
}

extern struct mpa_ctlr *
add_ctlr_to_array (struct mparray * mpa, struct mpa_pathinq * mpinq)
{
	int i, cmp;
	struct mpa_ctlr * mpac;

	vector_foreach_slot (mpa->ctlrs, mpac, i) {
		cmp = strcmp(mpac->id, mpinq->controller_id);
		if (cmp > 0)
			break;
		else if (cmp == 0) {
			return mpac;
		}
	}
	
	mpac = alloc_mpa_ctlr(mpinq);
	if (!vector_insert_slot(mpa->ctlrs, i, mpac)) {
		free_mpa_ctlr(mpac);
		return NULL;
	}
	
	return mpac;
}

extern struct mpa_path *
add_path_to_ctlr (struct mpa_ctlr * mpac, struct path * pp)
{
	int i, cmp;
	struct mpa_path * mpap;

	vector_foreach_slot (mpac->paths, mpap, i) {
		cmp = strcmp(mpap->devname, pp->dev);
		if (cmp > 0)
			break;
		else if (cmp == 0) {
			return mpap;
		}
	}

	mpap = alloc_mpa_path(pp);
	if (!vector_insert_slot( mpac->paths, i, mpap)) {
		free_mpa_path(mpap);
		printf("Returning null\n");
		return NULL;
	}

	return mpap;
}

/*
extern int 
add_path_to_ctlr (struct mpa_ctlr * mpac, struct mpa_path * mpap)
{
	int i, cmp;
	struct mpa_path * mpap_tmp;

	vector_foreach_slot (mpac->paths, mpap_tmp, i) {
		cmp = strcmp(mpap_tmp->devname, mpap->devname);
		if (cmp > 0)
			break;
		else if (cmp == 0) {
			free(mpap);
			mpac = mpac_tmp;
			return 1;
		}
	}
	if (!vector_insert_slot( mpac->paths, i, mpap)) 
		return 0;

	return 1;
}
*/

extern void
dump_mparray (struct mparray * mpa)
{
	int i;
	struct mpa_lun * mpal;

	printf("MPARRAY: id: %s label: %s\n", mpa->id, mpa->label);
	vector_foreach_slot(mpa->luns, mpal, i) {
		dump_mpa_lun(mpal);
	}
}

void
dump_mpa_lun (struct mpa_lun * mpal)
{
	int i;
	struct mpa_ctlr * mpac;

	printf("|-LUN %i (dm-%i) %s WWID: %s\n", mpal->lun, mpal->minor, mpal->name, mpal->wwid);
	vector_foreach_slot(mpal->ctlrs, mpac, i) {
		dump_mpa_ctlr(mpac);
	}
}

void
dump_mpa_ctlr (struct mpa_ctlr * mpac)
{
	int i;
	struct mpa_path * mpap;
	
	printf("  |--CTLR: %s", mpac->id);
	if (mpac->owner && mpac->preferred)
		printf(" (*)");
	else if (mpac->owner && !mpac->preferred)
		printf("  * ");
	else if (mpac->preferred && ! mpac->owner)
		printf(" ( )");
	printf("\n");
	vector_foreach_slot(mpac->paths, mpap, i) {
		dump_mpa_path(mpap);
	}	
}

void
dump_mpa_path (struct mpa_path * mpap)
{
	printf("    |---%s %i:%i:%i:%i\n", mpap->devname, mpap->host, mpap->controller, mpap->target, mpap->lun);
}

/*
 * This is the main function that will be run to create the data structs
 *
 * TODO: Need to check the return values of each step to make sure they're working otherwise we'll be passing null pointers
 */
extern int
add_path (struct path * pp, struct mpa_pathinq * mpinq, struct vectors vecs)
{
	struct mparray * mpa;
	struct mpa_lun * mpal;
	struct mpa_ctlr * mpac;
	struct mpa_path * mpap;

	mpa = add_array( mpinq );
	mpal = add_lun_to_array( mpa, pp, vecs );
	mpac = add_ctlr_to_lun( mpal, mpinq );
	mpap = add_path_to_ctlr( mpac, pp );
	mpac = add_ctlr_to_array( mpa, mpinq );
	mpap = add_path_to_ctlr( mpac, pp );

	return 1;
}
