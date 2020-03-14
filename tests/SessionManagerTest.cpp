#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SessionManager.h"
#include <string_view>

/**
 * Test max session  
 * **/
TEST(SessionManager, maxSessions){
    SessionManager manager{5};
    ASSERT_EQ(5, manager.getMaxSessions());
}


/**
 * Test adding connections 
 * **/
TEST(Session, addingConnection){
    SessionManager manager{5};
    Connection c{};
    manager.addConnection(c);
    std::set<networking::Connection> unassignedConnections = manager.getUnassignedConnections();
    ASSERT_TRUE(unassignedConnections.find(c) != unassignedConnections.end());
}


/**
 * Test removing connections
 * **/
TEST(Session, removingConnections){
    SessionManager manager{5};
    Connection c{};
    manager.addConnection(c);
    manager.removeConnection(c);
    std::set<networking::Connection> unassignedConnections = manager.getUnassignedConnections();
    ASSERT_TRUE(unassignedConnections.find(c) == unassignedConnections.end());
}

