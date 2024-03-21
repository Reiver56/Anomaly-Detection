#!/bin/bash

redis-server &

#voglio fargli fare ctrl+c per eseguire il comando successivo senza dover chiudere redis
sleep 1

#avvio il server di postgres
sudo service postgresql start

make -C Database-set run



