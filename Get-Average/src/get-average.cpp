#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <hiredis/hiredis.h>

using namespace std;

inline tuple<double,int> getAvarage(string streamName, int sizeVector, redisContext *c){
    tuple<double, int> result;
    redisReply* reply = (redisReply*)redisCommand(c, "XRANGE %s - +", streamName.c_str());
    if (reply == NULL) {
        cout << "Errore nell'esecuzione del comando" << endl;
        exit(1);
    }
    vector<double> values; int length = 0; string value = "";
    // perché ho messo length = values.size()? perché non voglio considerare i valori nulli nella media, perciò decremento il valore usato per la media 
    for (int i = 3; i < reply->element[0]->element[1]->elements; i+=8){
        value = reply->element[0]->element[1]->element[i]->str;
        if (value != "NULL"){
            values.push_back(atof(reply->element[0]->element[1]->element[i]->str));
            length++;
        }
        else{
            //cout << "trovato un NULL" << endl;
            values.push_back(0);
        }
        
    }
    
    freeReplyObject(reply);
    double sum = 0;
    for (int i = 0; i < values.size(); i++){
        sum += values[i];
    }
    if (length == 0){
        result = make_tuple(0,0);
        //cout << "media: " << get<0>(result) << " lunghezza: " << get<1>(result) << endl;
        return result;
    }
    else{result = make_tuple(sum/length, length);}
    
    //cout << "media: " << get<0>(result) << " lunghezza: " << get<1>(result) << endl;
    return result;
    // qua teoricamente se trovo una finestra che ha solo NULL dovrei segnalarlo mediante un booleano

    
}