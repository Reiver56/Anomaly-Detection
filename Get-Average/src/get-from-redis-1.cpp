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
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE stream_size - +");
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
    