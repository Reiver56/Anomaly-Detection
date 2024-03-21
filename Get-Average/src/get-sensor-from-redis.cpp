#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

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