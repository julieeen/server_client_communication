#!/system/bin/bash

cd Server/
gcc -o server Server.c database.c Parser.c AccessControl.c
cd ..

cd Client/
gcc -o client Client.c Help.c
cd ..
