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
	mpsa->ctlrs = vector_alloc();

        return mpsa;
}

extern struct mpa_lun *
alloc_mpa_lun ()
{
        struct mpa_lun * mpal;

        mpal = malloc(sizeof(struct mpa_lun));
        //mpal->paths = vector_alloc();

        return mpal;
}

extern int
add_array (struct mparray * mpa)
{
	int i, j;
	struct mparray * mpa_tmp;

	vector_foreach_slot (arrays, mpa_tmp, i) {
		j = strcmp(mpa_tmp->id, mpa->id);
		/* Existing array id has higher value. Insert new one in this place */
		if (j>0)
			break;
		else if (j==0)
			return 1;
	}
	
	if (!vector_insert_slot(arrays, i, mpa)) {
		return 0;
	}
	return 1;
}

extern int
add_lun_to_array (struct mparray * mpa, struct mpa_lun * mpal)
{
        int i = 0;
        struct mpa_lun * mpal_tmp;

        vector_foreach_slot (mpa->luns, mpal_tmp, i) {
		/* We've found an existing LUN higher numbered than the one we're inserting. Put it here */
	        if (mpal_tmp->lun > mpal->lun) {
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
        //mpal->mpa = mpa;

        return 0;
}

extern int 
add_ctlr_to_lun (struct mpa_lun * mpal, struct mpa_ctlr * mpac)
{

}

extern int 
add_ctlr_to_array (struct mparray * mpa, struct mpa_ctlr * mpac)
{
	int i = 0;
	struct mpa_ctlr * mpac_tmp;

	vector_foreach_slot (mpa->ctlrs, mpac_tmp, i) {
		
	}

}

extern int 
add_path_to_ctlr (struct mpa_ctlr * mpac, struct mpa_path * mpap)
{
	// TODO: We'll need to sort this
       	vector_insert_slot( mpac->paths, 0, mpap );
	return 1;
}

