#!bin/sh
echo "compiling...";
g++ main.cpp -lGL -lglfw -lpthread -lXrandr -lXi -ldl -I/usr/include/freetype2 include/glad.o -lfreetype -o main.bin
echo "running...";
gdb ./main.bin