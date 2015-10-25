#!/bin/bash

echo "Qual o ingrediente secreto?"
read -s SSHPASS

for computer in labcel1 labcel2 labcel3 labcel6 ada
do
    sshpass -p $SSHPASS rsync -azulvI boids4 $computer:boids4/
done
