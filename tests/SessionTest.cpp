#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Session.h"
#include <string_view>

/**
 * Test session id get created of size 6
 * **/
TEST(Session, isSessionIdCreated){
    Session session = Session();
    std::string_view id = session.getSessionId();
    ASSERT_EQ(6, id.size());
}


/**
 * Test adding client
 * **/
TEST(Session, addingClient){
    Session session = Session();
    Connection c{};
    session.addUser(c);
    ASSERT_TRUE(session.isUser(c));
}


/**
 * Test removing client
 * **/
TEST(Session, removingClient){
    Session session = Session();
    Connection c{};
    session.addUser(c);
    session.removeUser(c);
    ASSERT_EQ(0, session.getNumberOfUsers());
}


/**
 * Test if client is part of session
 * **/
TEST(Session, isClient){
    Session session = Session();
    Connection c{};
    session.addUser(c);
    ASSERT_TRUE(session.isUser(c));
}


/**
 * Test retrieving session id
 * **/
TEST(Session, retrieveSessionId){
    Session session = Session();
    ASSERT_TRUE(session.getSessionId().size() > 0);
}
