#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Session.h"
#include <string_view>

/**
 * Test session id get created of size 6
 * **/
Test(Session, isSessionIdCreated){
    Session session = Session();
    std::string_view id = session.getSessionId(id);
    ASSERT_EQ(6, id.size());
}


/**
 * Test adding client
 * **/
Test(Session, addingClient){
    Session session = Session();
    Connection c{};
    session.addClient(c);
    ASSERT_TRUE(session.clients.find(c)!=session.clients.end());
}


/**
 * Test removing client
 * **/
Test(Session, removingClient){
    Session session = Session();
    Connection c{};
    session.addClient(c);
    session.removeClient(c);
    ASSERT_EQ(0, session.clients.size());
}


/**
 * Test if client is part of session
 * **/
Test(Session, isClient){
    Session session = Session();
    Connection c{};
    session.addClient(c);
    ASSERT_TRUE(session.isClient(c));
}


/**
 * Test retrieving session id
 * **/
Test(Session, retrieveSessionId){
    Session session = Session();
    ASSERT_TRUE(session.getSessionId().size() > 0);
}
