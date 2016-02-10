/*==================================================
==================================================*/

#include <iostream>
#include <sstream>
#include <vector>
#include <sstream>
#include <arpa/inet.h>
#include <stdlib.h>

#include "MessageQueue.h"
#include "Message.h"
#include "MessageOpCode.h"
#include "MessageType.h"

using namespace std;

MessageQueue::MessageQueue(int N){
	bottom_ = new Message[N];
	top_ = bottom_;
	size_ = N;
}

MessageQueue::~MessageQueue()
{
   delete [] bottom_;
}

int MessageQueue::num_items() const
{
    return (top_ - bottom_ );
}

void MessageQueue::push(Message message)
{
    *bottom_ = message;
    bottom_++;
}

Message MessageQueue::pop()
{
    bottom_--;
    return *bottom_;
}


int MessageQueue::full() const
{
    return (num_items() >= size_);
}


int MessageQueue::empty() const
{   
    return (num_items() <= 0);
}



void MessageQueue::print() const
{

      cout << "Stack currently holds " << num_items() << " items: " ;        
      // for (InstructionCode *element=bottom_; element<top_; element++)
      // {
      //     cout << "  " << *element;
      // }
      cout << "\n";
    
}
    
