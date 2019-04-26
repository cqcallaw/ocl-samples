hello: src/hello/hello.cpp
	g++ -std=c++0x -o hello src/hello/hello.cpp -lOpenCL

clean:
	rm -rf hello