/*==================================================
=========== Message Compression CLASS ==============
====================================================
=== Compresses Image Raw Data to allow faster transmission rate
==================================================*/

#ifndef COMPRESSOR_H
#define	COMPRESSOR_H

#include <string>
#include <vector>
#include <iostream>
#include <bitset>
#include <map>
#include <iterator>

using namespace std;

class Compressor {

	public:

		Compressor();
		Compressor(string*);
		string compress();
		void setDictSize(int);
		virtual ~Compressor();

		int dictSize;
	
	private:

		string toComp;
		string compressed;
		map<string,int> dictionary;
		string w;
		char c;
		string wc;
		long i;
		char x;

};

#endif	/* COMPRESSOR_H */

