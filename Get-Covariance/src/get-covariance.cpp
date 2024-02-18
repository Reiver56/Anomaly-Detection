#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

double getCovariance(string streamName1, string streamName2, double avarange1, double avarange2){
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        if (c) {
            cout << "Errore di connessione: " << c->errstr << endl;
        } else {
            cout << "Can't allocate redis context" << endl;
        }
        exit(1);
    }
    redisReply* reply1 = (redisReply*)redisCommand(c, "XRANGE %s - +", streamName1.c_str());
    if (reply1 == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    redisReply* reply2 = (redisReply*)redisCommand(c, "XRANGE %s - +", streamName2.c_str());
    if (reply2 == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }

    double value1; double value2; double covariance; int j = 0;
    for (int i = 3; i < reply1->element[0]->element[1]->elements; i+=8){
        if ( reply1->element[0]->element[1]->element[i]->str != "NULL" && reply2->element[0]->element[1]->element[i]->str != "NULL"){
            value1 = atof(reply1->element[0]->element[1]->element[i]->str);
            value2 = atof(reply2->element[0]->element[1]->element[i]->str);
            covariance += (value1 - avarange1) * (value2 - avarange2);
        }
        else if (reply1->element[0]->element[1]->element[i]->str == "NULL" && reply2->element[0]->element[1]->element[i]->str != "NULL"){
            value2 = atof(reply2->element[0]->element[1]->element[i]->str);
            covariance += (0 - avarange1) * (value2 - avarange2);
        }
        else if (reply1->element[0]->element[1]->element[i]->str != "NULL" && reply2->element[0]->element[1]->element[i]->str == "NULL"){
            value1 = atof(reply1->element[0]->element[1]->element[i]->str);
            covariance += (value1 - avarange1) * (0 - avarange2);
        }
        else if (reply1->element[0]->element[1]->element[i]->str == "NULL" && reply2->element[0]->element[1]->element[i]->str == "NULL"){
            covariance += (0 - avarange1) * (0 - avarange2);
        }
        cout << covariance << endl;
        j += 1;
    }

    freeReplyObject(reply1);
    freeReplyObject(reply2);
    cout << covariance<< endl;
    cout << j << endl;
    cout << covariance/j << endl;
    return covariance/j;
}