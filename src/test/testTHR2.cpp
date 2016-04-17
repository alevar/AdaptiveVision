// g++ -std=c++11 -pthread -o testTHR2 testTHR2.cpp `pkg-config opencv --cflags --libs`

#include <iostream>
#include <thread>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

static const int num_threads = 2;

Mat inputSCN1;
Mat inputSCN2;

void call_from_thread1(int tid){
	std::cout << "HELLO WORLD: " << tid << std::endl;
	VideoCapture cap(1);
    if(!cap.isOpened()){
        cout<<"ERROR"<< endl;
    }

    namedWindow("CAM1",1);
    namedWindow("CAM2",1);

    while(true){
      
		cap >> inputSCN1;
		if(inputSCN2.rows != 0){
			imshow("CAM2", inputSCN2);
		}
		imshow("CAM1", inputSCN1);
		
		if(waitKey(30) >= 0){
	            
			break;
		}
	}
}

void call_from_thread2(int tid){
	std::cout << "HELLO WORLD: " << tid << std::endl;
	VideoCapture cap(0);
    if(!cap.isOpened()){
        cout<<"ERROR"<< endl;
    }
    while(true){
      
		cap >> inputSCN2;

	}

}

int main(){
	std::thread t[num_threads];
	// for (int i = 0; i < num_threads; ++i) {
	// 	t[i] = std::thread(call_from_thread, i);
	// }
	std::thread t1(call_from_thread1,0);
	std::thread t2(call_from_thread2,1);
	std::cout << "Launched from the main\n";
	
	t1.join();
	t2.join();
	return 0;
}