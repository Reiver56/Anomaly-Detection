#include <iostream>
#include <vector>
#include <string>
#include <hiredis/hiredis.h>

using namespace std;

inline void setStream(std::vector<std::vector<std::string>> dataWindow, std::string streamKey, int tempStart, int tempWindow, string target ,redisContext *c) {
    std::string command; int contatore = 0;
    //cout << "Inizio: " << tempStart << " Fine: " << tempWindow << endl; 
    for (int i = 0; i < dataWindow.size(); i++) {
        if (dataWindow[i][1] == target && stoi(dataWindow[i][0]) >= tempStart && stoi(dataWindow[i][0]) <= tempWindow){
        command += "XADD " + streamKey + " * timestamp " + dataWindow[i][0] + " value " + dataWindow[i][2] + " sensorID " + dataWindow[i][1];
        //cout << dataWindow[i][0] << " " << dataWindow[i][1] << " " << dataWindow[i][2] << endl;
        contatore++;
        }
    }
    //cout << "Contatore: " << contatore << endl;
    redisReply *reply = (redisReply *)redisCommand(c, command.c_str());
    if (reply == NULL) {
        std::cout << "Errore nell'esecuzione del comando" << std::endl;
        exit(1);
    }
    freeReplyObject(reply);
}

inline void isReady(redisContext *c, std::string command){
    //setta la stream isReady a command
    std::string streamKey = "isReady";
    // con comando SET
    std::string comma = "SET " + streamKey + " " + command;
    redisReply *reply = (redisReply *)redisCommand(c, comma.c_str());
    if (reply == NULL) {
        std::cout << "Errore nell'esecuzione del comando" << std::endl;
        exit(1);
    }
}

inline void isReady2(redisContext *c, std::string command){
    //setta la stream isReady a command
    std::string streamKey = "isReady2";
    // con comando SET
    std::string comma = "SET " + streamKey + " " + command;
    redisReply *reply = (redisReply *)redisCommand(c, comma.c_str());
    if (reply == NULL) {
        std::cout << "Errore nell'esecuzione del comando" << std::endl;
        exit(1);
    }
}

