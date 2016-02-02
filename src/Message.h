/*==================================================
==================== MESSAGE CLASS =================
====================================================
==== Class is meant to hold the current message ====
==== as well as all the information associated  ====
===================== with it. =====================
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
        Message();
        
        string buildMessage();
        void decodeMessage(string);

        InstructionCode getOpCode();
        MessageType getType();
        uint32_t getLength();
        string getInfo();

        string toString();
        void toMessage(string);
        
        virtual ~Message();

        string opcode;		        // Opcode for the message
        InstructionCode instOpcode;
        int type;		             // Datatype of the message
        uint32_t length;		      // Length of the message

        vector<string> tokens;

        string outMessage;	             // message as an array of bytes - contains opcode, type and length
    
    private:

        vector<string> &split(const string&, char, vector<string>&);
        vector<string> split(const string, char);

};

#endif	/* MESSAGE_H */

