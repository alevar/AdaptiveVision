/*==================================================
==================================================*/


#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <stdlib.h>

#include "Message.h"

#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

Message::Message(string opcode) {

	this->message = opcode;

	if(MessageOpCode::numArgs(opcode) == 1){
		message = message + "/1";
	}
	string text = "120";
	if(MessageOpCode::numArgs(opcode) == 2){
		message = message + "/VAL"+"/" + text;
	}

	message = message + ";";
    
}

string Message::buildMessage(){


    return message;
}

int Message::decodeMessage(string inMessage){

	return 0;
}

MessageOpCode Message::getOpCode(){

	return MessageOpCode("ACK");
}

MessageType Message::getType(){

	return MessageType("ACK");
}

string Message::getInfo(){

	return 0;
}

string Message::toString(){

}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


Message::~Message() {
}
