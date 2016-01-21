#ifndef OPCODE_H
#define	OPCODE_H

#include <string>

using namespace std;

enum InstructionCode {
    ACK,        // Acknowledged a transaction
    DEFAULT,    // Default Values are set for the client
    NEW_VALUE,  // New Value available followed by the value itself
    NOTHING,    // Nothing New to Send
    ERROR,      // ERROR followed by the error code
    READY,      // READY to receive a package
    CONTINUE,   // Continue with another action?
    CONFIRM,    // Confirm that the task was handled correctly. Followed by the code of the task
    AGAIN,      // Try performing the last action again. Followed by the code of the action/task
    TIME        // How long to wait before asking again? Followed by time interval
};

class OpCode {
    public:
        OpCode(string opcode);
        OpCode(string opcode, int operand);
        OpCode(string opcode, string label);
        OpCode(const OpCode& orig);
        virtual ~OpCode();
        InstructionCode getOpCode() const;
        int getOperand() const;
        string getOpCodeName() const;
        string getLabel() const;
        string toString() const;
        static int numArgs(string opcode);
        
    private:
        InstructionCode opcode;
        int operand;
        string label;
};

#endif	/* OPCODE_H */

