hello: src/hello/hello.cpp src/hello/hello.cl
	g++ -std=c++0x -o hello src/hello/hello.cpp -lOpenCL

sandbox: src/sandbox/main.cpp src/sandbox/sandbox.cl
	g++ -std=c++0x -o sandbox src/sandbox/main.cpp -lOpenCL

clean:
	rm -rf hello sandbox