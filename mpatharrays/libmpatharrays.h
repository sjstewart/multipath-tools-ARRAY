#ifndef _LIBMPATHARRAYS_H
#define _LIBMPATHARRAYS_H

#include <vector.h>

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
        char * dmname;
        int lun;
        vector ctlrs;
};

struct mpa_ctlr {
	char * id;
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

<<<<<<< HEAD
vector arrays;

/* Macros */
#define INIT_MPARRAY_PATHINQ(PATHINQ)  memset(&(PATHINQ), 0, sizeof(struct mpa_pathinq));
=======
struct mparray_pathinq {
	char * array_id;
	char * array_label;
	char * controller_id;
	int owner;
	int preferred;
};

#define INIT_MPARRAY_PATHINQ(PATHINQ)  memset(&(PATHINQ), 0, sizeof(struct mparray_pathinq));
>>>>>>> fce1916b6fa66b11269033937324ebc0da7a4d79
#define CLEAR_MPARRAY_PATHINQ(PATHINQ) hexdump( (PATHINQ).array_id, 16);\
					free( (PATHINQ).array_id );\
					free( (PATHINQ).array_label );\
					free( (PATHINQ).controller_id );


extern struct mparray * alloc_mparray (void);
extern struct mpa_lun * alloc_mpa_lun (void);
extern int add_lun_to_array (struct mparray * mpa, struct mpa_lun * mpal);
extern int add_ctlr_to_lun (struct mpa_lun * mpal, struct mpa_ctlr * mpac);
extern int add_ctlr_to_array (struct mparray * mpa, struct mpa_ctlr * mpac);
extern int add_path_to_ctlr (struct mpa_ctlr * mpac, struct mpa_path * mpap);

#endif /* _LIBMPATHARRAYS_H */
