#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <assert.h>
#include <chrono>

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>

int main()
{
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
		std::vector<cl::Device> devices;
		for (auto p = platform.begin(); devices.empty() && p != platform.end(); p++)
		{
			std::vector<cl::Device> pldev;

			try
			{
				p->getDevices(CL_DEVICE_TYPE_GPU, &pldev);

				for (auto d = pldev.begin(); devices.empty() && d != pldev.end(); d++)
				{
					if (!d->getInfo<CL_DEVICE_AVAILABLE>())
						continue;

					std::string ext = d->getInfo<CL_DEVICE_EXTENSIONS>();

					if (
						ext.find("cl_khr_fp64") == std::string::npos &&
						ext.find("cl_amd_fp64") == std::string::npos)
						continue;

					devices.push_back(*d);
					context = cl::Context(devices);
				}
			}
			catch (...)
			{
				devices.clear();
			}
		}

		if (devices.empty())
		{
			std::cerr << "GPUs with double precision not found." << std::endl;
			return 1;
		}

		std::cout << "Using device " << devices[0].getInfo<CL_DEVICE_NAME>() << std::endl;

		auto dimensions = devices[0].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();

		std::cout << "Max dimensions: " << dimensions[0] << "x" << dimensions[1] << "x" << dimensions[2] << std::endl;
		size_t global_dim = dimensions[0] * dimensions[1] * dimensions[2];

		// Create command queue.
		cl::CommandQueue queue(context, devices[0]);

		// Compile OpenCL program for found device.
		std::ifstream source_file("src/sandbox/sandbox.cl");
		std::string source_code(
			std::istreambuf_iterator<char>(source_file),
			(std::istreambuf_iterator<char>()));
		cl::Program program(
			context,
			cl::Program::Sources(
				1, std::make_pair(source_code.c_str(), source_code.length())));

		try
		{
			program.build(devices);
		}
		catch (const cl::Error &)
		{
			std::cerr
				<< "OpenCL compilation error" << std::endl
				<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devices[0])
				<< std::endl;
			return 1;
		}

		cl::Kernel add(program, "sandbox");

		// Prepare input data.
		std::vector<double> a(global_dim, 1);
		std::vector<double> b(global_dim, 2);
		std::vector<double> c(global_dim);

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
		cl::Event *event_handler = new cl::Event();

		auto begin = std::chrono::high_resolution_clock::now();

		// Launch kernel on the compute device.
		queue.enqueueNDRangeKernel(
			add,
			cl::NullRange,
			cl::NDRange(
				global_dim,
				1,
				1),
			cl::NullRange,
			nullptr, event_handler);

		// Wait for kernel to complete so we get accurate timing
		event_handler->wait();

		auto end = std::chrono::high_resolution_clock::now();
		std::cout
			<< "Executed " << global_dim << " work items in "
			<< std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()
			<< "ms" << std::endl;

		// Get result back to host.
		queue.enqueueReadBuffer(C, CL_TRUE, 0, c.size() * sizeof(double), c.data(), nullptr, event_handler);

		// Wait for read back to complete before verifying results
		event_handler->wait();

		delete event_handler;

		// Should get <global_dim> number of lines, each with result 3
		for (uint i = 0; i < global_dim; ++i)
		{
			assert(c[i] == 3);
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
