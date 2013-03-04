# Makefile
#
# Copyright (C) 2003 Christophe Varoqui, <christophe.varoqui@opensvc.com>
#
include ../Makefile.inc

OBJS = main.o detect_arrays.o

CFLAGS += -I$(multipathdir)
LDFLAGS += -lpthread -ldevmapper -ldl -L$(multipathdir) -lmultipath

EXEC = mpatharrays

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC) $(LDFLAGS)

install:

uninstall:

clean:
	rm -f core *.o $(EXEC) *.gz
