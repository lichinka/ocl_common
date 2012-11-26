#include <CL/cl.h>
#include <stdio.h>
#include <string.h>

#include "ocl_common.h" /* OpenCL helper functions */



const char *kernel_source = 
"__kernel void numbering(__global uint *global_mem)      \n"
"{                                                           \n"
"  global_mem[get_global_id(0)] = 2 * get_global_id(0);          \n"
"}                                                           \n";


void enqueue_job(cl_context *context, cl_kernel *kernel, cl_command_queue *queue, cl_event *event)
{
  int      ocl_status;

  int num_items = 50000;

  cl_mem buffer = clCreateBuffer(*context, CL_MEM_WRITE_ONLY, num_items * sizeof(cl_uint), NULL, &ocl_status);
  check_error(ocl_status, "Create buffer");  

  size_t global_work_size = num_items;

  ocl_status = clSetKernelArg(*kernel, 0, sizeof(buffer), (void*) &buffer);
  check_error(ocl_status, "Set Kernel Arg");

  ocl_status = clEnqueueNDRangeKernel(*queue, *kernel, 1, NULL, &global_work_size, NULL, 0, NULL, event);
  check_error(ocl_status, "Enqueue NDRange Kernel");

  /**
  ocl_status = clWaitForEvents(1, &event);
  check_error(ocl_status, "wait for event");
  */
  /* Looks at output * /
  cl_uint *ptr;
  ptr = (cl_uint *) clEnqueueMapBuffer(*queue, buffer, CL_TRUE, CL_MAP_READ, 0, num_items * sizeof(cl_uint), 0, NULL, NULL, NULL);
  int i;
  for (i = 0; i < 100; i++)
  {
    printf("%d %d\n", i, ptr[i]);
  } */
}



int main(int argc, char **argv)
{
  cl_context        context;
  cl_command_queue *list_queues;
  uint              num_queues;
  cl_kernel         kernel;

  num_queues = 4;
  set_opencl_env_multiple_queues(num_queues, &list_queues, &context);

  build_kernel (&context, "numbering", &kernel_source, NULL, &kernel);

  int i, j;
  int loop_out = 100;

  cl_event events[num_queues];

  for (i = 0; i < loop_out; i++)
  {
    for (j = 0; j < num_queues; j++)
    {
      if (i > 0)
      {
        int wait_status = clWaitForEvents(1, &(events[j]));
        check_error(wait_status, "Wait events");
      }
      enqueue_job(&context, &kernel, &(list_queues[j]), &(events[j]));

    }
  }

  release_opencl(num_queues, &list_queues, &context);

  return 0;
}


/* EOF */
