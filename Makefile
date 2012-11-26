CC = gcc 
CFLAGS = -Wall -fbounds-check
INCS = -I. 
LIBS = -lOpenCL
OBJS = ocl_common.o multi_queue_test.o

all: cl_test 

cl_test: $(OBJS)
	$(CC) $(CFLAGS) $(INCS) -o $@ $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f *.o cl_test
