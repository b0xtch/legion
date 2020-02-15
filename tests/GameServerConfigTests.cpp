#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "GameServer.h"

// WIP: GameServerConfig should be made more testable?

TEST(GameServerConfigTests, constructor) {
    GameServerConfig gsc{"{ \"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }"};
    ASSERT_STREQ("./fake/directory/", gsc.getGameConfigDir());
    ASSERT_EQ(373, gsc.getMaxSessions());
    ASSERT_EQ(473, gsc.getMaxConnections());
}

TEST(GameServerConfigTests, constructorWithBadJson) {
    GameServerConfig gsc{"{,, [] \"games\":\"./fake/directory/\", \"maxSessions\":373, \"maxConnections\":473 }"};
}

TEST(GameServerConfigTests, constructorWithBadKeys) {
    GameServerConfig gsc{"{ \"gamez\":\"./fake/directory/\", \"maxconnections\":473 }"};
}