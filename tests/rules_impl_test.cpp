#include <algorithm>
#include <string>
#include <iostream>

#include <RuleCollection_TEST.h>


int main()
{
    int b = 1;

    RuleCollection::Arithmetic addobj1(b, 1, RuleCollection::MathOperation::ADD);

    std::cout << "b is: " << b << std::endl; 
    addobj1.func();
    std::cout << "b is: " << b << std::endl; 

    std::cout << "\n\n=============== Testing Loop (Until) =============== \n\n" << std::endl; 

    RuleCollection::Arithmetic mathobj1(b, 10, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic mathobj2(b, 9, RuleCollection::MathOperation::SUBTRACT);
    RuleCollection::Arithmetic mathobj3(b, 4, RuleCollection::MathOperation::MULTIPLY);
    RuleCollection::Arithmetic mathobj4(b, 2, RuleCollection::MathOperation::DIVIDE);

    // std::vector<RuleCollection::GenRule> r = {&addobj1, &addobj2, &addobj3, &addobj4, &addobj5};
    std::vector<RuleCollection::GenRule*> r;
    r.push_back(&mathobj1);
    r.push_back(&mathobj2);
    r.push_back(&mathobj3);
    r.push_back(&mathobj4);

    int a = 100;
    RuleCollection::Condition end_condition {b, a, RuleCollection::ConditionType::GREATER};

    RuleCollection::Loop loopobj(end_condition, RuleCollection::LoopType::UNTIL, r);

    loopobj.func();

    std::cout << "b is: " << b << std::endl; 

    return 0;
}