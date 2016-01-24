#ifndef MESSAGE_H
#define	MESSAGE_H

#include <string>



using namespace std;

class Message {
public:
    Message(string,string);
    Message(string);

    int findLength();
    
    string buildMessage();
    int decodeMessage();
    
    virtual ~Message();

    int opcode;		// Opcode for the message
    int type;		// Datatype of the message
    int length;		// Length of the message
    char rawMessage;	// Raw Message representation

    string message;	// message as an array of bytes - contains opcode, type and length

};

#endif	/* MESSAGE_H */

