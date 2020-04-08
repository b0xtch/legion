#include <algorithm>
#include <stdio.h>
#include "RuleCollection.h"

int main() {
    std::cout << "Testing timer\n";

    int addition = 0;
    RuleCollection::Arithmetic add1(addition, 2, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic add2(addition, 3, RuleCollection::MathOperation::ADD);
    RuleCollection::Arithmetic add3(addition, 1, RuleCollection::MathOperation::SUBTRACT);
    RuleCollection::Arithmetic add4(addition, 2, RuleCollection::MathOperation::SUBTRACT);
    RuleCollection::Arithmetic add5(addition, 3, RuleCollection::MathOperation::MULTIPLY);

    std::vector<RuleCollection::GenRule*> rulesList;
    rulesList.push_back(add1);
    rulesList.push_back(add2);
    rulesList.push_back(add3);
    rulesList.push_back(add4);
    rulesList.push_back(add5);

    RuleCollection::Timer timer(2, RuleCollection::TimerMode::AT_MOST, rulesList);
    timer->func();

    return 0;
}