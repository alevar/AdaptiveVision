client.e: client.o OpCode.o
	g++ -std=c++11 -o client client.o OpCode.o

client.o: client.cpp OpCode.h
	g++ -c -std=c++11 -o client.o client.cpp

OpCode.o: OpCode.cpp OpCode.h
	g++ -c -std=c++11 -o OpCode.o OpCode.cpp

server.e: server.o OpCode.o
	g++ -std=c++11 -o server server.o OpCode.o

server.o: server.cpp OpCode.h
	g++ -c -std=c++11 -o server.o server.cpp

OpCode.o: OpCode.cpp OpCode.h
	g++ -c -std=c++11 -o OpCode.o OpCode.cpp
