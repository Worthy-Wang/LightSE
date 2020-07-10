#版本三
CC=g++
CFLAGS=-g -Wall
target=server
src=$(wildcard *.cpp) $(wildcard ./net/*.cpp) $(wildcard ./threadpool/*.cpp) $(wildcard ./cache/*.cpp) $(wildcard ./timerfd/*.cpp)
obj=$(patsubst %.cpp, %.o, $(src))
link=-lpthread  -L /usr/local/lib /usr/local/lib/libjsoncpp.a
$(target):$(obj)
	$(CC) $(CFLAGS)  $^ -o $@ $(link)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(link)
clean:
	rm -rf $(target) $(obj)
