#!/system/bin/bash

cd Server/
gcc -o server Server.c database.c Parser.c AccessControl.c
osascript <<END 
tell app "Terminal" to do script "cd Documents/workspace/Projekt1/Server/
./server" 
END
cd ..

sleep 3s

cd Client/
gcc -o client Client.c Help.c
osascript <<END 
tell app "Terminal" to do script "
cd Documents/workspace/Projekt1/Client/
./client" 
END
cd ..


