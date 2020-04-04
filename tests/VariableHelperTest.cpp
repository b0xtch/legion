#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <iomanip>
#include <string>
#include "json.hpp"
#include "jsonDSL.h"
#include "variableHelper.h"

namespace VH = VariableHelper;

using varMap = std::map<JsonDSL::VariableDataType, varCollection>;

class varHelperTests : public ::testing::Test{
protected:
    varMap testMap;
    varMap emptyMap;

    JsonDSL::VariableDataType boolMap = JsonDSL::VarBoolean;
    JsonDSL::VariableDataType listMap = JsonDSL::VarList;
    JsonDSL::VariableDataType objMap = JsonDSL::VarObject;
    JsonDSL::VariableDataType intMap = JsonDSL::VarInteger;
    JsonDSL::VariableDataType strMap = JsonDSL::VarString;

    void SetUp() override {
        testMap = VH::getEmptyVarMap();
        emptyMap = VH::getEmptyVarMap();
        testMap[boolMap].push_back("true");
        testMap[boolMap].push_back("false");
        testMap[boolMap].push_back("bool_lit");

        testMap[intMap].push_back("1");
    }
};

TEST_F(varHelperTests, detectBoolExpressionWithInvalidArgNum){
    std::string expr = "true ==";
    EXPECT_FALSE(VH::isBooleanExpression(expr, testMap));
}

TEST_F(varHelperTests, detectValidBoolExpression){
    std::string expr = "true == false";
    EXPECT_TRUE(VH::isBooleanExpression(expr,testMap));
}

TEST_F(varHelperTests, detectBooleanLiteral){
    std::string expr = "bool_lit";
    EXPECT_TRUE(VH::isBooleanExpression(expr,testMap));
}

TEST_F(varHelperTests, detectInvalidTypeComparison){
    std::string expr = "1 == true";
    EXPECT_FALSE(VH::isBooleanExpression(expr,testMap));
}

TEST_F(varHelperTests, whitespaceDoesNotAffectComparison){
    std::string expr1 = "  bool_lit  ";
    EXPECT_TRUE(VH::isBooleanExpression(expr1,testMap));
    std::string expr2 = "  true  ==  false  ";
    EXPECT_TRUE(VH::isBooleanExpression(expr2,testMap));
}

TEST_F(varHelperTests, detectVarAccessLocations){
    std::string expr = "{string}";
    int beginBraceLoc = 0;
    int endBraceLoc = 7;
    auto locs = VH::getVarAccessLocations(expr);
    EXPECT_EQ(locs.first, beginBraceLoc);
    EXPECT_EQ(locs.second, endBraceLoc);
}

TEST_F(varHelperTests, canExtractVarAccessStr){
    std::string expr = "{string}";
    std::string varName = "string";
    auto extractedVars = VH::extractStringVarAccesses(expr);
    EXPECT_EQ(extractedVars.size(), 1);
    EXPECT_EQ(extractedVars[0], varName); 
}

TEST_F(varHelperTests, canHandleEmptyVarAccess){
    std::string expr = "{}";
    auto extractedVars = VH::extractStringVarAccesses(expr);
    EXPECT_EQ(extractedVars[0], ""); 
}

TEST_F(varHelperTests, canExtractMultipleVars){
    std::string expr = "the {quick} {brown} fox jumped over the {lazy} dog";
    std::string firstVar = "quick";
    std::string secondVar = "brown";
    std::string thirdVar = "lazy";
    auto extractedVars = VH::extractStringVarAccesses(expr);
    EXPECT_EQ(extractedVars.size(), 3);
    EXPECT_EQ(extractedVars[0], firstVar);
    EXPECT_EQ(extractedVars[1], secondVar);
    EXPECT_EQ(extractedVars[2], thirdVar);
}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
