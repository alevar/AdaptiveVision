// g++ -std=c++11 -o testVecMax2 testVecMax2.cpp

#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool IsGreater (int i,int t) {
  return (i>t);
}

// vector<int> test = {0,0,1,0,2,6,4,10,9,15,23,35,57,36,30,26,14,7,5,2,0,0};
vector<int> test = {90915, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 1, 6, 2, 7, 11, 13, 17, 17, 14, 28, 23, 54, 77, 110, 137, 193, 227, 248, 275, 366, 419, 410, 434, 455, 449, 506, 550, 644, 751, 758, 670, 699, 655, 687, 735, 701, 700, 711, 713, 718, 793, 804, 825, 879, 899, 920, 907, 927, 969, 953, 957, 955, 924, 797, 833, 819, 715, 649, 579, 561, 545, 460, 501, 414, 412, 407, 402, 407, 374, 314, 348, 330, 279, 261, 270, 189, 181, 145, 123, 105, 116, 74, 47, 33, 25, 19, 16, 8, 10, 8, 3, 4, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 190, 0};


int main( int argc, char** argv ){


	// int g = 0;
	// int pos = 0;

	// for (int i = 1; i < test.size()-1; i++){
	// 	if(test[i] > g){
	// 		g = test[i];
	// 		pos = i;
	// 	}
	// }

	// int posPrev = 0;
	// int valPrev = 0;
	// int posCur = 0;
	// int valCur = 0;

	// int minVal = 0;

	// for (int i = 0; i < test.size(); ++i){
	// 	valCur = test[i];
	// 	if(valPrev < 5){
	// 		continue;
	// 	}
	// 	else{
	// 		if(valCur > minThresh){
	// 			minVal = valCur;
	// 		}
	// 	}
	// }
	// cout << "GREAT G " << g << " AT " << pos << " +++ " << test[pos] <<endl;

	// cout << "SIZE:" << test.size();

	// sort(test.begin(),test.end());

	test.erase (test.begin()+255,test.end());
    test.erase (test.begin(),test.begin()+1);

    vector<int> newTest = {};
    for (int i = 0; i < test.size(); ++i){
    	if(i<2 || i>test.size()-2){
    		newTest.push_back(test[i]);
    	}
    	else{

	    	int mean = (*prev(prev(test.begin()+i))+*prev(test.begin()+i)+*(test.begin()+i)+*next(test.begin()+i)+*next(next(test.begin()+i)))/5;
	    	newTest.push_back(mean);
    	}
    }

    for (int i = 0; i < newTest.size(); i++){
		cout << i << " " << newTest[i] << endl;
	}

	auto biggest = max_element(begin(newTest), end(newTest));
	int posM = biggest-begin(newTest);

	int minThresh = (int)((*biggest*1)/100);

	int greatest = *biggest;
	
	// int min = 1+(std::upper_bound (begin(test), end(test), (greatest/5))-begin(test));
 //    reverse(begin(test),end(test));
 //    int max = 1+test.size()-(std::upper_bound (begin(test), end(test), (greatest/5))-begin(test));
 //    reverse(begin(test),end(test));

    int min = distance( begin(newTest), find_if( begin(newTest), end(newTest), [minThresh](int x) { return x >= minThresh; }));
    reverse(begin(newTest),end(newTest));
    int max = newTest.size()-distance( begin(newTest), find_if( begin(newTest), end(newTest), [minThresh](int x) { return x >= minThresh; }));
    reverse(begin(newTest),end(newTest));

	// cout << "GREAT: " << greatest << " AT: " << posM << endl;
	cout << "MIN THRESH" << minThresh << " min: " << min << " max " << max << endl;
}