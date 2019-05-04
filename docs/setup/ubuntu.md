# Verify Hardware Support

Most modern GPUs support OpenCL. For integrated graphics devices (iGPUs), use `lscpu` to get the processor SKU. Detailed information for Intel SKUs is available from [ark.intel.com](ark.intel.com). Detailed information for AMD processors is available from [AMD's product page](https://www.amd.com/en/products/specifications/processors).

# Configure Permissions
```
$ sudo usermod -a -G video <user_id>
```

Users running OpenCL applications require direct access to the GPU; this access is granted by membership in the `video` group.

# Install OCL ICD Loader
```
$ sudo apt install ocl-icd-libopencl1 ocl-icd-opencl-dev
```

OpenCL applications generally link against an OpenCL Installable Compute Device (ICD) loader instead of a specific OpenCL implementation; see [https://github.com/bashbaug/OpenCLPapers/blob/master/OpenCLOnLinux.asciidoc](https://github.com/bashbaug/OpenCLPapers/blob/master/OpenCLOnLinux.asciidoc) for more information about this system.

# Install OCL Headers
```
$ sudo apt install opencl-c-headers opencl-clhpp-headers
```
The OpenCL API has its own set of header files; the above command installs both C and C++ headers files. The C header can be found in `<CL/cl.h>`; the C++ header is in `<CL/cl.hpp>`.

# Install Compute Runtime
OpenCL requires a compute runtime to manage the interaction between the OpenCL API and the GPU.

### AMD

TODO

### Intel
Intel's OpenCL support is provided through the NEO compute runtime, available as a PPA.

```
$ sudo add-apt-repository ppa:intel-opencl/intel-opencl
$ sudo apt update
$ sudo apt install intel-opencl
```

### Nvidia
Nvidia provides OpenCL support through their proprietary driver, available from the [graphics-drivers PPA](https://launchpad.net/~graphics-drivers/+archive/ubuntu/ppa).

```
sudo add-apt-repository ppa:graphics-drivers/ppa
sudo apt update
sudo apt install nvidia-graphics-drivers-<version>
```

# Verification
The `clinfo` utility can be used to verify the environment has been properly configured.

```
$ sudo apt install clinfo
$ clinfo
<platform information should show here>
```

# Troubleshooting

If `clinfo` indicates there are 0 supported platforms:

1. Verify your host has OpenCL-capable hardware attached
2. Verify clinfo is running as a user with direct GPU access (member of the `video` group)
3. Verify the correct compute runtime is installed