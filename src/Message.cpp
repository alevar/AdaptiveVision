

/* Composing a statement

A statement is composed in the following manner:

OpCode/Information;

Where:

OpCode - one of the codes supported by the OpCode class
/ - delimiting that the opcode is done and the infomation to be received is there
Information - the image to be accepted by the server
; - the EOF signal

*/


#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stdlib.h>

#include "Message.h"

#include "MessageOpCode.h"

using namespace std;

Message::Message(string opcode) {

	int operand;
	operand = MessageOpCode::numArgs(opcode);

	this->message = opcode;

	for (int i = 0; i<operand;i++){
		message = message + "/1";
	}

	message = message + ";";

	// if(operand == 2){
	// 	message[1] = to_string(operand).c_str();
	// }
	
	cout << message << "		test message"<<endl;

    
}

string Message::buildMessage(){


    return message;
}

int Message::decodeMessage(){
	return 0;
}


Message::~Message() {
}
