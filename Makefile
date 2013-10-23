CC 	= gcc
CFLAGS 	= -g -O0 -Wall -fbounds-check -fPIC
INCS 	= -I. -I$(ATISTREAMSDKROOT)/include
LIBS 	= -L$(ATISTREAMSDKROOT)/lib/x86_64 -lOpenCL
OBJS 	= ocl_common.o multi_queue_test.o
BIN  	= cl_test
BINLIB	= liboclcommon.so

all: $(BIN) $(BINLIB)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) -o $@ $(OBJS) $(LIBS)

$(BINLIB): $(OBJS)
	$(CC) $(CFLAGS) $(INCS) -shared -Wl,-soname,$@ -o $@ ocl_common.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BIN) $(BINLIB)
