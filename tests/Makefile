
CC := g++
CFLAGS := -std=c++17 -w
TARGET := engine_arith engine_test json_convert_test rules_struct_test rules_impl rules_impl2 timer_test

ENGINE_CPP := engine_test.cpp ../tools/gameServer/src/engine.cpp
ENGINE_INCLUDE := ../tools/gameServer/include/ ../lib/nlohmann_json/include/ ../lib/json/include/

JSON_CPP := json_convert_test.cpp ../lib/json/src/jsonConvert.cpp
JSON_INCLUDE := ../lib/nlohmann_json/include/ ../lib/json/include/

RULE_STRUCT_CPP := rules_struct_test.cpp
RULE_INCLUDE := ../tools/gameServer/include/

ENGINE_ARITH_CPP := engine_arith.cpp

RULES_IMPL_CPP := rules_impl_test.cpp ../tools/gameServer/src/player.cpp

RULES_IMPL_2_CPP := rules_impl_test_2.cpp

TIMER_TEST_CPP := timer_test.cpp

# all dir for headers, might not work if there are duplicate header file names
# use if lazy 
ALL_INCLUDE := ../lib/json/include/ ../lib/networking/include/ ../lib/nlohmann_json/include/ ../lib/utils/include/ ../tools/gameClient/include ../tools/gameServer/include

# _CPP := ... path/to/cpp
# _INCLUDE := ... dir/of/each/header

# add test to target

# all: engine json

rules_impl:
	$(CC) $(CFLAGS) $(RULES_IMPL_CPP) $(foreach h, $(RULE_INCLUDE), -I$h) -o $(word 5, $(TARGET))

rules_impl2:
	$(CC) $(CFLAGS) $(RULES_IMPL_2_CPP) -o $(word 6, $(TARGET))

timer_test:
	$(CC) $(CFLAGS) $(TIMER_TEST_CPP) -o $(word 6, $(TARGET))

# engine_arith:
# 	$(CC) $(CFLAGS) $(ENGINE_ARITH_CPP) -o $(word 1, $(TARGET)) 

# engine:
# 	$(CC) $(CFLAGS) $(ENGINE_CPP) $(foreach h, $(ENGINE_INCLUDE), -I$h) -o $(word 2, $(TARGET)) 

# json: 
# 	$(CC) $(CFLAGS) $(JSON_CPP) $(foreach h, $(ALL_INCLUDE), -I$h) -o $(word 3, $(TARGET))

# rules:
# 	$(CC) $(CFLAGS) $(RULE_STRUCT_CPP) $(foreach h, $(RULE_INCLUDE), -I$h) -o $(word 4, $(TARGET))

clean:
	rm -f $(TARGET)
# 	rm *.gch
