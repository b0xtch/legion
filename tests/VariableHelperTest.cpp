#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "json.hpp"
#include "jsonDSL.h"
#include "variableHelper.h"

namespace VH = VariableHelper;

using json = nlohmann::json;
using varCollection = std::vector<std::string>;
using varMap = std::map<JsonDSL::VariableDataType, varCollection>;

class varHelperTests : public ::testing::Test{
protected:

    std::string directoryPrefix;

    varMap testMap;
    varMap emptyMap;

    JsonDSL::VariableDataType boolMap = JsonDSL::VarBoolean;
    JsonDSL::VariableDataType listMap = JsonDSL::VarList;
    JsonDSL::VariableDataType objMap = JsonDSL::VarObject;
    JsonDSL::VariableDataType intMap = JsonDSL::VarInteger;
    JsonDSL::VariableDataType strMap = JsonDSL::VarString;

    void SetUp() override {

        directoryPrefix = "tests/testfiles/variableHelperFiles/";

        testMap = VH::getEmptyVarMap();
        emptyMap = VH::getEmptyVarMap();
        testMap[boolMap].push_back("true");
        testMap[boolMap].push_back("false");
        testMap[boolMap].push_back("bool_lit");

        testMap[intMap].push_back("1");
    }

    json getJsonFromFile(std::string filePath){
        std::ifstream jsonFile (filePath);
        return json::parse(jsonFile);
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

TEST_F(varHelperTests, canCollectLiteralsFromConstAndVarFields){
    std::string fileLocation = directoryPrefix + "constVarLitsOnly.json";
    json j_object = getJsonFromFile(fileLocation);
    varMap map = VH::collectVarNames(j_object);

    EXPECT_EQ(2, map[intMap].size());
    EXPECT_EQ(4, map[boolMap].size()); //true and false are added to the list
    EXPECT_EQ(2, map[strMap].size());

    std::string constInt = "constA";
    std::string varBool = "varB";
    std::string constStr = "constC";

    auto intIt = std::find(map[intMap].begin(), map[intMap].end(), constInt);
    auto boolIt = std::find(map[boolMap].begin(), map[boolMap].end(), varBool);
    auto strIt = std::find(map[strMap].begin(), map[strMap].end(), constStr);

    EXPECT_NE(map[intMap].end(), intIt);
    EXPECT_NE(map[boolMap].end(), boolIt);
    EXPECT_NE(map[strMap].end(), strIt);
}

TEST_F(varHelperTests, canCollectSetUpVars){
    std::string fileLocation = directoryPrefix + "setUpLiteralVars.json";
    json j_object = getJsonFromFile(fileLocation);
    varMap map = VH::collectVarNames(j_object);

    EXPECT_EQ(0, map[objMap].size());
    EXPECT_EQ(2, map[intMap].size());
    EXPECT_EQ(4, map[boolMap].size()); //true and false are added to the list
    EXPECT_EQ(1, map[strMap].size());

    std::string userDefInt = "setUpD";
    std::string userDefBool = "setUpE";
    std::string setUpStr = "setUpC";

    auto intIt = std::find(map[intMap].begin(), map[intMap].end(), userDefInt);
    auto boolIt = std::find(map[boolMap].begin(), map[boolMap].end(), userDefBool);
    auto strIt = std::find(map[strMap].begin(), map[strMap].end(), setUpStr);

    EXPECT_NE(map[intMap].end(), intIt);
    EXPECT_NE(map[boolMap].end(), boolIt);
    EXPECT_NE(map[strMap].end(), strIt);
}

TEST_F(varHelperTests, canDetectIfExpressionIsFuncCall){
    std::vector<std::string> expressions = {};
    expressions.push_back("notAFuncCall");
    expressions.push_back("freeFuncCall()");
    expressions.push_back("obj.callWithArg(arg)");
    expressions.push_back("obj.func.call()");

    std::vector<std::optional<std::vector<size_t>>> results = {};

    for(auto expr : expressions){
        results.push_back(VH::isMethodCall(expr));
    }

    EXPECT_FALSE(results[0].has_value());
    EXPECT_FALSE(results[1].has_value());
    ASSERT_TRUE(results[2].has_value());
    ASSERT_TRUE(results[3].has_value());

    std::vector<size_t> expr2Vals = {3,15,19};
    std::vector<size_t> expr3Vals = {8,13,14};

    EXPECT_TRUE(expr2Vals == results[2].value());
    EXPECT_TRUE(expr3Vals == results[3].value());
}

TEST_F(varHelperTests, canDetectValidMethodCall){
    MethodProperties upFromMethod = {JsonDSL::VarInteger, 
                                    JsonDSL::VarInteger, 
                                    "upFrom"};
    
    
    MethodProperties intStrMethod = {JsonDSL::VarInteger, 
                                    JsonDSL::VarString, 
                                    "addFromString"};

    using functionList = std::vector<MethodProperties>;
    functionList funcList = {upFromMethod, intStrMethod};

    std::string fileLocation = directoryPrefix + "constVarLitsOnly.json";
    json j_object = getJsonFromFile(fileLocation);
    varMap map = VH::collectVarNames(j_object);

    std::string expr1 = "varA.upFrom(constA)";
    std::string expr2 = "constA.addFromString(varC)";

    EXPECT_TRUE(VH::isValidMethodCall(expr1, map, funcList, j_object));
    EXPECT_TRUE(VH::isValidMethodCall(expr2, map, funcList, j_object));
}

TEST_F(varHelperTests, collectVarNamesDoesNotAlterJson){
    std::string fileLocation = directoryPrefix + "constVarLitsOnly.json";
    json j_object = getJsonFromFile(fileLocation);
    varMap map = VH::collectVarNames(j_object);

    EXPECT_TRUE(j_object["constants"]["constA"].is_number());
    EXPECT_TRUE(j_object["variables"]["varB"].is_boolean());
}

TEST_F(varHelperTests, canGetJsonFieldFromVar){
    std::string fileLocation = directoryPrefix + "constVarLitsOnly.json";
    json j_object = getJsonFromFile(fileLocation);
    varMap map = VH::collectVarNames(j_object);

    auto variablesJson = VH::getJsonFieldVarIsFrom(j_object, "varA");
    auto emptyJson = VH::getJsonFieldVarIsFrom(j_object, "nonexistentVarName");

    EXPECT_FALSE(emptyJson.has_value());
    ASSERT_TRUE(variablesJson.has_value());

    EXPECT_TRUE(variablesJson.value().contains("varA"));
    EXPECT_TRUE(variablesJson.value().contains("varB"));
    EXPECT_TRUE(variablesJson.value().contains("varC"));
}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
//TEST_F(varHelperTests, name){}
