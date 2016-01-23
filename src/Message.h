#ifndef MESSAGE_H
#define	MESSAGE_H

#include <string>

#include "MessageOpCode.h"

using namespace std;

class Message {
public:
    Message(int);

    int findLength();
    
    char * buildMessage();
    int decodeMessage();
    
    virtual ~Message();

    int opcode;		// Opcode for the message
    int type;		// Datatype of the message
    int length;		// Length of the message
    char rawMessage;	// Raw Message representation

    char * message;	// message as an array of bytes - contains opcode, type and length

};

#endif	/* MESSAGE_H */

