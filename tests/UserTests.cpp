#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "User.h"
#include "Server.h"

networking::Connection makeTestConn(uintptr_t id) {
    networking::Connection conn{};
    conn.id = id;
    return conn;
}

TEST(UserTests, constructor) {
    auto conn = makeTestConn(373);
    User u{conn};
    ASSERT_EQ(373, u.getConn().id);
    ASSERT_EQ(conn, u.getConn());
}

TEST(UserTests, name) {
    User u{makeTestConn(373)};
    std::string str{"Foo Bar"};
    u.setName(str);
    ASSERT_STREQ("Foo Bar", u.getName().c_str());
}

TEST(UserTests, nameEmpty) {
    User u{makeTestConn(373)};
    std::string str{""};
    u.setName(str);
    ASSERT_STREQ("", u.getName().c_str());
}