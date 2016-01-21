#ifndef MESSAGE_H
#define	MESSAGE_H

#include <string>

#include "OpCode.h"

using namespace std;

class Message {
public:
    Message(string opcode);
    
    byte buildMessage();
    
    virtual ~Message();

    int opcode;
    int length;
    
private:

};

#endif	/* MESSAGE_H */

