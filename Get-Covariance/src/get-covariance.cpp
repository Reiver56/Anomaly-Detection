#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

inline tuple<double, int> getCovariance(string streamName1, string streamName2, double avarange1, double avarange2, redisContext *c){

    
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

    double value1=0; double value2=0; double covariance = 0; int j = 0;
    for (int i = 3; i < reply1->element[0]->element[1]->elements; i+=8){
        
        string stringa1 = reply1->element[0]->element[1]->element[i]->str;
        string stringa2 = reply2->element[0]->element[1]->element[i]->str;

        if ( stringa1 != "NULL" && stringa2 != "NULL"){
            value1 = atof(reply1->element[0]->element[1]->element[i]->str);
            value2 = atof(reply2->element[0]->element[1]->element[i]->str);
            covariance += (value1 - avarange1) * (value2 - avarange2);
            ++j;
        }
        else if (stringa1 == "NULL" && stringa2 != "NULL"){
            value2 = atof(reply2->element[0]->element[1]->element[i]->str);
            covariance += (value2 - avarange2);
            ++j;
        }
        else if (stringa1 != "NULL" && stringa2 == "NULL"){
            value1 = atof(reply1->element[0]->element[1]->element[i]->str);
            covariance += (value1 - avarange1);
            ++j;
        }
        else if (stringa1 == "NULL" && stringa2 == "NULL"){
            covariance += 0;
        }
        //cout << covariance << endl;
        
    }

    freeReplyObject(reply1);
    freeReplyObject(reply2);
    //cout << covariance<< endl;
    //cout << j << endl;
    //cout << covariance/j << endl;
    if (j == 0){
        return make_tuple(0,1);
    }
    else{
        return make_tuple(covariance/j, 0);
    }
    
}