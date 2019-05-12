# Audience
This code and documentation is written for developers who are familiar with C/C++ application development in Linux and want to explore OpenCL application development.

# Why OpenCL?

Even with [many](https://github.com/Erkaman/vulkan_minimal_compute) [well-known](https://www.khronos.org/opengl/wiki/Compute_Shader) [compute](https://docs.microsoft.com/en-us/windows/desktop/direct3d11/direct3d-11-advanced-stages-compute-shader) [alternatives](https://developer.nvidia.com/cuda-zone), I have yet to find another API with OCL's ability to run on multiple platforms and in multiple environments. CUDA is platform-specific; DirectX is OS-specific. OpenGL compute shaders are okay, but aren't designed to run on heterogenous platforms and require familiarity with GLSL (OpenCL kernels are generally written in a derivative of the C99 language specification). For serious GPGPU work, OCL wins.

# Environment Setup

* [Ubuntu](docs/setup/ubuntu.md)

# Hello World

Follow the environment setup for your OS and platform, making sure to include the verification step.

```
$ git clone https://github.com/cqcallaw/ocl-samples.git && cd ocl-samples
$ make
$ ./hello
```

Sample output:

```
$ ./hello
Using device GeForce GTX 1080
Max dimensions: 1024x1024x64
Computed 67108864 values in 128ms
$
```

# Further Reading

* [Khronos OpenCL page](https://www.khronos.org/opencl/)
* [OpenCL return codes](https://streamhpc.com/blog/2013-04-28/opencl-error-codes/)
* [SimpleOpenCLSamples](https://github.com/bashbaug/SimpleOpenCLSamples)
