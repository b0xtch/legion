
CC := g++
CFLAGS := -std=c++17 -w
TARGET := engine_test json_convert_test rules_struct_test

ENGINE_CPP := engine_test.cpp ../tools/gameServer/src/engine.cpp
ENGINE_INCLUDE := ../tools/gameServer/include/ ../lib/nlohmann_json/include/ ../lib/json/include/

JSON_CPP := json_convert_test.cpp ../lib/json/src/jsonConvert.cpp
JSON_INCLUDE := ../lib/nlohmann_json/include/ ../lib/json/include/

RULE_STRUCT_CPP := rules_struct_test.cpp
RULE_INCLUDE := ../tools/gameServer/include/

# all dir for headers, might not work if there are duplicate header file names
# use if lazy 
ALL_INCLUDE := ../lib/json/include/ ../lib/networking/include/ ../lib/nlohmann_json/include/ ../lib/utils/include/ ../tools/gameClient/include ../tools/gameServer/include

# _CPP := ... path/to/cpp
# _INCLUDE := ... dir/of/each/header

# add test to target

all: engine json

engine:
# 	$(CC) $(CFLAGS) $(ENGINE_CPP) $(foreach h, $(ENGINE_INCLUDE), -I$h) -o $(word 1, $(TARGET)) 

json: 
	$(CC) $(CFLAGS) $(JSON_CPP) $(foreach h, $(ALL_INCLUDE), -I$h) -o $(word 2, $(TARGET))

rules:
	$(CC) $(CFLAGS) $(RULE_STRUCT_CPP) $(foreach h, $(RULE_INCLUDE), -I$h) -o $(word 3, $(TARGET))

clean:
	rm -f $(TARGET)
# 	rm *.gch
