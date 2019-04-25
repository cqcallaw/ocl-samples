#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

// Compute c = a + b.

int main()
{
	const size_t N = 32;

	try
	{
		// Get list of OpenCL platforms.
		std::vector<cl::Platform> platform;
		cl::Platform::get(&platform);

		if (platform.empty())
		{
			std::cerr << "OpenCL platforms not found." << std::endl;
			return 1;
		}

		// Get first available GPU device which supports double precision.
		cl::Context context;
		std::vector<cl::Device> device;
		for (auto p = platform.begin(); device.empty() && p != platform.end(); p++)
		{
			std::vector<cl::Device> pldev;

			try
			{
				p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

				for (auto d = pldev.begin(); device.empty() && d != pldev.end(); d++)
				{
					if (!d->getInfo<CL_DEVICE_AVAILABLE>())
						continue;

					std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

					if (
						ext.find("cl_khr_fp64") == std::string::npos &&
						ext.find("cl_amd_fp64") == std::string::npos)
						continue;

					device.push_back(*d);
					context = cl::Context(device);
				}
			}
			catch (...)
			{
				device.clear();
			}
		}

		if (device.empty())
		{
			std::cerr << "GPUs with double precision not found." << std::endl;
			return 1;
		}

		// Create command queue.
		cl::CommandQueue queue(context, device[0]);

		// Compile OpenCL program for found device.
		std::ifstream source_file("main.cl");
		std::string source_code(std::istreambuf_iterator<char>(source_file), (std::istreambuf_iterator<char>()));
		cl::Program program(context, cl::Program::Sources(
										 1, std::make_pair(source_code.c_str(), source_code.length())));

		try
		{
			program.build(device);
		}
		catch (const cl::Error &)
		{
			std::cerr
				<< "OpenCL compilation error" << std::endl
				<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device[0])
				<< std::endl;
			return 1;
		}

		cl::Kernel add(program, "main_kernel");

		// Prepare input data.
		std::vector<double> a(N, 1);
		std::vector<double> b(N, 2);
		std::vector<double> c(N);

		// Allocate device buffers and transfer input data to device.
		cl::Buffer A(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 a.size() * sizeof(double), a.data());

		cl::Buffer B(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
					 b.size() * sizeof(double), b.data());

		cl::Buffer C(context, CL_MEM_READ_WRITE,
					 c.size() * sizeof(double));

		// Set kernel parameters.
		add.setArg(0, A);
		add.setArg(1, B);
		add.setArg(2, C);

		// Setup event handler
		cl::Event* event_handler = new cl::Event();

		// Launch kernel on the compute device.
		queue.enqueueNDRangeKernel(add, cl::NullRange, cl::NDRange(N, 1, 1), cl::NullRange, nullptr, event_handler);

		// Get result back to host.
		queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(double), c.data());

		// Wait for kernel to complete before reading results
		event_handler->wait();

		delete event_handler;

		// Should get N number of lines, each with result 3
		for(uint i = 0; i < N; ++i){
			std::cout << "[" << i << "]: " << c[i] << std::endl;
		}
	}
	catch (const cl::Error &err)
	{
		std::cerr
			<< "OpenCL error: "
			<< err.what() << "(" << err.err() << ")"
			<< std::endl;
		return 1;
	}
}
