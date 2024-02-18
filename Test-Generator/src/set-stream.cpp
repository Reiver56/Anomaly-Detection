#include <iostream>
#include <vector>
#include <string>
#include <cstdlib> // per atoi
#include <hiredis/hiredis.h>

using namespace std;


void setStream(vector<vector<string>> dataWindow, string streamKey, int tempStart, int tempWindow, redisContext *c) {
    string command;
    for (int i = tempStart; i < tempWindow; i++){
        command += "XADD " + streamKey + " * timestamp " + dataWindow[i][0] + " value " + dataWindow[i][2] + " sensorID " +dataWindow[i][1];
    }
    redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    freeReplyObject(reply);
}
   