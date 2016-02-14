/*==================================================
==================================================*/

#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>

#include "MessageStack.h"
#include "Message.h"
#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

MessageStack::MessageStack(int N){
	bottom_ = new Message[N];
	top_ = bottom_;
	size_ = N;
}

MessageStack::~MessageStack()
{
	 delete [] bottom_;
}

int MessageStack::num_items() const
{
		return (top_ - bottom_ );
}

void MessageStack::push(Message message)
{
		*top_ = message;
		top_++;
}

Message MessageStack::pop()
{
		top_--;
		return *top_;
}


int MessageStack::full() const
{
		return (num_items() >= size_);
}


int MessageStack::empty() const
{   
		return (num_items() <= 0);
}



void MessageStack::print() const
{

			cout << "Stack currently holds " << num_items() << " items: " ;        
			// for (InstructionCode *element=bottom_; element<top_; element++)
			// {
			//     cout << "  " << *element;
			// }
			cout << "\n";
		
}
		
