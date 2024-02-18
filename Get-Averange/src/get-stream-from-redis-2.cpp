#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

vector<string> getStreamFromRedis(){
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
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