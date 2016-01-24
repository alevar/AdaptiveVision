/*==================================================
==================================================*/

#ifndef MESSAGETYPE_H
#define	MESSAGETYPE_H

#include <string>

using namespace std;

enum TypeCode {
    IMG,
    HSV
};

class MessageType {
    public:
        MessageType(string);
        
    private:
        TypeCode type;
};

#endif	/* MESSAGETYPE_H */

