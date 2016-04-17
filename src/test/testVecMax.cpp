#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

bool IsGreater (int i,int t) {
  return (i>t);
}

// vector<int> test = {0,0,1,0,2,6,4,10,9,15,23,35,57,36,30,26,14,7,5,2,0,0};
int test[22] = {0,0,1,0,2,6,4,10,9,15,23,35,57,36,30,26,14,7,5,2,0,0};


int main( int argc, char** argv ){

	cout << "SIZE:" << test.size();

	// sort(test.begin(),test.end());

	auto biggest = max_element(begin(test), end(test));
	int posM = distance(begin(test), biggest);

	int minThresh = (int)((*biggest*20)/100);

	int distance = *biggest;
	int compDist;
	int closest;

	for (int i = 0; i < 22; ++i)
	{
		compDist = test[i]-minThresh;
		if(test[i]>minThresh && compDist<distance){
			distance = compDist;
			closest = i;
		}
	}

	cout << "CLOSEST: " << test[closest] << " AT " << closest << endl;

	// auto findMax = find_if(begin(test), end(test),[minThresh](int & m) -> bool { return m> minThresh; });
	// int posFMax = distance(begin(test), findMax);

	// int finalM = *biggest - findMax

	// auto upper = upper_bound(begin(test), end(test),minThresh);
	// int posU = distance(begin(test), upper);

	// auto findMin = find_if(begin(test), end(test),[n](int & m) -> bool { return m> n; });
	// int posFMin = distance(begin(test), findMin);

	// cout << "Max Element: " << *findMax << " at position: " << posFMax << endl;
	// cout << "Max Element: " << *upper << " at position: " << posU << endl;

	for (int i = 0; i < 22; ++i)
	{
		cout << test[i] << " ";
	}
	cout << endl;
}