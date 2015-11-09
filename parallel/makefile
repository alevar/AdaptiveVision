# gcc stereo_detect_parallel.cpp -o stereo_detect_parallel -std=c++11 `pkg-config opencv --cflags --libs` -fopenmp  -lstdc++
stereo_detect: stereo_detect_parallel.o PID.o
	g++ -std=c++11 -o  stereo_detect_parallel stereo_detect_parallel.o PID.o `pkg-config opencv --cflags --libs` -fopenmp -lstdc++

stereo_detect.o: stereo_detect_parallel.cpp PID.h
	g++ -c -std=c++11 -o stereo_detect_parallel.o stereo_detect_parallel.cpp -fopenmp -lstdc++

PID.o: PID.cpp PID.h
	g++ -c -std=c++11 -o PID.o PID.cpp
