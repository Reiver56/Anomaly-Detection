#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

double getAvarange(string streamName, int sizeVector){
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE %s - +", streamName.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    vector<double> values;
    for (int i = 3; i < reply->element[0]->element[1]->elements; i+=8){
        if (reply->element[0]->element[1]->element[i]->str != "NULL"){
            values.push_back(atof(reply->element[0]->element[1]->element[i]->str));
        }
        
    }
    
    freeReplyObject(reply);
    double sum = 0;
    for (int i = 0; i < values.size(); i++){
        sum += values[i];
    }
    return sum/values.size();

    
}