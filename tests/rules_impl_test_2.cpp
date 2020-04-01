#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>
#include "RuleCollection_TEST.h"


enum Arith {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE
};

struct Arithmetic {
    using KindPair = std::pair<int, int>;
    
    Arithmetic(KindPair values, Arith operation)
    : values(values),
      operation(operation)
      {}

    KindPair values;
    Arith operation;
    int64_t result;
};


typedef int& destination;

struct Add {
    Add(destination to, int value) :
            to{to},
            value{value}
            {};

    void func(){
        std::cout << "Adding ... " << value << " to " << to << std::endl;
        to += value;
    }

    destination to;
    int value;
};


enum ConditionType{
    EQUAL,
    GREATER,
    LESS,
    AND,
    OR
};

template <typename T>
struct Condition {
    Condition(T &v1, T &v2, ConditionType t) :
        value1{v1},
        value2{v2},
        type{t}
        {};

    T value1;
    T value2;
    ConditionType type;

    bool operator()() const{
        switch(type)
        {
            case ConditionType::EQUAL:
                return (value1 == value2);
            case ConditionType::GREATER:
                return (value1 > value2);
            case ConditionType::LESS:
                return (value1 < value2);
            case ConditionType::AND:
                return (value1 && value2);
            case ConditionType::OR:
                return (value1 || value2);
        }
    }
};

enum LoopType{
        UNTIL,
        WHILE
};

template <typename T>
struct Loop<T> {
    Loop(Condition<T> c, LoopType type) :
        loopCondition{c},
        type{type}
        {};

    Condition<T> loopCondition;
    LoopType type;
}; 

struct ControlStructures {
    // ForEach forEach;
    Loop loop;
    // Inparallel inparallel;
    // Parallelfor parallelfor;
    // Switch switch;
    // When when;
};


template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>; 

template<typename... T>
struct Components{
    using component = std::variant<T...>;

    void visit(){
        for (auto& entity : entities){
            std::visit(overloaded {
                [](int& value){value += value;},
                // [](ControlStructures rule){},
                // [](ListOperations rule){},
                [](Loop rule){
                    std::cout << "Looping..." <<std::endl;
                    bool do_condition;
                    switch(rule.type)
                    {
                        case LoopType::UNTIL:
                            while(!rule.loopCondition()){
                                //do work
                                // how to pass in a std::vector<Components> = RuleList
                            }
                            break;

                        case LoopType::WHILE:
                           while(rule.loopCondition()){
                                //do work
                                // how to pass in a std::vector<Components> = RuleList
                           }
                    }

                },
                [](Arithmetic rule){
                    switch (rule.operation)
                    {
                    case ADD:
                        rule.result = rule.values.first + rule.values.second;
                        std::cout << rule.result << std::endl;
                        break;
                    case SUBTRACT:
                        rule.result = rule.values.first - rule.values.second;
                        std::cout << rule.result << std::endl;
                        break;
                    case MULTIPLY:
                        rule.result = rule.values.first * rule.values.second;
                        std::cout << rule.result << std::endl;
                        break;
                    case DIVIDE:
                        rule.result = rule.values.first / rule.values.second;
                        std::cout << rule.result << std::endl;
                        break;
                    default:
                        break;
                    }
                },
                // [](Timing rule){},
                // [](HumanInput rule){},
                // [](Output rule){}
            }, entity);
        }
    }
    
    template <typename V>
    void visit(V&& visitor){
        for (auto& entity : entities){
            std::visit(visitor, entity);
        }
    }

    std::vector<component> entities;
};

using RulesList = std::vector<Components>
struct Rule{
    Rule(Components &c, RulesList& r) :
        controlStruct (c),
        rulesToRun (r)
        {};

    Components controlStruct;
    RulesList rulesToRun;

    void func(){
        c.visit();
    }
}

int main() {    
    Arithmetic addition {{43, 5}, Arith::ADD};
    Components<Arithmetic> comp1 {{addition}};
    comp1.visit();
    
    int a = 0;
    int b = 10;

    Condition<int> c1 {a, b, ConditionType::LESS};

    // below is irrelevant at this point

    Add add1 {a, 1};
    Add add2 {a, 2};
    Add add3 {a, 3};

    RulesList rules_list;
    rules_list.push_back(add1);
    rules_list.push_back(add2);
    rules_list.push_back(add3);

    Loop loop1 {c1, LoopType::UNTIL, rules_list};
    Components<Loop> comp2 {{loop1}};
    comp2.visit();

    std::cout << "a is " << a <<std::endl;

    // above is ireleveant at this point


}