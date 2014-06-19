#!/system/bin/bash
cd Server/
gcc -o server Server.c database.c Parser.c AccessControl.c
cd ..
cd Client/
gcc -o client Client.c Help.c
cd ..

xterm -e "cd ~/workspace/Projekt1/Server
./server" &
sleep 1
xterm -e "cd ~/workspace/Projekt1/Client
./client"
