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

bool createMap(unordered_map<string, InstructionCode>& m) {
    m["PUT"] = PUT;
    m["GET"] = GET;
    m["ACK"] = ACK;
    m["DEFAULT"] = DEFAULT;
    m["NEW_VALUE"] = NEW_VALUE;
    m["NOTHING"] = NOTHING;
    m["ERROR"] = ERROR;
    m["READY"] = READY;
    m["CONTINUE"] = CONTINUE;
    m["CONFIRM"] = CONFIRM;
    m["AGAIN"] = AGAIN;
    m["LEN"] = LEN;
    m["TIME"] = TIME;
    return true;
}

unordered_map<string, InstructionCode> messageOpCodeMap;
bool messageDummy = createMap(messageOpCodeMap);

bool createArgMap(unordered_map<string, int>& m) {
    m["PUT"] = 2;
    m["GET"] = 2;
    m["ACK"] = 0;
    m["DEFAULT"] = 0;
    m["NEW_VALUE"] = 2;
    m["NOTHING"] = 0;
    m["ERROR"] = 1;
    m["READY"] = 0;
    m["CONTINUE"] = 0;
    m["CONFIRM"] = 1;
    m["AGAIN"] = 1;
    m["LEN"] = 1;
    m["TIME"] = 1;
    return true;
}

unordered_map<string, int> messageArgMap;
bool messageDummy2 = createArgMap(messageArgMap);

const char* messageOpCodeNames[] = {
    "PUT",
    "GET",
    "ACK",        // Acknowledged a transaction
    "DEFAULT",    // Default Values are set for the client
    "NEW_VALUE",  // New Value available followed by the value itself
    "NOTHING",    // Nothing New to Send
    "ERROR",      // ERROR followed by the error code
    "READY",      // READY to receive a package
    "CONTINUE",   // Continue with another action?
    "CONFIRM",    // Confirm that the task was handled correctly. Followed by the code of the task
    "AGAIN",      // Try performing the last action again. Followed by the code of the action/task
    "LEN",
    "TIME"
};


Message::Message(){

}

Message::Message(string opcode) {

	this->opcode = opcode;

	MessageOpCode dummy(opcode);
    MessageOpCode *inst;
    inst = &dummy;
    this->instOpcode = inst->MessageOpCode::getOpCode();

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

	this->length = htonl(message.size());
	this->outMessage = opcode;

	if(MessageOpCode::numArgs(opcode) == 1){
		outMessage = outMessage + "/" + message;
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
	return this->tokens[tokens.size()-2];
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

void Message::toMessage(string stringData){
	this->tokens = split(stringData,'/');

	MessageOpCode dummy(tokens[0]);
	MessageOpCode *inst;
    inst = &dummy;

	this->instOpcode = inst->MessageOpCode::getOpCode();

	this->opcode = tokens[0];

	this->outMessage = tokens[0];

	if(MessageOpCode::numArgs(tokens[0]) == 1){
		outMessage = outMessage + "/1";
	}

	if(MessageOpCode::numArgs(tokens[0]) == 2){
		outMessage = outMessage + "/VAL/"+ to_string(tokens.back().size()) + "/" + tokens.back();
	}
}

Message::~Message() {
}
