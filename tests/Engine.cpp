
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <json.hpp>
#include "engine.h"

using json    = nlohmann::json;
using Engine::String;

using Engine::Game;
using Engine::Configuration;
using Engine::Constants;
using Engine::Variables;
using Engine::PerPlayer;
using Engine::PerAudience;
using Engine::Rules;

using Engine::EngineImpl
using ::testing::InSequence;
using ::testing::Return;

template <typename Type>
class MockEngineIml : public EngineImpl<Type>{
public:
    MOCK_METHOD0(buildGame, Game());
    MOCK_METHOD1(initalizeEngine, Game(const Type& input));
    MOCK_METHOD1(validGameConfig, bool(const Type& input));

    // Private Class methods
    MOCK_METHOD1(setConfiguration, Configuration(const Type& input));
    MOCK_METHOD1(setConstants, Constants(const Type& input));
    MOCK_METHOD1(setVariables, Variables(const Type& input));
    MOCK_METHOD1(setPerPlayer, PerPlayer(const Type& input));
    MOCK_METHOD1(setPerAudience, PerAudience(const Type& input));
    MOCK_METHOD1(setRules, Rules(const Type& input));
};

class EngineTests: public ::testing::Test{
protected:
    void SetUp() override{}

    void TearDown() override{}

    json readJsonFromFile(const String& file_path) {
        std::ifstream input(file_path);

        if(input.fail()) {
            throw std::runtime_error("Unable to open file " + file_path);
        }

        return json::parse(input);
    }

    MockEngineIml<json> game;
    json data = readJsonFromFile("./data.json");
};

TEST_F(EngineTests, INITIALIZE_ENGINE){
    {
        InSequence s;
        EXPECT_CALL(game, initalizeEngine(data))
        .Times(1)
        //.WillOnce(Return());
        // TODO check if the function returns an instance of a Game type

        EXPECT_CALL(game, validGameConfig(data))
        .Times(1)
        .WillOnce(Return(true)); 

        EXPECT_CALL(game, buildGame())
        .Times(1)
        .WillOnce(Return(true));
    }

    EXPECT_CALL(game, setConfiguration(data["configuration"]))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(game, setConstants(data["constants"]))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(game, setVariables(data["variables"]))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(game, setPerPlayer(data["per-player"]))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(game, setPerAudience(data["per-audience"]))
    .Times(1)
    .WillOnce(Return(true));

    EXPECT_CALL(game, setRules(data["rules"]))
    .Times(1)
    .WillOnce(Return(true));
}