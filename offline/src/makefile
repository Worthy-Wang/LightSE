#版本三
CC=g++
CFLAGS=-g -Wall 
target=server
src=$(wildcard *.cpp) $(wildcard ./tinyxml2/*.cpp) 
obj=$(patsubst %.cpp, %.o, $(src))
link= # -lpthread  -L /usr/local/lib /usr/local/lib/libjsoncpp.a
$(target):$(obj)
	$(CC) $(CFLAGS)  $^ -o $@ $(link)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(link)
clean:
	rm -rf $(target) $(obj)
