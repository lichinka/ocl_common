#ifndef COMMON_OCL
#define COMMON_OCL

#define _WORK_ITEMS_PER_DIMENSION_  16

#include <stdio.h>
#include <string.h>
#include <CL/cl.h>



/**
 * A structure to hold all the OpenCL objects together.
 */
typedef struct OCL_object
{
    cl_int            status;
    cl_event          *events;
    cl_kernel         kernel;
    cl_context        context;
    cl_program        *program;
    cl_device_id      device_id;
    cl_platform_id    platform_id;
    cl_command_queue  *queues;
    int               queue_count;
    char              is_initialized;
} OCL_object;



/**
 * Initializes the OpenCL platform. 
 * This function should be called before any other one.
 *
 * ocl_obj          A pointer to the uninitialized OCL_object structure on
 *                  the user's side;
 * number_of_queues the number of queues to initialize;
 * device_hint      tries to select the device with the given id.-
 *
 */
OCL_object* init_opencl (OCL_object *ocl_obj,
                         const int number_of_queues,
                         const int device_hint);

/**
 * Initializes the OpenCL platform. 
 * This function should be called before any other one.-
 *
 */
void init_platform (cl_platform_id *platform, 
                    cl_device_id *device, 
                    cl_context *context,
                    cl_command_queue *queue);

/**
 * Shuts OpenCL down, deallocating all referenced memory.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side.-
 *
 */
void deactivate_opencl (OCL_object *ocl_obj);

/* Setting opencl by inititalizing 1 device on 1 context with num_queues queues */
void set_opencl_env_multiple_queues(int num_queues, cl_command_queue **list_queues, cl_context *context);

/* Releases the opencl queues and memory used */
void release_opencl(int num_queues, cl_command_queue **list_queues, cl_context *context);

/**
 * Builds a kernel for a given device and context from a source file.-
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * file_name    file containing the OpenCL kernel code;
 * kernel_name  name of the kernel to activate;
 * options      compile-time options, like include directories or defines.-
 *
 */
void build_kernel_from_file (OCL_object *ocl_obj,
                             const char *file_name, 
                             char *kernel_name, 
                             const char *options);

/**
 * Builds a kernel for a given device and context from string buffer, returning a pointer to the compiled `cl_program`.
 *
 */
cl_program* build_kernel (cl_context *context, 
                          char *kernel_name, 
                          const char **kernel_source, 
                          const char *options,
                          cl_kernel *kernel);

/**
 * Attaches the received value as a kernel argument.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_size         size (in bytes) of the argument being passed;
 * arg_value_ptr    pointer to the argument value.-
 *
 */
void set_kernel_value_arg (OCL_object *ocl_obj,
                           cl_uint arg_index, 
                           size_t arg_size, 
                           const void *arg_value_ptr);

/**
 * Attaches the received memory object as a kernel argument.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_value_ptr    pointer to the argument value.-
 *
 */
void set_kernel_mem_arg (OCL_object *ocl_obj,
                         cl_uint arg_index, 
                         const void *arg_value_ptr);

/**
 * Attaches the received integer value as a kernel argument.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_value_ptr    pointer to the argument value.-
 *
 */
void set_kernel_int_arg (OCL_object *ocl_obj,
                         cl_uint arg_index, 
                         const int *arg_value_ptr);

/**
 * Attaches the received float value as a kernel argument.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_value_ptr    pointer to the argument value.-
 *
 */
void set_kernel_float_arg (OCL_object *ocl_obj,
                           cl_uint arg_index, 
                           const float *arg_value_ptr);

/**
 * Attaches the received double value as a kernel argument.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_value_ptr    pointer to the argument value.-
 *
 */
void set_kernel_double_arg (OCL_object *ocl_obj,
                           cl_uint arg_index, 
                           const double *arg_value_ptr);

/**
 * Marks a kernel parameter as local memory.-
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * arg_index        argument index in the kernel-function prototype;
 * arg_size         size (in bytes) of the allocated local memory.-
 *
 */
void set_local_mem (OCL_object *ocl_obj,
                    cl_uint arg_index, 
                    size_t arg_size);

/**
 * Creates and returns a OpenCL buffer, i.e. cl_mem object.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * flags        memory flags, as defined by OpenCL;
 * size         size (in bytes) of the buffer to create.-
 *
 */
cl_mem create_buffer (OCL_object *ocl_obj,
                      cl_mem_flags flags, 
                      size_t size);

