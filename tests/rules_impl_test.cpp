#include <algorithm>
#include <string>
#include <iostream>

#include <RuleCollection_TEST.h>

int retMod2(int x) {return x % 2;}
int retMod3(int x) {return x % 3;}

int main()
{
    int b = 1;

    std::cout << "\n\n=============== Testing Loop (Until) =============== \n\n" << std::endl; 

    RuleCollection::Arithmetic mathobj1(b, 10, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic mathobj2(b, 9, RuleCollection::MathOperation::SUBTRACT);
    RuleCollection::Arithmetic mathobj3(b, 4, RuleCollection::MathOperation::MULTIPLY);
    RuleCollection::Arithmetic mathobj4(b, 2, RuleCollection::MathOperation::DIVIDE);

    std::vector<RuleCollection::GenRule*> r;
    r.push_back(&mathobj1);
    r.push_back(&mathobj2);
    r.push_back(&mathobj3);
    r.push_back(&mathobj4);

    int max1 = 100;
    RuleCollection::Condition<int> end_condition1 {b, max1, RuleCollection::ConditionType::GREATER};

    RuleCollection::Loop<int> loopobj(end_condition1, RuleCollection::LoopType::UNTIL, r);

    loopobj.func();

    std::cout << "b is: " << b << std::endl; 

    std::cout << "\n\n=============== Testing Loop (While) with When =============== \n\n" << std::endl; 

    int c = 0;
    int check1 = 20;
    int check2 = 10;
    int check3 = 5;

    //condiion for cases
    RuleCollection::Condition<int> caseCond1 {c, check1, RuleCollection::ConditionType::GREATER};
    RuleCollection::Condition<int> caseCond2 {c, check2, RuleCollection::ConditionType::GREATER};
    RuleCollection::Condition<int> caseCond3 {c, check3, RuleCollection::ConditionType::GREATER};

    //rules to run if case is valid
    RuleCollection::Arithmetic addobj1(c, 1, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic addobj2(c, 2, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic addobj3(c, 3, RuleCollection::MathOperation::ADD);

    //rules to run for each case (only 1 thing ran twice)
    std::vector<RuleCollection::GenRule*> rC1 = {&addobj1, &addobj1};
    std::vector<RuleCollection::GenRule*> rC2 = {&addobj2, &addobj2};
    std::vector<RuleCollection::GenRule*> rC3 = {&addobj3, &addobj3};

    //cases
    RuleCollection::Case<int> case1 {caseCond1, rC1};
    RuleCollection::Case<int> case2 {caseCond2, rC3};
    RuleCollection::Case<int> case3 {caseCond2, rC3};

    //CaseList
    std::vector<RuleCollection::Case<int>> caseList = {case1, case2, case3};

    //when
    RuleCollection::When<int> whenobj (caseList);

    //while loop condition
    RuleCollection::Condition<int> end_condition2 {c, max1, RuleCollection::ConditionType::LESS};

    //while loop rules
    RuleCollection::Arithmetic mathobj5(c, 10, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic mathobj6(c, 9, RuleCollection::MathOperation::SUBTRACT);
    RuleCollection::Arithmetic mathobj7(c, 4, RuleCollection::MathOperation::MULTIPLY);
    RuleCollection::Arithmetic mathobj8(c, 2, RuleCollection::MathOperation::DIVIDE);

    std::vector<RuleCollection::GenRule*> r2 = {};

    //adding obj to rules to run in loop
    r2.push_back(&mathobj5);
    r2.push_back(&whenobj);

    r2.push_back(&mathobj6);
    r2.push_back(&whenobj);

    r2.push_back(&mathobj7);
    r2.push_back(&whenobj);

    r2.push_back(&mathobj8);
    r2.push_back(&whenobj);

    RuleCollection::Loop<int> loopobj2(end_condition2, RuleCollection::LoopType::WHILE, r2);

    loopobj2.func();



    return 0;
}