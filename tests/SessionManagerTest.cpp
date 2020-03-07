#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "SessionManager.h"
#include <string_view>

/**
 * Test max session  
 * **/
Test(SessionManager, maxSessions){
    SessionManager manager{5};
    ASSERT_EQ(5, manager.getMaxSessions());
}


/**
 * Test adding connections 
 * **/
Test(Session, addingConnection){
    SessionManager manager{5};
    Connection c{};
    manager.addConnection(c);
    auto it = find(manager.unassignedConnections.begin(), unassignedConnections.end(), c);
    ASSERT_TRUE(it != unassignedConnections.end());
}


/**
 * Test removing connections
 * **/
Test(Session, removingConnections){
    SessionManager manager{5};
    Connection c{};
    manager.addConnection(c);
    manager.removeConnetions(c)
    auto it = find(manager.unassignedConnections.begin(), unassignedConnections.end(), c);
    ASSERT_TRUE(it == unassignedConnections.end());
}

