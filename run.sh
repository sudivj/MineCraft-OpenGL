#!bin/sh
clear
#echo "creating block data...";
#g++ -c src/world/block_data.cpp
echo "compiling...";
g++ main.cpp -lGL -lglfw -lpthread -lXrandr -lXi -ldl -I/usr/include/freetype2 include/glad.o -lfreetype -o main.bin
echo "running...";
./main.bin