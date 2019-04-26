#if defined(cl_khr_fp64)
#  pragma OPENCL EXTENSION cl_khr_fp64: enable
#elif defined(cl_amd_fp64)
#  pragma OPENCL EXTENSION cl_amd_fp64: enable
#else
#  error double precision is not supported
#endif

kernel void sandbox(global const double *a, global const double *b, global double *c) {
	size_t gid = get_global_id(0);
	c[gid] = a[gid] + b[gid];
};