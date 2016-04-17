/*==================================================
=================== MESSAGE OPCODE =================
====================================================
===== Class is meant to hold information about =====
===== all the operation and instruction codes  =====
===== that can be exchanged between the client =====
======= and the server. The class also holds =======
=== conversion and access tools for the OpCodes  ===
==================================================*/

#include <iostream>
#include <sstream>
#include <unordered_map>

#include "MessageOpCode.h"

using namespace std;

bool create_map(unordered_map<string, InstructionCode>& m) {
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

unordered_map<string, InstructionCode> OpCodeMap;
bool dummy = create_map(OpCodeMap);

bool create_arg_map(unordered_map<string, int>& m) {
    m["PUT"] = 2;
    m["GET"] = 2;
    m["ACK"] = 1;
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

unordered_map<string, int> ArgMap;
bool dummy2 = create_arg_map(ArgMap);

const char* opcodeNames[] = {
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

MessageOpCode::MessageOpCode(string opcode) {

    this->opcode = OpCodeMap[opcode];
    this->operand = -1;
    this->label = "";
}

MessageOpCode::MessageOpCode(string opcode, int operand) {

    this->opcode = OpCodeMap[opcode];
    this->operand = operand;
    this->label = "";
}

MessageOpCode::MessageOpCode(string opcode, string label) {

    this->opcode = OpCodeMap[opcode];
    this->operand = -1;
    this->label = label;
}

MessageOpCode::MessageOpCode(const MessageOpCode& orig) {

    this->opcode = orig.opcode;
    this->operand = orig.operand;
    this->label = orig.label;
}

MessageOpCode::~MessageOpCode() {
}

InstructionCode MessageOpCode::getOpCode() const {
    return this->opcode;
}

InstructionCode MessageOpCode::getOpCode(string opcode){

    return OpCodeMap[opcode];
}

int MessageOpCode::getOperand() const {
    return this->operand;
}

string MessageOpCode::getLabel() const {
    return this->label;
}

string MessageOpCode::getOpCodeName() const {
    return opcodeNames[opcode];
}

string MessageOpCode::toString() const {
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

int MessageOpCode::numArgs(string opcode) {
    return ArgMap[opcode];
}