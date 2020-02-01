
CC = g++
CFLAGS = -std=c++17
TARGET = test
CPP = json_convert_test.cpp jsonConvert.cpp
HEADERS = json.hpp jsonConvert.h

main: 
	$(CC) $(CFLAGS) -o $(TARGET) $(CPP) $(HEADERS)

clean:
	rm -f $(TARGET)
	rm *.gch
