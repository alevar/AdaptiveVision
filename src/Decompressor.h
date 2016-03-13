/*==================================================
=========== Message Compression CLASS ==============
====================================================
=== Compresses Image Raw Data to allow faster transmission rate
==================================================*/

#ifndef DEDecompressor_H
#define	DEDecompressor_H

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <map>
#include <iterator>

using namespace std;

class Decompressor {

	public:

		Decompressor();
		Decompressor(string*);
		string unCompress();
		void setDictSize(int);
		virtual ~Decompressor();

		int dictSize;
	
	private:

		string toComp;
		// string w;
		string result;
		string entry;
		map<int,string> dictionary;

};

#endif	/* DEDecompressor_H */

