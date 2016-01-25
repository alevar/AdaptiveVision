/*==================================================
==================================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>

#include "Message.h"

#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

Message::Message(){

}

Message::Message(string opcode) {

	this->opcode = opcode;

	this->outMessage = opcode;

	if(MessageOpCode::numArgs(opcode) == 1){
		outMessage = outMessage + "/1";
	}

	outMessage = outMessage + "/;";
    
}

Message::Message(string opcode, string message) {

	this->opcode = opcode;

	MessageOpCode dummy(opcode);
    MessageOpCode *inst;
    inst = &dummy;
    this->instOpcode = inst->MessageOpCode::getOpCode();
    cout<<this->instOpcode<<endl;

	this->length = htonl(message.size());
	this->outMessage = opcode;

	if(MessageOpCode::numArgs(opcode) == 1){
		outMessage = outMessage + "/1";
	}

	if(MessageOpCode::numArgs(opcode) == 2){
		outMessage = outMessage + "/VAL/"+ to_string(message.size()) + "/" + message;
	}

	outMessage = outMessage + "/;";
	this->tokens = split(outMessage,'/');
    
}

string Message::buildMessage(){

    return outMessage;

}

void Message::decodeMessage(string inMessage){

	this->tokens = split(inMessage,'/');

}

InstructionCode Message::getOpCode(){
	return this->instOpcode;
}

MessageType Message::getType(){

	return MessageType(tokens[1]);
}

string Message::getInfo(){
	return this->tokens[2];
}

uint32_t Message::getLength(){
	return this->length;
}

string Message::toString(){
	return this->outMessage;
}

vector<string> &Message::split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> Message::split(const string s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}


Message::~Message() {
}
