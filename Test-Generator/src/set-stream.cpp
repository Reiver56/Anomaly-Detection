#include <iostream>
#include <vector>
#include <string>
#include <hiredis/hiredis.h>

using namespace std;

inline void setStream(std::vector<std::vector<std::string>> dataWindow, std::string streamKey, int tempStart, int tempWindow, redisContext *c) {
    std::string command;
    for (int i = tempStart; i < tempWindow; i++) {
        command += "XADD " + streamKey + " * timestamp " + dataWindow[i][0] + " value " + dataWindow[i][2] + " sensorID " + dataWindow[i][1];
    }
    redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
    if (reply == NULL) {
        std::cout << "Errore nell'esecuzione del comando" << std::endl;
        exit(1);
    }
    freeReplyObject(reply);
}