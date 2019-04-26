# Audience
The following guide is written for developers who are familiar with the Linux Command Line Interface (CLI), and developing C or C++ applications in Linux.

# Verify Hardware Support

## Intel
Most modern Intel GPUs are supported by the NEO compute runtime. For integreated graphics devices, use `lscpu` to get the processor SKU; detailed information for each SKU is available from [ark.intel.com](ark.intel.com)

# Install OCL Headers and ICD Loader
```
$ sudo apt install opencl-c-headers opencl-clhpp-headers
```
The OpenCL API has its own set of header files; the above command installs both C and C++ headers files. The C header can be found in `<CL/cl.h>`; C++ header is in `<CL/cl.hpp>`

```
$ sudo apt install ocl-icd-libopencl1 ocl-icd-opencl-dev
```

OpenCL applications generally link against an OpenCL Installable Compute Device (ICD) loader instead of a specific OpenCL implementation; see [https://github.com/bashbaug/OpenCLPapers/blob/master/OpenCLOnLinux.asciidoc](https://github.com/bashbaug/OpenCLPapers/blob/master/OpenCLOnLinux.asciidoc) for more information about this system. Install the OCL ICD and associated development headers as follows

# Configure Permissions
```
$ sudo usermod -a -G video <user_id>
```

Users running OpenCL applications require direct access to the GPU. This access is granted by membership in the `video` group, so  unprivileged development users must be made members of this group.

# Install Compute Runtime
The final component of the Open Compute development environemt is a compute runtime that manages the interaction between the OpenCL API and the GPU.

## Intel
```
$ sudo add-apt-repository ppa:intel-opencl/intel-opencl
$ sudo apt update
$ sudo apt install intel-opencl
```

Intel GPUs support OpenCL through NEO compute runtime, available as a [Launchpad PPA](https://launchpad.net/~intel-opencl/+archive/ubuntu/intel-opencl)

# Verification
```
$ sudo apt install clinfo
$ clinfo
<platform information should show here>
```

The `clinfo` utility can be used to verify the environment has been properly configured:

# Hello World
```
$ sudo apt install git && git clone https://github.com/cqcallaw/ocl-samples.git && cd ocl-samples
$ make
$ ./hello
```