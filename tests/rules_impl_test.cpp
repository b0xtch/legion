#include <algorithm>
#include <string>
#include <iostream>

#include <RuleCollection.h>


// void loopImpl(RuleCollection::Loop &data){
//     switch(data.type)
//     {
//         case RuleCollection::LoopType::UNTIL:
//             while(!data.con){
//                 for(auto rule : data.rules_to_run){
//                     rule.func();
//                 }
//             }
//         case RuleCollection::LoopType::WHILE:
//             while(data.con){
//                 for(auto rule : data.rules_to_run){
//                     rule.func();
//                 }
//             }
//     }
// }


// void forEachImpl(RuleCollection::ForEach);


int main()
{
    int b = 1;

    RuleCollection::Arithmetic addobj1(b, 1, RuleCollection::MathOperation::ADD);

    std::cout << "b is: " << b << std::endl; 
    addobj1.func();
    std::cout << "b is: " << b << std::endl; 

    RuleCollection::Arithmetic addobj2(b, 2, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic addobj3(b, 3, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic addobj4(b, 4, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic addobj5(b, 5, RuleCollection::MathOperation::ADD);

    // std::vector<RuleCollection::GenRule> r = {addobj1, addobj2, addobj3, addobj4, addobj5};
    std::vector<RuleCollection::GenRule*> r;
    r.push_back(&addobj1);
    r.push_back(&addobj2);
    r.push_back(&addobj3);
    r.push_back(&addobj4);
    r.push_back(&addobj5);

    for(auto x : r){
        std::cout << x->rule_name << " " << x->to << " " << x->value << std::endl;
    }


    // bool end_condition = b>100;

    // RuleCollection::Loop loopobj(end_condition, RuleCollection::LoopType::UNTIL, r);

    // loopobj.func();
    // std::cout << "b is: " << b << std::endl; 

    return 0;
}