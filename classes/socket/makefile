client: client.o OpCode.o
	g++ -std=c++11 -o client client.o OpCode.o

stereo_detect.o: client.cpp OpCode.h
	g++ -c -std=c++11 -o client.o client.cpp -fopenmp -lstdc++

OpCode.o: OpCode.cpp OpCode.h
	g++ -c -std=c++11 -o OpCode.o OpCode.cpp
