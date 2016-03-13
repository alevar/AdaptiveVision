/*==================================================
==================================================*/

#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <iterator>
#include <vector>

#include "Compressor.h"

using namespace std;

Compressor::Compressor(){
	this->dictSize = 256;
}

Compressor::Compressor(string *input) {
	toComp = *input;
	this->dictSize = 256;
}

string Compressor::compress(){
	
	for (int i = 0; i < dictSize; i++)
    	dictionary[string(1, i)] = i;

	for (string::const_iterator it = toComp.begin();it != toComp.end(); ++it) {
	    c = *it;
	    wc = w + c;
	    if (dictionary.count(wc))
	    	w = wc;
	    else {
	    	if(dictionary[w]<255){
	    		bitset<8> mybits1(dictionary[w]);
		    	bitset<8> mybits2(0);
	    		i = mybits1.to_ulong(); 
				x = static_cast<unsigned char>( i );
				this->compressed = this->compressed + x;
				i = mybits2.to_ulong(); 
				x = static_cast<unsigned char>( i );
				this->compressed = this->compressed + x;
	    	}
	    	else{
	    		bitset<8> mybits1((dictionary[w] & 0xFF));
		    	bitset<8> mybits2(((dictionary[w] >> 8) & 0xFF));
	    		i = mybits1.to_ulong(); 
				x = static_cast<unsigned char>( i );
				this->compressed = this->compressed + x;
				i = mybits2.to_ulong(); 
				x = static_cast<unsigned char>( i );
				this->compressed = this->compressed + x;
	    	}

	    	// Add wc to the dictionary.
	    	dictionary[wc] = dictSize++;
	    	w = string(1, c);
	    }
	}

	if (!w.empty()){
	    bitset<8> mybits1((dictionary[w] & 0xFF));
    	bitset<8> mybits2(((dictionary[w] >> 8) & 0xFF));
		i = mybits1.to_ulong(); 
		x = static_cast<unsigned char>( i );
		this->compressed = this->compressed + x;
		i = mybits2.to_ulong(); 
		x = static_cast<unsigned char>( i );
		this->compressed = this->compressed + x;
	}

	return this->compressed;
}

Compressor::~Compressor() {
}