#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameServer.h"

TEST(GameServerConfigTests, parse) {
    GameServerConfig gsc{};
    gsc.parse("{ \"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }");
    ASSERT_STREQ("./fake/directory/", gsc.getGameConfigDir().c_str());
    ASSERT_EQ(373, gsc.getMaxSessions());
    ASSERT_EQ(473, gsc.getMaxConnections());
}

TEST(GameServerConfigTests, parseBadJson) {
    GameServerConfig gsc{};
    EXPECT_THROW(gsc.parse(""), std::runtime_error);
    EXPECT_THROW(gsc.parse("{ ,\"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }"), std::runtime_error);
}

TEST(GameServerConfigTests, parseBadKeys) {
    GameServerConfig gsc{};
    EXPECT_THROW(gsc.parse("{ \"gamez\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }"), std::runtime_error);
}

TEST(GameServerConfigTests, parseBadValues) {
    GameServerConfig gsc{};
    EXPECT_THROW(gsc.parse("{ \"games\":\"./fake/directory/\", \"maxSessions\":\"not a number\", \"maxConnections\":473 }"), std::runtime_error);
    EXPECT_THROW(gsc.parse("{ \"games\":\"./fake/directory/\", \"maxSessions\":\"not a number\", \"maxConnections\":null }"), std::runtime_error);
}