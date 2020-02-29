#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <unordered_map>

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
    Components<Arithmetic> comp4 {{addition}};
    comp4.visit();
}