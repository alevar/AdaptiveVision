/*==================================================
==================== MESSAGE STACK =================
====================================================
===== Class is meant to hold information about =====
====== the instructions to be processed after ======
==== the execution of the currently working one ====
==================================================*/

#ifndef MESSAGESTACK_H
#define	MESSAGESTACK_H

#include <string>
#include <vector>

#include "Message.h"
#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

class MessageStack {

    public:
        MessageStack(int size);
        void push(Message message);
	    int num_items() const;
	    Message pop();
	    int full() const;
	    int empty() const;
	    void print() const;
	    ~MessageStack();
    
    private:

	    Message *bottom_;
	    Message *top_;
	    int size_;

};

#endif	/* MESSAGESTACK_H */

