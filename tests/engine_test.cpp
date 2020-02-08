#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <any>

/*
 * I realize this is not a test but ill get to converting to a test
 * I thought it would be helpful to see how the interpreter is working
 * */

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


int main(){    
    
    auto printer = [](auto&& value){std::cout << value << " ";};

    Components<int, double, std::string, bool, RuleTypes, SetupTypes> comp1;
    comp1.entities.emplace_back(44);
    comp1.entities.emplace_back(3.127);
    comp1.entities.emplace_back("foo");
    comp1.entities.emplace_back(false);
    comp1.entities.emplace_back(KindQuestionAnswer);
    comp1.entities.emplace_back(Scores);

    comp1.visit(printer);
    std::cout << std::endl;

    
    comp1.visit();

    comp1.visit();
    
    comp1.visit(printer);
    std::cout << std::endl;


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

}