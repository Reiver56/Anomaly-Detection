#!/bin/bash

make -C Test-Generator run &

make -C Get-Average run &

make -C Get-Covariance run &

make -C Get-Detection run 

# Aspetta che tutti i processi terminino (opzionale, a seconda della logica desiderata)
wait
