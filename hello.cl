#if defined(cl_khr_fp64)
#  pragma OPENCL EXTENSION cl_khr_fp64: enable
#elif defined(cl_amd_fp64)
#  pragma OPENCL EXTENSION cl_amd_fp64: enable
#else
#  error double precision is not supported
#endif

kernel void hello_kernel(global const double *a, global const double *b, global double *c) {
	size_t gid_x = get_global_id(0);
	size_t gid_y = get_global_id(1);
	size_t gid_z = get_global_id(2);

	// get necessary dimensions for buffer offsets
	size_t columns = get_global_size(0);
	size_t rows = get_global_size(1);

	// compute buffer offset from dimensional information
	size_t buffer_offset = gid_x + gid_y * columns + gid_z * columns * rows;

	// do computation, though kernel runtime is dominated by address offset computations
	*(c + buffer_offset) = *(a + buffer_offset) + *(b + buffer_offset);
};