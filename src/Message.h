/*==================================================
==================================================*/

#ifndef MESSAGE_H
#define	MESSAGE_H

#include <string>
#include <vector>

#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

class Message {
    public:
        Message(string,string);
        Message(string);

        int findLength();
        
        string buildMessage();
        int decodeMessage(string);
        MessageOpCode getOpCode();
        MessageType getType();
        string getInfo();
        string toString();
        
        virtual ~Message();

        int opcode;		// Opcode for the message
        int type;		// Datatype of the message
        int length;		// Length of the message
        char rawMessage;	// Raw Message representation

        string message;	// message as an array of bytes - contains opcode, type and length
    
    private:
        vector<string> split(const string, char, vector<string>);
        vector<string> split(const string, char);

};

#endif	/* MESSAGE_H */

