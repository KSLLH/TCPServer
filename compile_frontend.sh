#!/usr/bin/bash
g++ -std=c++11 -I /data/work/TCPServer ./frontend/frontend.cpp ./frontend/client.h ./frontend/client.cpp ./util/init_google.h ./util/init_google.cpp ./util/thread_pool.h -o ./frontend/frontend -lpthread -lgflags -lglog -DEBUG
