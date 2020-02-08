#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <any>

// TODO
/**
 * 
 * Create a way to output from each struct type
 * Implement the rest of the JsonDsl types just put them into lambda functions with (ENUM& )
 * 
*/

enum SetupTypes{
    Kind, SetupPrompt,
    KindInteger, KindString, KindBoolean,
    KindQuestionAnswer, KindMultipleChoice  
};

enum RuleTypes{
    Foreach, Loop, InParallel, Parallelfor,
    Switch, When, Extend, Reverse, Shuffle, 
    Sort, Deal, Discard, Add, Timer, 
    InputChoice, InputText, InputVote, 
    Message, GlobalMessage, Scores
};

enum Arithmetic {
    upFrom,
    downFrom,
    add
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
                [](double& value){value += value;},
                [](std::string& value){std::cout << "bool item" << std::endl;},
                [](bool& value){std::cout << "bool item" << std::endl;},
                [](RuleTypes value){
                    std::cout << "rules item" << " " << RuleTypes(value) << std::endl;
                },
                [](SetupTypes value){
                    std::cout << "Upload your json!" << std::endl;

                }
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


    template<typename E, typename A>
    struct arithm {
        arithm(const E& type): type(type) {} // change this to accept a pair for addition
        
        template<typename T>
        auto& operator()(const T& value){
            switch (type)
            {
                case upFrom:
                    value++;
                    break;
                case downFrom:
                    value--;
                    break;
            }
        }

        auto& operator()(const A& value){
            switch (type)
            {
                case add:
                    std::cout << value.first + value.second << std::endl;
                    return value;
                    break;
            }
        }

        E type;
    };

template<typename E, typename A> // switch this to accept an varidic args
struct Interpreter
{
    Interpreter(const E& type): type(type) {}

    template <typename T>
    auto operator()(T&& value){
        component.entities.emplace_back(value);
        component.visit(arithm<E, T>{type});
    }

    Components<A> component;
    E type;
};


int main()
{    
    // using var_t = std::variant<int, const char*>;
    // std::vector<var_t> vars = {1, 2, "Hello, World!"};

    // for (auto& v : vars) {
    //     std::visit(overloaded {  // (3)
    //         [](int i) { printf("%d\n", i); },
    //         [](const char* str) { puts(str); }
    //     }, v);
    // }

    // auto print_container = [](VariantContainer<int, double, std::string, bool, FragileItem, Setup>& value){
    //     value.visit(print_visitor{}); 
    //     std::cout << std::endl;
    // };


    auto printer = [](auto&& value){std::cout << value << " ";};

    // Components<int, double, std::string, bool, RuleTypes, SetupTypes> comp1;
    // comp1.entities.emplace_back(44);
    // comp1.entities.emplace_back(3.127);
    // comp1.entities.emplace_back("foo");
    // comp1.entities.emplace_back(false);
    // comp1.entities.emplace_back(KindQuestionAnswer);
    // comp1.entities.emplace_back(Scores);

    // comp1.visit(printer);
    // std::cout << std::endl;
    
    // comp1.visit();

    // comp1.visit();
    
    // comp1.visit(printer);
    // std::cout << std::endl;


    // Components<int> comp2;
    // comp2.entities.emplace_back(1);

    // comp2.visit(printer);
    // std::cout << std::endl;

    // comp2.visit(Interpreter<Arithmetic> {upFrom});

    // comp2.visit(printer);
    // std::cout << std::endl;

    // std::cout << std::endl;

    // Components<int> comp3;
    // comp3.entities.emplace_back(2);

    // comp3.visit(printer);
    // std::cout << std::endl;

    // comp3.visit(Interpreter<Arithmetic> {downFrom});

    // comp3.visit(printer);
    // std::cout << std::endl;
    

    using KindPair = std::pair<int, int>;
    Components<KindPair> comp4;
    KindPair b {1, 1};
    comp4.entities.emplace_back(b);

    comp4.visit(Interpreter<Arithmetic, KindPair> {add});
}