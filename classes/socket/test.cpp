#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <netinet/in.h>

#include <map>
#include <vector>

#define MAXHOSTNAME 256
using namespace std;



int main(int argc , char *argv[])
{
    map<string,int > sampleAnswer;
    vector<int> test = {1,2};

    sampleAnswer.insert ( pair<string,int >("127.0.0.1",2));

    cout<<sampleAnswer.count("127.0.0.1")<<endl;

    return 0;
}