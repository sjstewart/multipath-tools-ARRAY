#ifndef MPATHARRAY_INQUIRER
#define MPATHARRAY_INQUIRER

#define INQUIRER_NAME_LEN 8

extern int inquire_arrays (struct vectors vecs);
int add_inquirer (const char * inq, void * inq_function);
extern void init_inquirers (void);

struct inquirer {
	struct list_head node;
	char * name;
	int (*inquirer_fn)(struct mpa_pathinq * mpap, struct path * pp);
};


static LIST_HEAD(inquirers);

extern int
rdac_inquirer(struct mpa_pathinq * mpap, struct path * pp);


#endif /* MPATHARRAY_INQUIRER */
