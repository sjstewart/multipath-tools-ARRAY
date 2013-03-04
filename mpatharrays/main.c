/**
 * mpatharrays
 *
 * Author:	Sean Stewart
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <structs.h>
#include <libdevmapper.h>
#include <vector.h>
#include <structs_vec.h>
#include <getopt.h>
#include <string.h>
#include <debug.h>
#include <defaults.h>
#include <dmparser.h>
#include <devmapper.h>
#include <config.h>
#include <discovery.h>
#include <print.h>

int logsink;
struct vectors vecs;

int load_mp_vecs(void);
int dump_path (struct path * pp);
int dump_multipath(struct multipath * mpp);
static int get_dm_mpvec (vector curmp, vector pathvec, char * refwwid);

/*
 * This function already exists, but it's been stripped down for use here
 */
static int
get_dm_mpvec (vector curmp, vector pathvec, char * refwwid)
{
        int i;
        struct multipath * mpp;
        char params[PARAMS_SIZE], status[PARAMS_SIZE];

        if (dm_get_maps(curmp))
                return 1;

        vector_foreach_slot (curmp, mpp, i) {
                /*
 *                  * discard out of scope maps
 *                                   */
                if (mpp->wwid && refwwid &&
                    strncmp(mpp->wwid, refwwid, WWID_SIZE)) {
                        condlog(3, "skip map %s: out of scope", mpp->alias);
                        free_multipath(mpp, KEEP_PATHS);
                        vector_del_slot(curmp, i);
                        i--;
                        continue;
                }

		//  THIS is where the status is determined.. We may need this
                dm_get_map(mpp->alias, &mpp->size, params);
                dm_get_status(mpp->alias, status);

                disassemble_map(pathvec, params, mpp);

                /*
 *                  * disassemble_map() can add new paths to pathvec.
 *                                   * If not in "fast list mode", we need to fetch information
 *                                                    * about them
 *                                                                     */
                disassemble_status(status, mpp);
        }
        return 0;
}

/*
static int
filter_pathvec (vector pathvec, char * refwwid)
{
        int i;
        struct path * pp;

        if (!refwwid || !strlen(refwwid))
                return 0;

        vector_foreach_slot (pathvec, pp, i) {
                if (strncmp(pp->wwid, refwwid, WWID_SIZE) != 0) {
                        condlog(3, "skip path %s : out of scope", pp->dev);
                        free_path(pp);
                        vector_del_slot(pathvec, i);
                        i--;
                }
        }
        return 0;
}
*/

int
load_mp_vecs ()
{
	int i;
	//struct multipath *mpp;
	struct path *pp;
	vector curmp = NULL;
	vector pathvec = NULL;

	curmp = vector_alloc();
	pathvec = vector_alloc();

	if (!curmp || !pathvec) {
		condlog(0, "it didn't work");
		goto out;
	}

	vecs.pathvec = pathvec;
	vecs.mpvec = curmp;

	if (path_discovery(pathvec, conf, 31))
		return 1;

//	if (dm_get_maps(curmp))
//		return 1;

	// TODO: Get this dup function out of here if possible
	if (get_dm_mpvec(curmp, pathvec, NULL))
		return 1;

	get_path_layout(pathvec, 0);
        //filter_pathvec(pathvec, NULL);

	vector_foreach_slot (pathvec, pp, i) {
		/*
		 * Print it!
 		 */
		dump_path(pp);
	}

	return 0;
out:
	return 1;
}

/**
 * Debug function to dump the properties of a multipath device
 */
int
dump_multipath (struct multipath * mpp)
{
	printf("dm-%i ", mpp->dmi->minor);
	printf("(%s)\n", mpp->alias);
	printf("  wwid = %s\n", mpp->wwid);
	printf("  alias_prefix = %s\n", mpp->alias_prefix);
	printf("  selector = %s\n", mpp->selector);
	printf("  features = %s\n", mpp->features);
	printf("  hwhandler = %s\n", mpp->hwhandler);
	printf("  alias_old = %s\n", mpp->alias_old);
	printf("  retain_hwhandler = %d\n", mpp->retain_hwhandler);

	return 0;
}

int
dump_path (struct path * pp)
{
	struct multipath * mpp;

	printf("    |--dev: %s\n", pp->dev);
	mpp = find_mp_by_wwid(vecs.mpvec, pp->wwid);
	if(mpp)
	{
		printf("       |--multipath device: %s (dm-%i)\n", mpp->alias, mpp->dmi->minor);
	} else {
		printf("       |--multipath device: (none)\n");
	}
	printf("       |--wwid: %s\n", pp->wwid);
	printf("       |--vendor_id: %s\n", pp->vendor_id);
	printf("       |--product_id: %s\n", pp->product_id);
	printf("       |--revision: %s\n", pp->rev);
	printf("       |--state: %i\n", pp->state);
	printf("       |--dmstate: %i\n", pp->dmstate);
	printf("       |--chkrstate: %i\n", pp->chkrstate);
	printf("       |--prio: %i\n", pp->priority);
	printf("       |--pgindex: %i\n", pp->pgindex);
	printf("       |--HCTL: %i:%i:%i:%i\n", pp->sg_id.host_no, pp->sg_id.channel, pp->sg_id.scsi_id, pp->sg_id.lun);

	return 0;
}

int
main (int argc, char *argv[])
{
	//int arg;

	if (load_config(DEFAULT_CONFIGFILE))
		exit(1);
	
	printf ("Loaded multipath.conf. Verbosity is %d\n", conf->verbosity);
	
	/* Temp function */
	load_mp_vecs();
	
	detect_arrays();
	
	exit(0);
}
