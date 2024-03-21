#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

inline vector<string> getStreamFromRedis(redisContext *c){
    vector<string> streams;
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE stream_names - +");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    for (int i = 0; i < reply->elements; i++){
        streams.push_back(reply->element[i]->element[1]->element[1]->str);
    }
    freeReplyObject(reply);
    return streams;
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

inline std::string isReady2(redisContext *c){
    redisReply* reply = (redisReply*)redisCommand(c, "GET isReady2");
    if (reply == NULL) {
        std::cout << "Errore nell'esecuzione del comando" << std::endl;
        exit(1);
    }
    std::string command = reply->str;
    freeReplyObject(reply);
    return command;

}

inline void setIsReady2(redisContext *c){
    string streamKey = "isReady2";
    string command = "no";
    string comma = "SET " + streamKey + " " + command;
    redisReply *reply = (redisReply *)redisCommand(c, comma.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    freeReplyObject(reply);
}

inline vector<int> getSensor(redisContext *c){
    vector<int> sensors;
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE sensor_names - +");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    for (int i = 0; i < reply->elements; i++){
        string sensor = reply->element[i]->element[1]->element[1]->str;
        sensors.push_back(stoi(sensor));
    }
    freeReplyObject(reply);
    return sensors;
}