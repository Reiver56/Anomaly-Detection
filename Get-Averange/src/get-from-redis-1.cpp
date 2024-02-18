#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

int getInfoFromRedis(){
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE stream_size - +");
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    
    // converto la stringa in intero
    int window = atoi(reply->element[0]->element[1]->element[1]->str);
    freeReplyObject(reply);
    // chiudo la connessione
    redisFree(c);

    return window;
    
   

}
    