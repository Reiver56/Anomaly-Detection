#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

inline int getInfoFromRedis(redisContext *c){
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE stream_window - +");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    
    // converto la stringa in intero
    int window = atoi(reply->element[0]->element[1]->element[1]->str);
    freeReplyObject(reply);
    // chiudo la connessione
   

    return window;
}

inline vector<int> getTimes(redisContext *c){
    vector<int> times;
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE time_window - +");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    times.push_back(stoi(reply->element[0]->element[1]->element[1]->str));
    times.push_back(stoi(reply->element[0]->element[1]->element[3]->str));
    freeReplyObject(reply);
    return times;
}

inline string isReady_(redisContext *c){
    // questa funzione fa una get sulla stream isReady
    redisReply* reply = (redisReply*)redisCommand(c, "GET isReady");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    string command = reply->str;
    freeReplyObject(reply);
    return command;
}

inline void isReady(redisContext *c){
    //setta la stream isReady a command
    std::string streamKey = "isReady";
    std::string command = "no";
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


    