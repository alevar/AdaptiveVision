#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <iterator>
#include <vector>

#include <fstream>
#include <sstream>

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"

using namespace std;
using namespace cv;

string compress(const string &uncompressed) {

	int dictSize = 256;
	map<string,int> dictionary;
	for (int i = 0; i < 256; i++)
    	dictionary[string(1, i)] = i;
 
	string w;
	string final;
	for (string::const_iterator it = uncompressed.begin();it != uncompressed.end(); ++it) {
	    char c = *it;
	    string wc = w + c;
	    if (dictionary.count(wc))
	    	w = wc;
	    else {
	    	if(dictionary[w]<255){
	    		bitset<8> mybits1(dictionary[w]);
		    	bitset<8> mybits2(0);
	    		long i = mybits1.to_ulong(); 
				char x = static_cast<unsigned char>( i );
				final = final + x;
				i = mybits2.to_ulong(); 
				x = static_cast<unsigned char>( i );
				final = final + x;
	    	}
	    	else{
	    		bitset<8> mybits1((dictionary[w] & 0xFF));
		    	bitset<8> mybits2(((dictionary[w] >> 8) & 0xFF));
	    		long i = mybits1.to_ulong(); 
				char x = static_cast<unsigned char>( i );
				final = final + x;
				i = mybits2.to_ulong(); 
				x = static_cast<unsigned char>( i );
				final = final + x;
	    	}

	    	// Add wc to the dictionary.
	    	dictionary[wc] = dictSize++;
	    	w = string(1, c);
	    }
	}

	if (!w.empty()){
	    bitset<8> mybits1((dictionary[w] & 0xFF));
    	bitset<8> mybits2(((dictionary[w] >> 8) & 0xFF));
		long i = mybits1.to_ulong(); 
		char x = static_cast<unsigned char>( i );
		final = final + x;
		i = mybits2.to_ulong(); 
		x = static_cast<unsigned char>( i );
		final = final + x;
	}

	return final;
}

string decompress(string input) {
  // Build the dictionary.
  int dictSize = 256;
  map<int,string> dictionary;
  for (int i = 0; i < 256; i++)
    dictionary[i] = string(1, i);
 
  string w(1,input[0]);
  string result = w;
  string entry;
  for (int i=2 ;i<=input.length(); i++) {

  	if(int(input[i+1]) == 0){
  		int k = int(input[i]);
	    if (dictionary.count(k)){
	  		i++;
	      	entry = dictionary[k];
	    }
	    else if (k == dictSize)
	      entry = w + w[0];
	    else
	      throw "Bad compressed k";
  	}
  	else{
  		int k = int(input[i])+256;
  		if (dictionary.count(k)){
  			i++;
	      	entry = dictionary[k];
	    }
	    else if (k == dictSize)
	      entry = w + w[0];
	    else
	      throw "Bad compressed k";
  	}    
 
    result += entry;
 
    dictionary[dictSize++] = w + entry[0];
 
    w = entry;
  }
  return result;
}

int main() {
	
	Mat inputTPL = imread("/home/sparrow/Pictures/Webcam/ball.jpg");

	Mat image = (inputTPL.reshape(0,1));
	// int  imgSize = image.total()*image.elemSize()

	unsigned char* test1 = image.data;
	// const char * c = (const char*)test1;
	// string test2 = c;

	// string str;
	// str = reinterpret_cast<char*>(test1);
	// std::string str(reinterpret_cast<char*>(test1));

	stringstream s;
	s << test1;

	string str_my_txt = s.str();

	string test = str_my_txt.substr(0,100);
	cout << test << endl;

  	string result = compress(test);

  	for(int i=0; i<test.length(); i++){
  		cout << int(test[i]) << endl;
  	}

  	ofstream myfile ("test.txt");
	if (myfile.is_open())
	{
		myfile << test;
		myfile << "\n";
		myfile << result;
		myfile.close();
	}
	else cout << "Unable to open file";

	// int N = result.length();  // bound on string length
	// bitset<256> b;
	// for (int i = 0; i < result.length(); ++i) {
	//   char c = result[i];
	//   for (int j = 7; j >= 0 && c; --j) {
	//     if (c & 0x1) {
	//       b.set(8 * i + j);
	//     }
	//     c >>= 1;
	//   }
	// }

	string result2 = decompress(result);
}