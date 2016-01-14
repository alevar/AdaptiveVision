stereo_detect: test.o cameraDetect.o
	g++ -std=c++11 -o test test.o cameraDetect.o `pkg-config opencv --cflags --libs` -lstdc++

stereo_detect.o: test.cpp cameraDetect.h
	g++ -c -std=c++11 -o test.o test.cpp -fopenmp -lstdc++

cameraDetect.o: cameraDetect.cpp cameraDetect.h
	g++ -c -std=c++11 -o cameraDetect.o cameraDetect.cpp
