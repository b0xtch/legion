#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameServer.h"

// WIP: GameServerConfig should be made more testable?

TEST(GameServerConfigTests, constructor) {
    GameServerConfig gsc{};
    gsc.parse("{ \"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }");
    ASSERT_STREQ("./fake/directory/", gsc.getGameConfigDir().c_str());
    ASSERT_EQ(373, gsc.getMaxSessions());
    ASSERT_EQ(473, gsc.getMaxConnections());
}

TEST(GameServerConfigTests, constructorWithBadJson) {
    // https://stackoverflow.com/a/23271612 (testing exceptions)
    GameServerConfig gsc{};
    try {
        gsc.parse("{ ,\"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }");
        FAIL(); // If parse didn't throw an exception, then the test failed.
    }
    catch (std::runtime_error& e) {
        
    }
}

TEST(GameServerConfigTests, constructorWithBadKeys) {
    GameServerConfig gsc{};
    try {
        gsc.parse("{ \"gamez\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }");
        FAIL();
    }
    catch (std::runtime_error& e) {
        
    }
}