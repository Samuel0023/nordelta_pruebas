#!/bin/bash
echo -e "\n\nHaciendo make de todos los modulos...\n\n"
cd ./Memoria
make clean
make
cd..
cd server
make clean
make
cd ..
cd swamp
make clean
make
cd ..
cd kernel
make clean
make
cd ..
cd matelib
make clean
make
echo -e "\n\nHechos los makes!\n\n"
