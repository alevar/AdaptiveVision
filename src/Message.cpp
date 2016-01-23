#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdlib.h>

#include "Message.h"

using namespace std;

Message::Message(int opcode) {
	
	this->opcode = opcode;

    
}

char * Message::buildMessage(){

	message = (char*) malloc(8);

	message[0] = 35;
	message[1] = 46;

	message[2]='\0';

    return message;
}

int Message::decodeMessage(){
	return 0;
}


Message::~Message() {
}
