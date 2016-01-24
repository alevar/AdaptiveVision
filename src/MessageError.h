/*==================================================
==================================================*/

#ifndef MESSAGEERROR_H
#define	MESSAGEERROR_H

#include <string>

using namespace std;

enum ErrorCode {
    LOS,
    INT
};

class MessageError {
    public:
        MessageError(string);
        
    private:
        ErrorCode errorcode;
};

#endif	/* MESSAGEERROR_H */

