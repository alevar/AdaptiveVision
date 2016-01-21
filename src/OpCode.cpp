#include <iostream>
#include <sstream>
#include <unordered_map>

#include "OpCode.h"

using namespace std;

bool create_map(unordered_map<string, InstructionCode>& m) {
    m["ACK"] = ACK;
    m["DEFAULT"] = DEFAULT;
    m["NEW_VALUE"] = NEW_VALUE;
    m["NOTHING"] = NOTHING;
    m["ERROR"] = ERROR;
    m["READY"] = READY;
    m["CONTINUE"] = CONTINUE;
    m["CONFIRM"] = CONFIRM;
    m["AGAIN"] = AGAIN;
    m["TIME"] = TIME;
    return true;
}

unordered_map<string, InstructionCode> OpCodeMap;
bool dummy = create_map(OpCodeMap);

bool create_arg_map(unordered_map<string, int>& m) {
    m["ACK"] = 0;
    m["DEFAULT"] = 0;
    m["NEW_VALUE"] = 1;
    m["NOTHING"] = 0;
    m["ERROR"] = 1;
    m["READY"] = 0;
    m["CONTINUE"] = 0;
    m["CONFIRM"] = 1;
    m["AGAIN"] = 1;
    m["TIME"] = 1;
    return true;
}

unordered_map<string, int> ArgMap;
bool dummy2 = create_arg_map(ArgMap);

const char* opcodeNames[] = {
    "ACK",        // Acknowledged a transaction
    "DEFAULT",    // Default Values are set for the client
    "NEW_VALUE",  // New Value available followed by the value itself
    "NOTHING",    // Nothing New to Send
    "ERROR",      // ERROR followed by the error code
    "READY",      // READY to receive a package
    "CONTINUE",   // Continue with another action?
    "CONFIRM",    // Confirm that the task was handled correctly. Followed by the code of the task
    "AGAIN",      // Try performing the last action again. Followed by the code of the action/task
    "TIME"
};

OpCode::OpCode(string opcode) {

    this->opcode = OpCodeMap[opcode];
    this->operand = -1;
    this->label = "";
}

OpCode::OpCode(string opcode, int operand) {

    this->opcode = OpCodeMap[opcode];
    this->operand = operand;
    this->label = "";
}

OpCode::OpCode(string opcode, string label) {

    this->opcode = OpCodeMap[opcode];
    this->operand = -1;
    this->label = label;
}

OpCode::OpCode(const OpCode& orig) {

    this->opcode = orig.opcode;
    this->operand = orig.operand;
    this->label = orig.label;
}

OpCode::~OpCode() {
}

InstructionCode OpCode::getOpCode() const {
    return this->opcode;
}

int OpCode::getOperand() const {
    return this->operand;
}

string OpCode::getLabel() const {
    return this->label;
}

string OpCode::getOpCodeName() const {
    return opcodeNames[opcode];
}

string OpCode::toString() const {
    string result = "          ";
    string opName = getOpCodeName();
    result += opName;

    if (numArgs(opName) > 0) {
        stringstream ss;
        ss << operand;
        string operandStr = ss.str();
        for (int i = 0; i < 32 - opName.size() - operandStr.size(); i++)
            result += " ";
        result += operandStr;
    }
    return result;
}

int OpCode::numArgs(string opcode) {
    return ArgMap[opcode];
}