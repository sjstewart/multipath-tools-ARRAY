#ifndef _LIBMPATHARRAYS_H
#define _LIBMPATHARRAYS_H

#include <vector.h>
#include <structs.h>
#include <structs_vec.h>

struct mparray {
        int testint;
	char * id;
	char * label;
        vector luns;
	vector ctlrs;
};

struct mpa_lun {
        char * wwid;
        char * name;
        int minor;
        int lun;
        vector ctlrs;
};

struct mpa_ctlr {
	char * id;
	int owner;
	int preferred;
	vector paths;
};

struct mpa_path {
        char * devname;
        int host;
        int controller;
        int target;
        int lun;
};

struct mpa_pathinq {
	char * array_id;
	char * array_label;
	char * controller_id;
	int owner;
	int preferred;
};

vector arrays;

/* Macros */
#define INIT_MPARRAY_PATHINQ(PATHINQ)  memset(&(PATHINQ), 0, sizeof(struct mpa_pathinq));
#define CLEAR_MPARRAY_PATHINQ(PATHINQ)  free( (PATHINQ).array_id );\
					free( (PATHINQ).array_label );\
					free( (PATHINQ).controller_id );


extern struct mparray * alloc_mparray (struct mpa_pathinq * mpinq);
extern struct mpa_lun * alloc_mpa_lun (struct path * pp, struct vectors vec);
extern struct mpa_ctlr * alloc_mpa_ctlr (struct mpa_pathinq * mpinq);
extern struct mpa_path * alloc_mpa_path (struct path * pp);
extern void free_mparray (struct mparray * mpa);
extern void free_mpa_lun (struct mpa_lun * mpal);
extern void free_mpa_ctlr (struct mpa_ctlr * mpac);
extern void free_mpa_path (struct mpa_path *mpap);
extern struct mpa_lun * add_lun_to_array (struct mparray * mpa, struct path * pp, struct vectors vec);
extern struct mpa_ctlr * add_ctlr_to_lun (struct mpa_lun * mpal, struct mpa_pathinq * mpinq);
extern struct mpa_ctlr * add_ctlr_to_array (struct mparray * mpa, struct mpa_pathinq * mpinq);
extern struct mpa_path * add_path_to_ctlr (struct mpa_ctlr * mpac, struct path * pp);
extern int add_path (struct path * pp, struct mpa_pathinq * mpinq, struct vectors vec);

extern void dump_mparray (struct mparray * mpa);
void dump_mpa_lun (struct mpa_lun * mpal);
void dump_mpa_ctlr (struct mpa_ctlr * mpac);
void dump_mpa_path (struct mpa_path * mpap);


#endif /* _LIBMPATHARRAYS_H */