/**
 * Clears an OpenCL buffer, i.e., cl_mem object.
 *
 * mem_obj      A pointer to the cl_mem object.-
 *
 */
void release_buffer (cl_mem *mem_obj);

/**
 * Reads an OpenCL buffer from the device, returning a pointer to the 
 * associated `cl_event` object.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * queue_index  index of the command queue on which the read is performed;
 * cl_mem_ptr   pointer to a valid cl_mem object, from which to copy;
 * size         size (in bytes) of the data to read from the device;
 * target_ptr   pointer to the data to which to copy, on the host.-
 *
 */
cl_event* read_buffer (OCL_object *ocl_obj,
                       int queue_index,
                       cl_mem *cl_mem_ptr, 
                       size_t size, 
                       void *target_ptr);

/**
 * Reads an OpenCL buffer from the device, waiting for the operation to
 * finish before returning.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * queue_index  index of the command queue on which the read is performed;
 * cl_mem_ptr   pointer to a valid cl_mem object, from which to copy;
 * size         size (in bytes) of the data to read from the device;
 * target_ptr   pointer to the data to which to copy, on the host.-
 *
 */
void read_buffer_blocking (OCL_object *ocl_obj,
                           int queue_index,
                           cl_mem *cl_mem_ptr, 
                           size_t size, 
                           void *target_ptr);

/**
 * Enqueues a write operation of an OpenCL buffer to the device, returning
 * the associated event object.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * queue_index  index of the command queue on which the write is performed;
 * cl_mem_ptr   pointer to a valid cl_mem object;
 * size         size (in bytes) of the data to write to the device;
 * source_ptr   to the data from which to copy, on the host.-
 *
 */
cl_event* write_buffer (OCL_object *ocl_obj,
                        int queue_index,
                        cl_mem *cl_mem_ptr, 
                        size_t size, 
                        const void *source_ptr);

/**
 * Writes an OpenCL buffer to the device, waiting for the operation to
 * finish before returning.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * queue_index  index of the command queue on which the write is performed;
 * cl_mem_ptr   pointer to a valid cl_mem object;
 * size         size (in bytes) of the data to write to the device;
 * source_ptr   to the data from which to copy, on the host.-
 *
 */
void write_buffer_blocking (OCL_object *ocl_obj,
                            int queue_index,
                            cl_mem *cl_mem_ptr, 
                            size_t size, 
                            const void *source_ptr);


/**
 * Executes the kernel, using the received 1D range, waiting for it to finish.
 *
 * ocl_obj          A pointer to the initialized OCL_object structure on the
 *                  user's side;
 * queue_index      index of the command queue on which the kernel execution is 
 *                  performed;
 * global_offsets   offsets used for the kernel range (1 element). If NULL
 *                  is given, the offsets used are zero;
 * global_sizes     global execution range sizes (1 element);
 * local_sizes      local execution range sizes (1 element);
 *
 */
void run_kernel_1D_blocking (OCL_object *ocl_obj,
                             int queue_index,
                             const size_t *global_offsets,
                             const size_t *global_sizes,
                             const size_t *local_sizes);

/**
 * Executes the kernel, using the received 2D range, waiting for it to finish.
 *
 * ocl_obj      A pointer to the initialized OCL_object structure on the
 *              user's side;
 * queue_index  index of the command queue on which the write is performed;
 * cl_mem_ptr   pointer to a valid cl_mem object;
 * size         size (in bytes) of the data to write to the device;
 * source_ptr   to the data from which to copy, on the host.-
 *
 */
void run_kernel_2D_blocking (OCL_object *ocl_obj,
                             int queue_index,
                             const size_t *global_offsets,
                             const size_t *global_sizes,
                             const size_t *local_sizes);

/**
 * Activates the specified kernel by name.
 *
 */
void activate_kernel (OCL_object *ocl_obj,
                      const char *kernel_name);

/**
 * Check and prints OpenCL errors.-
 *
 */
void check_error (int status, char *msg);

/**
 * Internal helper functions not meant to be directly used within an application 
 */
void print_device_information (cl_device_id *device);
/**
 * Queries the system for a OpenCL platform, returning it
 * in the input parameter.-
 *
 * platform         Output parameter.-
 */
void get_platform (cl_platform_id *platform);

/**
 * Activates a devices and its context within the given platform.
 *
 * device_hint  tries to select the device with the given ID.-
 *
 */
void set_device_and_context (cl_platform_id *platform, 
                             cl_device_id *device, 
                             cl_context *context,
                             const int device_hint);

#endif /* COMMON_OCL */
