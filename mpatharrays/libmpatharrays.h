#ifndef _LIBMPATHARRAYS_H
#define _LIBMPATHARRAYS_H

#include <vector.h>

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

struct mparray_pathinq {
	char * array_id;
	char * array_label;
	char * controller_id;
	int owner;
	int preferred;
};

#define INIT_MPARRAY_PATHINQ(PATHINQ)  memset(&(PATHINQ), 0, sizeof(struct mparray_pathinq));
#define CLEAR_MPARRAY_PATHINQ(PATHINQ) hexdump( (PATHINQ).array_id, 16);\
					free( (PATHINQ).array_id );\
					free( (PATHINQ).array_label );\
					free( (PATHINQ).controller_id );


extern struct mparray * alloc_mparray (void);
extern struct mparray_lun * alloc_mparray_lun (void);
extern int add_lun_to_array (struct mparray * mpa, struct mparray_lun * mpal);
extern int add_path_to_lun (struct mparray_lun * mpal, struct mparray_path * mpap);

#endif /* _LIBMPATHARRAYS_H */
