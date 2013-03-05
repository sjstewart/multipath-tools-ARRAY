/**
 * library for detecting storage array information for multipath devices
 */

#include "libmpatharrays.h"
#include <vector.h>
#include <string.h>

extern struct mparray *
alloc_mparray ()
{
        struct mparray * mpsa;

        mpsa = malloc(sizeof(struct mparray));
        mpsa->luns = vector_alloc();

        return mpsa;
}

extern struct mparray_lun *
alloc_mparray_lun ()
{
        struct mparray_lun * mpal;

        mpal = malloc(sizeof(struct mparray_lun));
        mpal->paths = vector_alloc();

        return mpal;
}

extern int
add_lun_to_array (struct mparray * mpa, struct mparray_lun * mpal)
{
        int i = 0;
        struct mparray_lun * mpal_tmp;

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

extern int
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

