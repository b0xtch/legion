#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>


typedef struct Arithmetic Arithmetic;
// typedef struct ForEach ForEach;
typedef struct Loop Loop;
typedef struct Add Add;

// typedef struct Components Components;

using Rule = std::variant<
Arithmetic,
// ForEach,
Add,
Loop>;

using RulesList = std::vector<Rule>;

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

// template <typename T> 
// struct ForEach{
//     ForEach(std::vector<T> list, T el, RulesList &r) : 
//         list{list},
//         element{el},
//         rules_to_run{r}
//         {};

//     std::vector<T> list;
//     T element;
//     RulesList rules_to_run;
// };




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

enum LoopType{
        UNTIL,
        WHILE
    };

struct Loop {
    Loop(bool& condition, LoopType type, RulesList &r) :
        condition{condition},
        type{type},
        rules_to_run{r} 
        {};

    bool condition;
    LoopType type;
    RulesList rules_to_run;

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
                            std::cout << "Until" <<std::endl;
                            do_condition = !rule.condition;
                            break;
                        case LoopType::WHILE:
                            std::cout << "While" <<std::endl;
                            do_condition = rule.condition;
                    }

                    while(do_condition){
                        for(auto r : rule.rules_to_run){
                            //do work
                            auto x = std::get<Add>(r);
                            std::cout << "a is " << x.to <<std::endl;
                            x.func();
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

int main() {    
    Arithmetic addition {{43, 5}, Arith::ADD};
    Components<Arithmetic> comp1 {{addition}};
    comp1.visit();
    
    int a = 0;
    bool end_condition = a > 10;

    Add add1 {a, 1};
    Add add2 {a, 2};
    Add add3 {a, 3};

    RulesList rules_list;
    rules_list.push_back(add1);
    rules_list.push_back(add2);
    rules_list.push_back(add3);

    Loop loop1 {end_condition, LoopType::UNTIL, rules_list};
    Components<Loop> comp2 {{loop1}};
    comp2.visit();

    std::cout << "a is " << a <<std::endl;


}