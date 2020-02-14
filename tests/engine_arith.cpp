#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <any>
#include <unordered_map>
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
    downFrom
};


template<class...>
struct type_list{};

template<class... TYPES>
struct visitor_base
{
    using types = type_list<TYPES...>;
};

struct print_visitor : visitor_base<int, double, std::string, bool, RuleTypes, SetupTypes>
{
    template<class T>
    void operator()(T& _in)
    {
        std::cout << _in << " ";
    }
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


template<typename E>
struct arithm
{
    arithm(const E& value): value(value) {}
    auto operator()(Arithmetic& type){
        switch (type){
            case upFrom:
                std::cout << "adding one" << std::endl;
                value += 1;
                break;
            case downFrom:
                std::cout << "minus one" << std::endl;
                value -= 1;
                break;
                
            default:
                break;
        }
    }

    E value;
};

template<typename E>
struct Interpreter
{
    Interpreter(const E& value){
        component.entities.emplace_back(value);
    }

    template <class T>
    auto operator()(T&& value){
        component.visit(arithm<T>{value});
    }

    Components<E> component;
};


template <typename G, typename... Args> 
struct tes3 {
    std::unordered_map<G, Args...> map;
};

struct Example{
    std::string name {"all the way inside"};
    std::map<std::string, std::string> map {{"name", name}};
};

struct Configuration{
    Example ex;

    std::string name{"config"};
    std::map<std::string, Example> map {
        {"example", ex}
    };
}config;

int main()
{    

    tes3<std::string, Configuration> w {{
        {"config", config}
    }};
    
    for(auto& n : w.map) {
        std::cout << "Key:[" << n.first << "] Value:[" << n.second.map["example"].map["name"] << "]\n";
    }
    // using var_t = std::variant<int, const char*>;
    // std::vector<var_t> vars = {1, 2, "Hello, World!"};

    // for (auto& v : vars) {
    //     std::visit(overloaded {  // (3)
    //         [](int i) { printf("%d\n", i); },
    //         [](const char* str) { puts(str); }
    //     }, v);
    // }

    auto printer = [](auto&& value){std::cout << value << " ";};
    // auto print_container = [](VariantContainer<int, double, std::string, bool, FragileItem, Setup>& value){
    //     value.visit(print_visitor{}); 
    //     std::cout << std::endl;
    // };

    Components<int, double, std::string, bool, RuleTypes, SetupTypes> comp1;
    comp1.entities.emplace_back(1);
    comp1.entities.emplace_back(2.2);
    comp1.entities.emplace_back("foo");
    comp1.entities.emplace_back(false);
    comp1.entities.emplace_back(KindQuestionAnswer);
    comp1.entities.emplace_back(Scores);


    Components<int> comp2;
    comp2.entities.emplace_back(1);

    comp2.visit(printer);
    std::cout << std::endl;

    comp2.visit(Interpreter<Arithmetic> {upFrom});

    comp2.visit(printer);
    std::cout << std::endl;

    std::cout << std::endl;

    Components<int> comp3;
    comp3.entities.emplace_back(2);

    comp3.visit(printer);
    std::cout << std::endl;

    comp3.visit(Interpreter<Arithmetic> {downFrom});

    comp3.visit(printer);
    std::cout << std::endl;


    // print them
    // game_components.visit(printer);
    // std::cout << std::endl;

    //
    // game_components.visit();

    // double them
    // comp1.visit(Interpreter{});
    
    // print again
    // comp1.visit(printer);
    // std::cout << std::endl;

}