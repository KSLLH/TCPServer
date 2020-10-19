#!/usr/bin/bash
g++ -std=c++11 -I /data/work/TCPServer ./backend/backend.cpp ./backend/control.h ./backend/control.cpp ./util/init_google.h ./util/init_google.cpp ./backend/service_manager.h ./backend/service_manager.cpp ./backend/server.h ./backend/server.cpp ./util/thread_pool.h -o ./backend/backend -lgflags -lglog -lpthread -DEBUG
