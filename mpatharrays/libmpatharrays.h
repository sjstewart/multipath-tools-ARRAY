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

extern struct mparray * alloc_mparray (void);
extern struct mparray_lun * alloc_mparray_lun (void);
extern int add_lun_to_array (struct mparray * mpa, struct mparray_lun * mpal);
extern int add_path_to_lun (struct mparray_lun * mpal, struct mparray_path * mpap);

#endif /* _LIBMPATHARRAYS_H */
