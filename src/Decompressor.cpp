/*==================================================
==================================================*/

#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <iterator>
#include <vector>

#include "Decompressor.h"

using namespace std;

Decompressor::Decompressor(){
	this->dictSize = 256;
}

Decompressor::Decompressor(string *input) {
	toComp = *input;
	this->dictSize = 256;
}

string Decompressor::unCompress(){

	for (int i = 0; i < 256; i++)
		dictionary[i] = string(1, i);
 
	string w(1,toComp[0]);
	result = w;
	for (int i=2 ;i<=toComp.length(); i++) {

		if(int(toComp[i+1]) == 0){
			int k = int(toComp[i]);
			if (dictionary.count(k)){
				i++;
				entry = dictionary[k];
			}
			else if (k == dictSize){
				entry = w + w[0];
			}
			else{
				throw "Bad compressed k";
			}
		}
		else{
			int k = int(toComp[i])+256;
			if (dictionary.count(k)){
				i++;
				entry = dictionary[k];
			}
			else if (k == dictSize){
				entry = w + w[0];
			}
			else{
				throw "Bad compressed k";
			}
		}    
 
		result += entry;
 
		dictionary[dictSize++] = w + entry[0];
 
		w = entry;
	}
	return result;
}

Decompressor::~Decompressor() {
}